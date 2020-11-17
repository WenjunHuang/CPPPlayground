//
// Created by HUANG WEN JUN on 2020/11/17.
//

#include <iostream>
#include <mutex>
#include <rx.hpp>
#include <thread>

std::mutex consoleMutex;

void CTDetails() {
    consoleMutex.lock();
    std::cout << "Current Thread id => " << std::this_thread::get_id()
              << std::endl;
    consoleMutex.unlock();
}

void yield(bool y) {
    if (y) {
        std::this_thread::yield();
    }
}

int main() {
    auto threads = rxcpp::observe_on_event_loop();
    auto values  = rxcpp::observable<>::range(1);
    auto s1      = values.subscribe_on(threads).map([](int prime) {
        CTDetails();
        yield(true);
        return std::make_tuple("1:", prime);
    });
//    s1.take(6).as_blocking()
//    .subscribe(rxcpp::util::apply_to([](const char* s,int p){
//      CTDetails();
//        consoleMutex.lock();
//        printf("%s %d\n",s,p);
//        consoleMutex.unlock();
//        }));
    auto s2      = values.subscribe_on(threads).map([](int prime) {
      CTDetails();
      yield(true);
      return std::make_tuple("2:", prime);
    });

    s1.merge(s2).take(6).as_blocking().subscribe(rxcpp::util::apply_to([](const char *s,int p){
        CTDetails();
        consoleMutex.lock();
        printf("%s %d\n",s,p);
        consoleMutex.unlock();
    }));
}