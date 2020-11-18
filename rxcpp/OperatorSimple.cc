//
// Created by HUANG WEN JUN on 2020/11/18.
//
#include <rx.hpp>
#include <iostream>
#include <array>

using namespace rxcpp;
using namespace rxcpp::operators;
namespace rxu=rxcpp::util;

observable<std::string> helloNames(observable<std::string> src){
    return src.map([](const std::string& s){
        return "Hello, " + s + "!";
    });
}

int main() {
    std::array<std::string,4> a{"Praseed","Peter","Sanjay"};
    auto values = helloNames()
}