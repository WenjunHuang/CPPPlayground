//
// Created by HUANG WEN JUN on 2020/11/17.
//
#include <rx.hpp>
#include <iostream>
int main() {
    auto ints = rxcpp::observable<>::range(1,3)
    .map([](int n){return n*n;});
    auto values = ints.tap(
        [](int v){printf("Tap - OnNext:%d\n",v);},
        [](){printf("Tap - OnCompleted\n");}
        );

    values.subscribe([](int v){printf("Subscribe - OnNext: %d\n",v);},
                     [](){printf("Subscribe - OnCompleted\n");});
}