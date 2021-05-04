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
}