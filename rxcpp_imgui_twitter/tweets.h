//
// Created by rick on 2020/11/20.
//

#pragma once

#include "model.h"
#include "rxcurl.h"
#include "util.h"
#include <chrono>
#include <cpprest/oauth1.h>
#include <fstream>
#include <map>
#include <range/v3/action.hpp>
#include <range/v3/algorithm.hpp>
#include <range/v3/range.hpp>
#include <range/v3/view.hpp>
#include <rxcpp/rx.hpp>
#include <string>
#include <unordered_set>
#include <vector>

using namespace std;
using namespace std::chrono;
using namespace rxcpp;
namespace Rx {
using namespace rxcpp;
using namespace rxcpp::sources;
using namespace rxcpp::operators;
using namespace rxcpp::subjects;
} // namespace Rx
using namespace Rx;
using namespace model;
using namespace ::util;
using namespace rxcurl;
using namespace web::http::oauth1::experimental;

namespace tweets {
inline milliseconds timestamp_ms(const Tweet& tw)
{
    auto& tweet = tw.data->tweet;
    auto t = milliseconds(stoll(tweet["timestamp_ms"].get<string>()));
    return t;
}

auto isEndOfTweet = [](const string& s) {
    if (s.size() < 2)
        return false;
    auto it0 = s.begin() + (s.size() - 2);
    auto it1 = s.begin() + (s.size() - 1);
    return *it0 == '\r' && *it1 == '\n';
};

struct parseerror {
    std::exception_ptr ep;
};
struct parsedtweets {
    observable<Tweet> tweets;
    observable<parseerror> errors;
};

inline auto parsetweets(observe_on_one_worker worker,
    observe_on_one_worker tweetthread)
{
    return [=](observable<string> chunks) -> observable<parsedtweets> {
        return rxs::create<parsedtweets>([=](subscriber<parsedtweets> out) {
            // create strings split on \r
            auto strings = chunks | concat_map([](const string& s) {
                auto splits = split(s, "\r\n");
                return iterate(move(splits));
            }) | filter([](const string& s) {
                return !s.empty();
            }) | publish()
                | ref_count();

            // filter to last string in each line
            auto closes = strings | filter(isEndOfTweet) | rxo::map([](const string&) { return 0; });

            // group strings by line
            auto linewindows = strings | window_toggle(closes | start_with(0), [=](int) { return closes; });

            // reduce the strings for a line into one string
            auto lines = linewindows | flat_map([](const observable<string>& w) {
                return w | start_with<string>("") | sum();
            });
            int count = 0;
            rxsub::subject<parseerror> errorconduit;
            observable<Tweet> tweets = lines | filter([](const string& s) {
                return s.size() > 2 && s.find_first_not_of("\r\n") != string::npos;
            }) | group_by([count](const string&) mutable -> int {
                return ++count % std::thread::hardware_concurrency();
            }) | rxo::map([=](observable<string> shard) {
                return shard | observe_on(worker) | rxo::map([=](const string& line) -> observable<Tweet> {
                    try {
                        auto tweet = json::parse(line);
                        return rxs::from(Tweet(tweet));
                    } catch (...) {
                        errorconduit.get_subscriber().on_next(parseerror { std::current_exception() });
                    }
                    return rxs::empty<Tweet>();
                }) | merge();
            }) | merge(tweetthread)
                | tap([](const Tweet&) {}, [=]() { errorconduit.get_subscriber().on_completed(); })
                | finally([=]() {
                      errorconduit.get_subscriber().unsubscribe();
                  });
            out.on_next(parsedtweets { tweets, errorconduit.get_observable() });
            out.on_completed();

            return out.get_subscription();
        });
    };
}

inline auto onlytweets() -> function<observable<Tweet>(observable<Tweet>)>
{
    return [](observable<Tweet> s) {
        return s | filter([](const Tweet& tw) {
            auto& tweet = tw.data->tweet;
            return !!tweet.count("timestamp_ms");
        });
    };
}

enum class errorcodeclass {
    Invalid,
    TcpRetry,
    ErrorRetry,
    StatusRetry,
    RateLimited
};

inline errorcodeclass errorclassfrom(const HttpException& ex)
{
    switch (ex.code()) {
    case CURLE_COULDNT_RESOLVE_HOST:
    case CURLE_COULDNT_CONNECT:
    case CURLE_OPERATION_TIMEDOUT:
    case CURLE_BAD_CONTENT_ENCODING:
    case CURLE_REMOTE_FILE_NOT_FOUND:
        return errorcodeclass::ErrorRetry;
    case CURLE_GOT_NOTHING:
    case CURLE_PARTIAL_FILE:
    case CURLE_SEND_ERROR:
    case CURLE_RECV_ERROR:
        return errorcodeclass::TcpRetry;
    default:
        if (ex.code() == CURLE_HTTP_RETURNED_ERROR || ex.httpStatus() > 200) {
            if (ex.httpStatus() == 420) {
                return errorcodeclass::RateLimited;
            } else if (ex.httpStatus() == 404 || ex.httpStatus() == 406 || ex.httpStatus() == 413 || ex.httpStatus() == 416) {
                return errorcodeclass::Invalid;
            }
        }
    };
    return errorcodeclass::StatusRetry;
}

auto filechunks = [](observe_on_one_worker tweetthread, string filepath) {
    return observable<>::create<string>([=](subscriber<string> out) {
        auto values = make_tuple(ifstream { filepath }, string {});
        auto state = make_shared<decltype(values)>(move(values));

        // creates a worker whose lifetime is the same as this subscription
        auto coordinator = tweetthread.create_coordinator(out.get_subscription());
        auto controller = coordinator.get_worker();
        auto producer = [out, state](const rxsc::schedulable& self) {
            if (!out.is_subscribed()) {
                // terminate loop
                return;
            }
            if (getline(get<0>(*state), get<1>(*state))) {
                get<1>(*state) += "\r\n";
                out.on_next(get<1>(*state));
            } else {
                out.on_completed();
                return;
            }

            // tail recurse this same action to continue loop
            self();
        };

        controller.schedule(coordinator.act(producer));
    });
};

auto twitter_stream_reconnection = [](observe_on_one_worker tweetthread) {
    return [=](observable<string> chunks) {
        return chunks | timeout(seconds(90), tweetthread)
            | on_error_resume_next([=](std::exception_ptr ep) -> observable<string> {
                  try {
                      rethrow_exception(ep);
                  } catch (const HttpException& ex) {
                      cerr << ex.what() << endl;
                      switch (errorclassfrom(ex)) {
                      case errorcodeclass::TcpRetry:
                          cerr << "reconnecting after TCP error" << endl;
                          return observable<>::empty<string>();
                      case errorcodeclass::ErrorRetry:
                          cerr << "error code (" << ex.code() << ") -";
                      case errorcodeclass::StatusRetry:
                          cerr << "http status (" << ex.httpStatus() << ") - waiting to retry.." << endl;
                          return observable<>::timer(seconds(5), tweetthread) | stringAndIgnore();
                      case errorcodeclass::RateLimited:
                          cerr << "rate limited - waiting to retry.." << endl;
                          return observable<>::timer(minutes(1), tweetthread) | stringAndIgnore();
                      case errorcodeclass::Invalid:
                          cerr << "invalid request - propagate" << endl;
                      default:
                          cerr << "unrecognized error  - propagate" << endl;
                      };
                  }
                  //                  catch(const TimeoutError& ex) {
                  //                      cerr << "reconnecting after timeout" <<endl;
                  //                      return observable<>::empty<string>();
                  //                  }
                  catch (const exception& ex) {
                      cerr << "unknown exception - terminate" << endl;
                      cerr << ex.what() << endl;
                      terminate();
                  } catch (...) {
                      cerr << "unknown exception - not derived from std::exception - terminate" << endl;
                      terminate();
                  }
                  return observable<>::error<string>(ep, tweetthread);
              })
            | repeat();
    };
};

auto twitterrequest = [](observe_on_one_worker tweetthread, ::rxcurl::RxCurl factory,
                          string URL, string method, string CONS_KEY, string CONS_SEC, string ATOK_KEY, string ATOK_SEC) {
    return observable<>::defer([=]() {
        string url;
        {
            char* signedurl = nullptr;
            RXCPP_UNWIND_AUTO([&]() {
                if (signedurl != nullptr)
                    free(signedurl);
            });
            //            utility::string_t consumer_key,
            //            utility::string_t consumer_secret,
            //            utility::string_t temp_endpoint,
            //            utility::string_t auth_endpoint,
            //            utility::string_t token_endpoint,
            //            utility::string_t callback_uri,
            //            oauth1_method method,
            oauth1_config config {
                utility::conversions::to_string_t(CONS_KEY),
                utility::conversions::to_string_t(CONS_SEC),
                U(""),
                U(""),
                U(""),
                U(""),
                oauth1_methods::hmac_sha1
            };
            oauth1_token token {};
            //            config.set_token()
            //            signedurl =
            //TODO Oauth url
        }

        cerr << "start twitter stream request" << endl;

        return factory.create(HttpRequest { url, method, {}, {} }) | rxo::map([](HttpResponse r) {
            return r.body.chunks;
        }) | finally([]() { cerr << "end twitter stream request" << endl; })
            | merge(tweetthread);
    }) | twitter_stream_reconnection(tweetthread);
};

auto sentimentrequest = [](observe_on_one_worker worker, ::rxcurl::RxCurl factory, string url, string key, vector<string> text) -> observable<string> {
    static const regex nonascii(R"([^A-Za-z0-9])");

    std::map<string, string> headers;
    headers["Content-Type"] = "application/json";
    headers["Authorization"] = "Bearer " + key;

    auto body = json::parse(R"({"Inputs":{"input1":[]},"GlobalParameters":{}})");
    auto& input1 = body["Inputs"]["input1"];
    for (auto& t : text) {
        auto ascii = regex_replace(t, nonascii, " ");
        input1.push_back({ { "tweet_text", ascii } });
    }

    return observable<>::defer([=]() -> observable<string> {
        return factory.create(HttpRequest { url, "POST", headers, body.dump() })
            | rxo::map([](const HttpResponse& r) {
                  return r.body.complete;
              })
            | merge(worker)
            | tap([=](const exception_ptr&) {
                  cout << body << endl;
              });
    });
};

auto perspectiverequest = [](observe_on_one_worker worker,::rxcurl::RxCurl factory,string url,string key,string text)->observable<string> {
    std::map<string,string> headers;
    headers["Content-Type"] = "application/json";
    url += "?key=" + key;
    auto body = json::parse(R"({"comment": {"text": ""}, "languages": ["en"], "requestedAttributes": {"TOXICITY":{}, "INFLAMMATORY":{}, "SPAM":{}}, "doNotStore": true })");
    body["comment"]["text"] = text;

    return observable<>::defer([=]()->observable<string>{
      return factory.create(HttpRequest{url,"POST",headers,body.dump()})
        | rxo::map([](const HttpResponse& r){
                  return r.body.complete;
              }) |
      merge(worker) |
      tap([=](const exception_ptr&){
                cout << body << endl;
            })
    });

};
}