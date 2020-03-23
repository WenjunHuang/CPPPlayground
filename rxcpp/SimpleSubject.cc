//
// Created by rick on 2020/3/23.
//
#include <rx.hpp>
#include <memory>

int main() {
    rxcpp::subjects::subject<int> subject;
    auto observable = subject.get_observable();

    // subscribe twice
    observable.subscribe([](int v){
        printf("1-------%d\n",v);
    });
    observable.subscribe([](int v){
      printf("2-------%d\n",v);
    });

    // get the subscriber interface
    auto subscriber = subject.get_subscriber();
    subscriber.on_next(1);
    subscriber.on_next(4);
    subscriber.on_next(9);
    subscriber.on_next(16);

    rxcpp::observable<>::timer(std::chrono::milliseconds(2000))
    .subscribe([](long){});
}