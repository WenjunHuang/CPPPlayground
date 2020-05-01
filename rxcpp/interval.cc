//
// Created by rick on 2020/3/27.
//
#include <rx.hpp>
#include <chrono>

int main(){
    auto just = rxcpp::observable<>::just(1);
    rxcpp::observable<>::interval(std::chrono::seconds(1))
    .take(10)
    .subscribe([](auto value){
        std::cout << "Received: " << value << std::endl;
    });
}