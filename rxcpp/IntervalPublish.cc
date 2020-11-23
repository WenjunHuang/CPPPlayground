//
// Created by rick on 2020/11/23.
//

#include <rx.hpp>
#include <chrono>
#include <iostream>
#include <thread>

using namespace std::chrono;
using namespace std;
using namespace rxcpp;

int main() {
    auto period = seconds(1);
    auto obs = observable<>::interval(period,observe_on_event_loop()).publish();
    obs.connect();

    obs.subscribe([](int i ){
        cout << "first subscription:" << i << endl;
    });

    std::this_thread::sleep_for(period);
    obs.subscribe([](int i ){
      cout << "second subscription:" << i << endl;
    });

    char c;
    std::cin >> c;
}