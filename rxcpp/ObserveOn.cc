//
// Created by HUANG WEN JUN on 2020/11/17.
//
#include <rx.hpp>
#include <thread>
int main() {
    auto values = rxcpp::observable<>::range(1,4)
    .map([](int v){return v*v;});
    std::cout << "Main Thread id =>" << std::this_thread::get_id() << std::endl;

    values.observe_on(rxcpp::synchronize_new_thread())
    .as_blocking()
    .subscribe([](int v){
            std::cout << "Observable Thread id=>"
            << std::this_thread::get_id() << std::endl;
        });
}
