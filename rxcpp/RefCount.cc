//
// Created by HUANG WEN JUN on 2020/11/17.
//
#include <chrono>
#include <iostream>
#include <rx.hpp>
using namespace std::chrono;
int main() {
    auto observable =
        rxcpp::observable<>::interval(seconds(1),
                                      rxcpp::observe_on_event_loop())
            .tap([](long value) { printf("Publishing %ld\n", value); })
            .publish()
            .ref_count();

    std::cout << "Press any key to subscribe first" << std::endl;
    std::cin.ignore();
    auto subscription =
        observable.subscribe([](auto v) { printf("subscription 1: %ld\n", v); });

    std::cout << "Press any key to subscribe second" << std::endl;
    std::cin.ignore();
    auto subscription2 =
        observable.subscribe([](auto v) { printf("subscription 2: %ld\n", v); });

    std::cout << "Press any key to unsubscribe first" << std::endl;
    std::cin.ignore();
    subscription.unsubscribe();

    std::cout << "Press any key to unsubscribe second" << std::endl;
    std::cin.ignore();
    subscription2.unsubscribe();

    std::cout << "Press any key to exit." << std::endl;
    std::cin.ignore();
}
