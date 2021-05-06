//
// Created by rick on 2021/4/26.
//
#include <rx.hpp>
#include <rx-test.hpp>
#include <array>
#include <iostream>

namespace rxu = rxcpp::util;
using namespace rxcpp;
using namespace rxcpp::operators;

int main()
{
    std::array<int, 4> a = { { 10, 20, 30, 40 } };

    auto lambda_fn = [](int n) { return std::to_string(n * 2); };

    auto transform = [=](rxcpp::subscriber<std::string> dest) {
        return rxcpp::make_subscriber<int>(dest,
            rxcpp::make_observer_dynamic<int>(
                [=](int n) {
                    dest.on_next(lambda_fn(n));
                },
                [=](std::exception_ptr e) { dest.on_error(e); },
                [=]() { dest.on_completed(); }));
    };

    auto values = rxcpp::observable<>::iterate(a);
    values.lift<std::string>(transform).subscribe([](const std::string& f) { std::cout << f << std::endl; },
        []() { std::cout << "Hello World.." << std::endl; });
}
