//
// Created by rick on 2020/11/23.
//
#include <chrono>
#include <rx.hpp>

using namespace rxcpp;
using namespace std::chrono;

int main() {
    auto period = seconds(1);
    auto obs =
        observable<>::interval(period, observe_on_new_thread())
            .tap([](int i) { std::cout << "Publishing " << i << std::endl; })
            .publish();
    obs.connect();

    std::cout << "Press any key to subscribe" << std::endl;
    std::cin.get();

    auto subs = obs.subscribe([](int i){
        std::cout << "subscription: " << i << std::endl;
    });
    std::cout << "Press any key to unsubscribe." << std::endl;
    std::cin.get();
    subs.unsubscribe();

    std::cout << "Press any key to exit." << std::endl;
    std::cin.get();
}