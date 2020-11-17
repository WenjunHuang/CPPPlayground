//
// Created by HUANG WEN JUN on 2020/11/17.
//

#include <iostream>
#include <rx.hpp>

#include <array>

int main() {
    std::array<std::string, 4> a = {"Praseed", "Peter", "Sanjey"};
    auto values                  = rxcpp::observable<>::iterate(a).concat_map(
        [](std::string v) {
            std::array<std::string, 3> salutation = {"Mr.", "Monsieur", "Sir"};
            return rxcpp::observable<>::iterate(salutation);
        },
        [](std::string f, std::string s) { return s + " " + f; });
    values.subscribe([](std::string f) { std::cout << f << std::endl; },
                     []() { std::cout << "Hello World.." << std::endl; });
}
