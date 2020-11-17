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

    std::cout << "Press any key to subscribe" << std::endl;
    char key;
    std::cin >> key;

    auto subscription =
        observable.subscribe([](auto v) { printf("subscription: %ld\n", v); });

    std::cout << "Press any key to unsubscribe." << std::endl;
    std::cin >> key;
    subscription.unsubscribe();

    std::cout << "Press any key to exit." << std::endl;
    std::cin >> key;
}
