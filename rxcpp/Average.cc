//
// Created by HUANG WEN JUN on 2020/11/17.
//
#include <rx.hpp>

int main() {
    auto values = rxcpp::observable<>::range(1, 20).average();
    values.subscribe([](double v) { printf("average: %lf\n", v); },
                     []() { printf("OnCompleted\n"); });
}
