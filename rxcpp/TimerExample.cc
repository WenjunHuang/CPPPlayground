//
// Created by HUANG WEN JUN on 2020/11/17.
//

#include <chrono>
#include <iostream>
#include <rx.hpp>

int main() {
    auto scheduler = rxcpp::observe_on_new_thread();
    auto period    = std::chrono::milliseconds(1000);
    auto values = rxcpp::observable<>::timer(period, scheduler).finally([]() {
        printf("The final action\n");
    });
    values.as_blocking().subscribe([](int v) { printf("OnNext: %d\n", v); },
                                   []() { printf("OnCompleted\n"); });
}