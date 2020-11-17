//
// Created by HUANG WEN JUN on 2020/11/17.
//

#include <memory>
#include <rx.hpp>

int main() {
    auto values = rxcpp::observable<>::interval(std::chrono::milliseconds(50),
                                                rxcpp::observe_on_new_thread())
                      .take(5)
                      .replay();
    values.subscribe([](long v) { printf("[1] OnNext: %ld\n", v); },
                     []() { printf("[1] OnCompleted\n"); });
    values.connect();

    rxcpp::observable<>::timer(std::chrono::milliseconds(125))
        .subscribe([&](long){
            values.as_blocking()
            .subscribe([](long v){
                    printf("[2] OnNext: %ld\n",v);
                },[](){printf("[2] OnCompleted\n");});
        });

    rxcpp::observable<>::timer(std::chrono::milliseconds(2000)).subscribe([](long){});
}