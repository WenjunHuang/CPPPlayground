//
// Created by rick on 2020/11/20.
//
#include <iostream>
#include <rx.hpp>
#include <thread>

int main() {
    auto values = rxcpp::observable<>::range(1, 4)
                      .transform([](int v) { return v * v; })
                      .concat(rxcpp::observable<>::error<int>(
                          std::runtime_error("Error from producer!")));
    std::cout << "Main Thread id => " << std::this_thread::get_id()
              << std::endl;
    values.observe_on(rxcpp::synchronize_new_thread())
        .as_blocking()
        .subscribe(
            [](int v) {
                std::cout << "Observable Thread id => "
                          << std::this_thread::get_id() << " " << v
                          << std::endl;
            },
            [](std::exception_ptr ep) {
                printf("OnError: %s\n", rxcpp::util::what(ep).c_str());
            },
            []() { std::cout << "OnCompleted" << std::endl; });

    std::cout << "Main Thread id => " << std::this_thread::get_id()
              << std::endl;
}