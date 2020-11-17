//
// Created by rick on 2020/3/23.
//

#include <memory>
#include <rx.hpp>
#include <thread>
int main() {
    auto eventloop = rxcpp::observe_on_event_loop();
    auto values =
        rxcpp::observable<>::interval(std::chrono::seconds(2)).take(2);

    values.subscribe_on(eventloop).subscribe(
        [](int v) {
            std::cout << "[1] onNext " << v << " thread:" << std::this_thread::get_id() << std::endl;
        },
        []() { printf("[1] onCompleted\n"); });
    values.subscribe_on(eventloop).subscribe(
        [](int v) {
          std::cout << "[2] onNext " << v << " thread:" << std::this_thread::get_id() << std::endl;
        },
        []() { printf("[2] onCompleted\n"); });

    values.as_blocking().subscribe();

    rxcpp::observable<>::timer(std::chrono::seconds(2))
    .subscribe([](long){});
}