//
// Created by rick on 2020/11/23.
//
#include <rx.hpp>
#include <iostream>
#include <chrono>
using namespace rxcpp;
using namespace std::chrono;

int main() {
    auto period = seconds(1);
    auto obs = observable<>::interval(period,observe_on_new_thread())
    .publish();
    obs.subscribe([](int i){
        std::cout << "subscription: " << i << std::endl;
    });

    bool exit =false;
    while(!exit){
        std::cout << "Press enter to connect, q to exit." << std::endl;
        auto c = std::cin.get();
         if (c == '\n'){
            auto connection = obs.connect();
            std::cout << "Press any key to dispose of connection." << std::endl;
            c = std::cin.get();
            connection.unsubscribe();
        }
        if (c == 'q') {
            exit =true;
        }
    }
}
