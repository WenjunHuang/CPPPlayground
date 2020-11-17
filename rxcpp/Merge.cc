//
// Created by HUANG WEN JUN on 2020/11/17.
//

#include <rx.hpp>
#include <iostream>
#include <array>

int main() {
    auto o1 = rxcpp::observable<>::range(1,3);
    auto o2 = rxcpp::observable<>::range(4,6);
    auto values = o1.merge(o2);
    values.subscribe(
        [](int v){printf("OnNext: %d\n",v);},
        [](){printf("OnCompleted\n");}
        );
}