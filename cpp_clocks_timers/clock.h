//
// Created by rick on 2021/5/6.
//

#pragma once

#include <chrono>
#include <iomanip>
#include <iostream>

using namespace std;
using namespace std::chrono;

template <typename C>
void printClockData()
{
    cout << "- precision: ";

    // if time unit is less or equal one millisecond
    using P = typename C::period; // type of time unit
    if constexpr (ratio_less_equal_v<P, milli>) {
        // convert to and print as milliseconds
        using TT = typename ratio_multiply<P, kilo>::type;
        cout << fixed << double(TT::num) / TT::den
             << " milliseconds" << endl;
    } else {
        // print as seconds
        cout << fixed << double(P::num)/P::den << " seconds" << endl;
    }
    cout << "- is_steady: " << boolalpha << C::is_steady << endl;
}