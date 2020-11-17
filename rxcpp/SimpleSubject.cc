//
// Created by rick on 2020/3/23.
//
#include <memory>
#include <rx.hpp>

int main() {
    rxcpp::subjects::subject<int> subject;
    auto observable = subject.get_observable();
    observable.subscribe([](int v) { printf("1-------%d\n", v); });
    observable.subscribe([](int v) { printf("2--------%d\n", v); });

    auto subscriber = subject.get_subscriber();

    subscriber.on_next(1);
    subscriber.on_next(4);
    subscriber.on_next(9);
    subscriber.on_next(16);

    rxcpp::observable<>::timer(std::chrono::milliseconds(2000))
        .subscribe([&](long) {});
}