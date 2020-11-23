//
// Created by rick on 2020/3/27.
//
#include <chrono>
#include <rx.hpp>
using namespace std::chrono;

int main() {
    auto observable = rxcpp::observable<>::interval(std::chrono::seconds(1),rxcpp::observe_on_event_loop());

    observable.subscribe(
        [](int i) { std::cout << "first subscription:" << i << std::endl; });
    std::this_thread::sleep_for(seconds(1));
    observable.subscribe(
        [](int i) { std::cout << "second subscription:" << i << std::endl; });

    char c;
    std::cin >> c;
}