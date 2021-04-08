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
    inline milliseconds timestamp_ms(const Tweet& tw) {
        auto& tweet = tw.data->tweet;
        auto t      = milliseconds(stoll(tweet["timestamp_ms"].get<string>()));
        return t;
    }

    auto isEndOfTweet = [](const string& s) {
        if (s.size() < 2)
            return false;
        auto it0 = s.begin() + (s.size() - 2);
        auto it1 = s.begin() + (s.size() - 1);
        return *it0 == '\r' && *it1 == '\n';
    };

    struct ParseError {
        std::exception_ptr ep;
    };

    struct ParsedTweets {
        observable<Tweet> tweets;
        observable<ParseError> errors;
    };

    inline auto parseTweets(observe_on_one_worker worker,
                            observe_on_one_worker tweetThread) {
        return [=](observable<string> chunks) {
            return create<ParsedTweets>([=](subscriber<ParsedTweets> out) {
                auto strings =
                    chunks | concat_map([](const string& s) {
                        auto splits = split(s, "\r\n");
                        return iterate(std::move(splits));
                    }) |
                    filter([](const string& s) { return !s.empty(); }) |
                    publish() | ref_count();

                auto closes = strings | filter(isEndOfTweet) |
                              rxo::map([](const string&) { return 0; });

                auto lineWindows =
                    strings | window_toggle(closes | start_with(0),
                                            [=](int) { return closes; });

                auto lines =
                    lineWindows | flat_map([](const observable<string>& w) {
                        return w | start_with<string>("") | sum();
                    });

                int count = 0;
                rxsub::subject<ParseError> errorConduit;
                observable<Tweet> tweets =
                    lines | filter([](const string& s) {
                        return s.size() > 2 &&
                               s.find_first_not_of("\r\n") != string::npos;
                    }) |
                    group_by([count](const string&) mutable {
                        return ++count & std::thread::hardware_concurrency();
                    }) |
                    rxo::map([=](observable<string> shard) {
                        return shard |
                               observe_on(worker) |
                               rxo::map([=](const string& line)
                                            -> observable<Tweet> {
                                   try {
                                       auto tweet = json::parse(line);
                                       return rxs::from(Tweet(tweet));
                                   } catch (...) {
                                       errorConduit.get_subscriber().on_next(
                                           ParseError{
                                               std::current_exception()});
                                   }
                                   return rxs::empty<Tweet>();
                               }) |
                               merge() |
                               as_dynamic();
                    }) |
                    merge(tweetThread) |
                    tap([](const Tweet&) {},
                        [=]() {
                            errorConduit.get_subscriber().on_completed();
                        }) |
                    finally(
                        [=]() { errorConduit.get_subscriber().unsubscribe(); });
                out.on_next(
                    ParsedTweets{tweets, errorConduit.get_observable()});
                out.on_completed();
                return out.get_subscription();
            });
        };
    }

    inline auto onlyTweets() {
        return [](observable<Tweet> s) {
            return s | filter([](const Tweet& tw) {
                       auto& tweet = tw.data->tweet;
                       return !!tweet.count("timestamp_ms");
                   });
        };
    }

    enum class ErrorCodeClass {
        Invalid,
        TcpRetry,
        ErrorRetry,
        StatusRetry,
        RateLimited
    };

    inline ErrorCodeClass errorClassFrom(const HttpException& ex) {
        switch (ex.code()) {
        case CURLE_COULDNT_RESOLVE_HOST:
        case CURLE_COULDNT_CONNECT:
        case CURLE_OPERATION_TIMEDOUT:
        case CURLE_BAD_CONTENT_ENCODING:
        case CURLE_REMOTE_FILE_NOT_FOUND: return ErrorCodeClass::ErrorRetry;
        case CURLE_GOT_NOTHING:
        case CURLE_PARTIAL_FILE:
        case CURLE_SEND_ERROR:
        case CURLE_RECV_ERROR: return ErrorCodeClass::TcpRetry;
        default:
            if (ex.code() == CURLE_HTTP_RETURNED_ERROR ||
                ex.httpStatus() > 200) {
                if (ex.httpStatus() == 420) {
                    return ErrorCodeClass::RateLimited;
                } else if (ex.httpStatus() == 404 || ex.httpStatus() == 406 ||
                           ex.httpStatus() == 413 || ex.httpStatus() == 416) {
                    return ErrorCodeClass::Invalid;
                }
            }
        }
        return ErrorCodeClass::StatusRetry;
    }

    auto fileChunks = [](observe_on_one_worker tweetThread, string filePath) {
        return observable<>::create<string>([=](subscriber<string> out) {
            auto values = make_tuple(ifstream{filePath}, string{});
            auto state  = make_shared<decltype(values)>(move(values));

            auto coordinator =
                tweetThread.create_coordinator(out.get_subscription());
            auto controller = coordinator.get_worker();
            auto producer   = [out, state](const rxsc::schedulable& self) {
                if (!out.is_subscribed()) {
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

    auto twitterStreamReconnection = [](observe_on_one_worker tweetThread) {
        return [=](observable<string> chunks) {
            return chunks | timeout(seconds(90), tweetThread) |
                   on_error_resume_next([=](std::exception_ptr ep)
                                            -> observable<string> {
                       try {
                           rethrow_exception(ep);
                       } catch (const HttpException& ex) {
                           cerr << ex.what() << endl;
                           switch (errorClassFrom(ex)) {
                           case ErrorCodeClass::TcpRetry:
                               cerr << "reconnecting after TCP error" << endl;
                               return observable<>::empty<string>();
                           case ErrorCodeClass::ErrorRetry:
                               cerr << "error code (" << ex.code() << ") -";
                           case ErrorCodeClass::StatusRetry:
                               cerr << "http status (" << ex.httpStatus()
                                    << ") - waiting to retry.." << endl;
                               return observable<>::timer(seconds(5),
                                                          tweetThread) |
                                      stringAndIgnore();
                           case ErrorCodeClass::RateLimited:
                               cerr << "rate limited - waiting to retry.."
                                    << endl;
                               return observable<>::timer(minutes(1),
                                                          tweetThread) |
                                      stringAndIgnore();
                           case ErrorCodeClass::Invalid:
                               cerr << "invalid request - propagate" << endl;
                           default:
                               cerr << "unrecognized error - propagate" << endl;
                           }
                       } catch (const timeout_error& ex) {
                           cerr << "reconnecting after timeout" << endl;
                           return observable<>::empty<string>();
                       } catch (const exception& ex) {
                           cerr << "unknown exception - terminate" << endl;
                           cerr << ex.what() << endl;
                           terminate();
                       } catch (...) {
                           cerr << "unknown exception - not derived from "
                                   "std::exception - terminate"
                                << endl;
                           terminate();
                       }

                       return observable<>::error<string>(ep, tweetThread);
                   }) |
                   repeat();
        };
    };

    auto twitterRequest = [](observe_on_one_worker tweetThread, RxCurl factory,
                             string url, string method, string consKey,
                             string consSec, string atokKey, string atokSec) {
        return observable<>::defer([=]() {
                   oauth1_config config(to_wstring(consKey),
                                        to_wstring(consSec), {}, {}, {},
                                        to_wstring(url), to_wstring(method));
                   config.set_token(
                       oauth1_token{to_wstring(atokKey), to_wstring(atokSec)});
                   string url =
                       to_string(config.build_authorization_uri().get());

                   cerr << "start twitter stream request" << endl;

                   return factory.create(HttpRequest{url, method, {}, {}}) |
                          rxo::map(
                              [](HttpResponse r) { return r.body.chunks; }) |
                          finally([]() {
                              cerr << "end twitter stream request" << endl;
                          }) |
                          merge(tweetThread);
               }) |
               twitterStreamReconnection(tweetThread);
    };

    auto sentimentRequest = [](observe_on_one_worker worker, RxCurl factory,
                               string url, string key,
                               vector<string> text) -> observable<string> {
        std::map<string, string> headers;
        headers["Content-Type"]  = "application/json";
        headers["Authorization"] = "Bearer " + key;

        auto body =
            json::parse(R"({"Inputs":{"input1":[]},"GlobalParameters":{}})");
        static const regex noneAscii(R"([^A-Za-z0-9])");

        auto& input1 = body["Inputs"]["input1"];
        for (auto& t : text) {
            auto ascii = regex_replace(t, noneAscii, " ");
            input1.push_back({{"tweet_text", ascii}});
        }

        return observable<>::defer([=]() -> observable<string> {
            return factory.create(
                       HttpRequest{url, "POST", headers, body.dump()}) |
                   rxo::map([](HttpResponse r) { return r.body.complete; }) |
                   merge(worker) |
                   tap([=](exception_ptr) { cout << body << endl; });
        });
    };


    auto perspectiveRequest = [](observe_on_one_worker worker,RxCurl factory,string url,
                                 string key,string text)->observable<string> {
        std::map<string,string> headers;
        headers["Content-Type"] = "application/json";
        url += "?key=" + key;


        auto body = json::parse(R"({"comment":{"text":""},"languages":["en"],"requestedAttributes":{"TOXICITY":{},"INFLAMMATORY":{},"SPAM":{}},"doNotStore":true})");
        body["comment"]["text"] = text;

        return observable<>::defer([=]()->observable<string>{
            rxo::map([](HttpResponse r){
                return r.body.complete;
            }) |
            merge(worker) |
            tap([=](exception_ptr){
                    cout << body << endl;
                });
        });
    };
} // namespace tweets