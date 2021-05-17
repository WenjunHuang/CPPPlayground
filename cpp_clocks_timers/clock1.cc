//
// Created by rick on 2021/5/6.
//
#include <chrono>
#include "clock.h"
using namespace std;
using namespace std::chrono;

int main() {
    cout << "system_clock: " << endl;
    printClockData<system_clock>();

    cout << "\nhigh_resolution_clock: " << endl;
    printClockData<high_resolution_clock>();

    cout << "\nsteady_clock: " << endl;
    printClockData<steady_clock>();
}