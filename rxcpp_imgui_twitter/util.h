//
// Created by rick on 2020/11/15.
//

#pragma once
#include <algorithm>
#include <chrono>
#include <functional>
#include <iomanip>
#include <numeric>
#include <regex>
#include <sstream>
#include <string>
#include <filesystem>
#include <ctime>
#include <rxcpp/rx.hpp>

using namespace std;
using namespace std::chrono;
namespace Rx {
    using namespace rxcpp;
    using namespace rxcpp::sources;
    using namespace rxcpp::operators;
    using namespace rxcpp::subjects;
} // namespace Rx
using namespace Rx;

namespace util {
    const float fltmax = numeric_limits<float>::max();

    inline string tolower(string s) {
        transform(s.begin(), s.end(), s.begin(),
                  [=](char c) { return std::tolower(c); });
        return s;
    }

    enum class Split { KeepDelimiter, RemoveDelimiter, OnlyDelimiter };

    auto split = [](string s, const string& d, Split m = Split::KeepDelimiter) {
        regex delim{d};
        cregex_token_iterator cursor(&s[0],&s[0] + s.size(),delim,m ==Split::KeepDelimiter? initializer_list<int>({-1,0}):(m==Split::RemoveDelimiter?initializer_list<int>({-1}):initializer_list<int>({0})));
        cregex_token_iterator end;
        vector<string> splits(cursor,end);
        return splits;
    };

    inline string utcTextFrom(seconds time){
        stringstream buffer;
        time_t tb = time.count();
        tm tmb{};
        gmtime_s(&tmb,&tb);

        buffer << put_time(&tmb,"%a %b %d %H:%M:%S %Y");
        return buffer.str();
    }

    inline string utcTextFrom(system_clock::time_point time = system_clock::now()){
        return utcTextFrom(time_point_cast<seconds>(time).time_since_epoch());
    }

    inline function<observable<string>(observable<long>)> stringAndIgnore() {
        return [](observable<long> s){
            return s.map([](long){return string{};}).ignore_elements();
        };
    }

    template<typename To,typename Rep,typename Period>
    To floor(const duration<Rep,Period>& d){
        To t = std::chrono::duration_cast<To>(d);
        if (t > d)
            return t - To{1};
        return t;
    }

    std::wstring to_wstring(const std::string& str) {
        return std::filesystem::path(str).wstring();
    }

    std::string to_string(const std::wstring& wstr){
        return std::filesystem::path(wstr).string();
    }
} // namespace util