//
// Created by HUANG WEN JUN on 2020/11/17.
//
#include <memory>
#include <rx.hpp>
#include <thread>

int main() {
    auto eventLoop = rxcpp::observe_on_event_loop();
    auto values    = rxcpp::observable<>::interval(std::chrono::seconds(2))
                      .take(2)
                      .publish();

    values.subscribe_on(eventLoop).subscribe(
        [](int v) {
            std::cout << "[1] onNext: " << v
                      << " thread:" << std::this_thread::get_id() << std::endl;
        },
        []() { printf("[1] onCompleted"); });
    values.subscribe_on(eventLoop).subscribe(
        [](int v) {
            std::cout << "[2] onNext: " << v
                      << " thread:" << std::this_thread::get_id() << std::endl;
        },
        []() { printf("[2] onCompleted"); });

    values.connect();
    values.as_blocking().subscribe();
    rxcpp::observable<>::timer(std::chrono::milliseconds(2000))
        .subscribe([&](long) {});
}
