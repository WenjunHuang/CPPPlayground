//
// Created by rick on 2021/5/6.
//
#include <chrono>
#include <ctime>
#include <string>
#include <iostream>
#include <fmt/core.h>
#include <fmt/chrono.h>

using namespace std;
using namespace std::chrono;

string asString(const system_clock::time_point& tp){
    // convert to system time
    time_t t = system_clock::to_time_t(tp);
    char buffer[256] = {0};
    ctime_s(buffer,sizeof(buffer),&t);
//    ts.resize(ts.size()-1);
    return buffer;
}

int main() {
    // print the epoch of this system clock:
    system_clock::time_point tp;
    fmt::print("epoch: {}\n",asString(tp));
//    cout << "epoch: " << asString(tp) << endl;

    // print current time:
    tp = system_clock::now();
    fmt::print("now: {}\n",asString(tp));

    tp = system_clock::time_point::min();
    fmt::print("min: {}\n",asString(tp));

    tp = system_clock::time_point::max();
    fmt::print("max: {}\n",asString(tp));
}