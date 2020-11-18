//
// Created by HUANG WEN JUN on 2020/11/18.
//
#include <functional>
#include <rx.hpp>

std::function<rxcpp::observable<int>(rxcpp::observable<int>)>
getOddNumberSquare() {
    return [](rxcpp::observable<int> item) {
        return item.filter([](int v) { return v % 2; })
            .map([](const int v) { return v * v; })
            .take(3)
            .on_error_resume_next([](std::exception_ptr) {
                return rxcpp::sources::error<int>(
                    std::runtime_error("custom exception"));
            });
    };
}

int main() {
    auto value = rxcpp::observable<>::range(1, 7) | getOddNumberSquare();
    value.subscribe([](int v) { printf("OnNext: %d\n", v); },
                    []() { printf("OnCompleted\n"); });
}
