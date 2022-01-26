//
// Created by HUANG WEN JUN on 2021/12/29.
//

#include <algorithm>
#include <iostream>
#include <vector>
#include <string>
#include <fmt/printf.h>

int main() {
    std::vector<std::string> input = {"hello", "world"};
    std::for_each(std::begin(input), std::end(input), [](const auto &v) {
        fmt::print("{}\n", v);
    });

    std::vector<std::string> output(2);
    std::move(input.begin(), input.end(), output.begin());
    fmt::print("vector size:{}\n", input.size());
    std::for_each(std::begin(input), std::end(input), [](const auto &v) {
        fmt::print("{}\n", v);
    });

}
