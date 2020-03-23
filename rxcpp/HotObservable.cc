//
// Created by rick on 2020/3/23.
//
#include <iostream>
#include <memory>
#include <rx.hpp>

int main() {
    auto eventloop = rxcpp::observe_on_event_loop();

    auto values = rxcpp::observable<>::interval(std::chrono::seconds(2))
                      .take(2)
                      .publish();
    //----- Subscribe Twice
    values.subscribe_on(eventloop).subscribe(
        [](int v) { std::cout << "[1] onNext: " << v << std::endl; },
        []() { std::cout << "[1] onCompleted" << '\n'; });

    values.subscribe_on(eventloop).subscribe(
        [](int v) { std::cout << "[2] onNext: " << v << std::endl; },
        []() { std::cout << "[2] onCompleted" << '\n'; });

    values.connect();
    values.as_blocking().subscribe();
    rxcpp::observable<>::timer(std::chrono::milliseconds(2000))
    .subscribe([](long){});
}