#include <iostream>
#include <rx.hpp>
#include <thread>
#include <tuple>

int main() {
    auto currentThreadId = std::this_thread::get_id();
    std::cout << "Main Thread: " << currentThreadId << std::endl;

    auto threads = rxcpp::observe_on_event_loop();
    auto values  = rxcpp::observable<>::range(1);

    auto s1 = values.subscribe_on(threads).map([](int num) {
        std::cout << "Received 1: " << num << " on "
                  << std::this_thread::get_id() << std::endl;
        return std::make_tuple("1: ", num);
    });

    auto s2 = values.subscribe_on(threads).map([](int num) {
        std::cout << "Received 2: " << num << " on "
                  << std::this_thread::get_id() << std::endl;
        return std::make_tuple("2: ", num);
    });

    s1.merge(s2).take(6).observe_on(threads)
//    .as_blocking()
    .subscribe(rxcpp::util::apply_to([](const char*s ,int p){
        std::cout << s << " " << p << '\n';
    }));
}