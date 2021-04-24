//
// Created by HUANG WEN JUN on 2020/11/17.
//
#include <rx.hpp>

int main() {
    auto values = rxcpp::observable<>::range(1, 20).average();
    auto copyValues = values;
    values.subscribe([](double v) { printf("average: %lf\n", v); },
                     []() { printf("OnCompleted\n"); });
    printf("again\n");
    copyValues.subscribe([](double v) { printf("average: %lf\n", v); },
                     []() { printf("OnCompleted\n"); });
}
