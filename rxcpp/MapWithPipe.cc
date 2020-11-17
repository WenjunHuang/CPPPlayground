//
// Created by HUANG WEN JUN on 2020/11/17.
//

#include <iostream>
#include <rx.hpp>
namespace Rx {
    using namespace rxcpp;
    using namespace rxcpp::sources;
    using namespace rxcpp::operators;
    using namespace rxcpp::util;
} // namespace Rx
using namespace Rx;

int main() {
    auto ints = observable<>::range(1, 10)
                | map([](int n) { return n * n; });
    ints.subscribe([](int v){
        printf("OnNext: %d\n",v);
    });
}