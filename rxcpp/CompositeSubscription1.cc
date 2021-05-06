//
// Created by rick on 2021/5/6.
//
#include <rx.hpp>
#include <iostream>
using namespace std;
using namespace rxcpp;

struct PureUnSubscribeFunctionObject{
    void unsubscribe(){
        cout << "PureUnSubscribeFunctionObject unsubscribe" << endl;
    }
};

struct MySubscribtion:subscription_base {

};
int main(){
    composite_subscription cs;
    cs.add([](){
        cout << "cs unsubscribed" << endl;
    });

    subscription single1{PureUnSubscribeFunctionObject()};
//    if (single1.is_subscribed())
//        cout << "single1 is subscribed" << endl;

    auto coordination = rxcpp::identity_current_thread();
    auto worker       = coordination.create_coordinator().get_worker();
//    if (worker.is_subscribed())
//        cout << "worker is subscribed" << endl;

    worker.add(single1);
    auto weakSub = worker.add(cs);
    weakSub.lock()->unsubscribe();
}