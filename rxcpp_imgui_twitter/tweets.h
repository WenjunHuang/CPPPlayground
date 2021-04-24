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
    observe_on_one_worker tweetthread) {
    return [=](observable<string> chunks) -> observable<parsedtweets> {
        return rxs::create<parsedtweets>([=](subscriber<parsedtweets> out){
            // create strings split on \r
            auto strings = chunks |
                concat_map([](const string& s){
                    auto splits = split(s,"\r\n");
                    return iterate(move(splits));
                }) |
                filter([](const string& s){
                    return !s.empty();
                }) |
                publish() |
                ref_count();

        });
    };
}
}