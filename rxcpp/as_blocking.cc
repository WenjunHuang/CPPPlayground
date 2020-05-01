//
// Created by rick on 2020/4/5.
//
#include <rx.hpp>
#include <iostream>
#include <thread>

int main() {
    std::cout << "Main Thread: " << std::this_thread::get_id() << std::endl;
    rxcpp::schedulers::run_loop loop{};
    rxcpp::schedulers::make_run_loop()
    auto values = rxcpp::observable<>::from(rxcpp::observe_on_run_loop(),1,2,3)
        .tap([](int v) {
            std::cout << "[thread " << std::this_thread::get_id() << "] Emit value: " << v << std::endl;
        });
    values
//    .as_blocking()
    .subscribe([](int v){
        std::cout << "[Observe 1] [thread " << std::this_thread::get_id() << "] OnNext: " << v << std::endl;
    });

    values
//        .as_blocking()
        .subscribe([](int v){
          std::cout << "[Observe 2] [thread " << std::this_thread::get_id() << "] OnNext: " << v << std::endl;
        });

    std::this_thread::sleep_for(std::chrono::seconds(10));

}