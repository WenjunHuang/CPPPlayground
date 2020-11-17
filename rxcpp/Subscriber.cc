//
// Created by HUANG WEN JUN on 2020/11/17.
//
#include <rx.hpp>
int main() {
    auto subscription = rxcpp::composite_subscription();
    auto subscriber   = rxcpp::make_subscriber<int>(
        subscription,
        [&](int v) {
            printf("OnNext: --- %d\n", v);
            if (v == 3)
                subscription.unsubscribe();
        },
        []() { printf("OnCompleted\n"); });
    rxcpp::observable<>::create<int>(
        [](rxcpp::subscriber<int> s){
            for (int i = 0;i < 5;i++){
                if (!s.is_subscribed())
                    break;
                s.on_next(i);
            }
            s.on_completed();
        }).subscribe(subscriber);
}