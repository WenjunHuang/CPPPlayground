//
// Created by HUANG WEN JUN on 2020/11/17.
//

#include <array>
#include <iostream>
#include <rx.hpp>

int main() {
    std::array<int, 3> a{1, 2, 3};
    auto values = rxcpp::observable<>::iterate(a);
    values.subscribe([](int v) { printf("OnNext: %d\n", v); },
                     []() { printf("OnCompleted\n"); });
}