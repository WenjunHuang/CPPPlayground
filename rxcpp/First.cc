//
// Created by HUANG WEN JUN on 2020/11/17.
//
#include <rx.hpp>
#include <iostream>
int main() {
    auto observable = rxcpp::observable<>::range(1,12);
    observable.subscribe([](int v){printf("OnNext: %d\n",v);},[](){printf("OnCompleted\n");});

}