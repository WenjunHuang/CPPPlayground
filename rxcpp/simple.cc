//
// Created by rick on 2020/3/22.
//
#include <rx.hpp>

namespace Rx{
    using namespace rxcpp;
    using namespace rxcpp::sources;
    using namespace rxcpp::operators;
    using namespace rxcpp::util;
}

using namespace Rx;

#include <regex>
#include <random>
using namespace std;
using namespace std::chrono;

int main() {
    random_device rd;
    mt19937  gen(rd());
    uniform_int_distribution dist(4,18);

    auto bytes = range(0,10) |
        flat_map([&](int i) {
            auto body = from((uint8_t)('A' + i)) |
                repeat(dist(gen)) |
                as_dynamic();
            auto delim = from((uint8_t)'\r');
            return from(body,delim) | concat();
        }) |
        window(17) |
        flat_map([](observable<uint8_t> w) {
            return w | reduce(vector<uint8_t>(),[](vector<uint8_t> v,uint8_t b){
                v.push_back(b);
                return v;
            }) | as_dynamic();
        }) |
        tap([](vector<uint8_t>& v){
            copy(v.begin(),v.end(),ostream_iterator<long>(cout," "));
            cout << endl;
        });

    auto removespaces = [](string s) {
        s.erase(remove_if(s.begin(),s.end(),::isspace),s.end());
        return s;
    };
}