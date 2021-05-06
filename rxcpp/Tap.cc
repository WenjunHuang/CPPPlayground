//
// Created by rick on 2021/4/29.
//

#include <string>
#include <iostream>
#include <rx.hpp>
using namespace rxcpp;
using namespace std;

int main(){
    auto coordination = observe_on_new_thread();
    auto coordinator = coordination.create_coordinator();
    auto worker = coordinator.get_worker();

    observable<>::create<string>([=](subscriber<string> subs){
        worker.schedule([=](){

        });
    });
}