//
// Created by rick on 2021/5/6.
//
#include <chrono>
#include <iomanip>
#include <iostream>

using namespace std;
using namespace std::chrono;

int main()
{
    chrono::minutes oneMinute { 1 };
    chrono::seconds sixtySeconds = oneMinute; // coarser unit to more precise unit
    chrono::minutes oneMinuteAgain = duration_cast<minutes>(sixtySeconds); // muse use duration_cast to cast from coarser unit to more precise unit because of information lose

    duration<double, ratio<60>> halfMin { 0.5 };
    //    seconds s1 = halfMin; // ERROR
    seconds s1 = duration_cast<seconds>(halfMin); // floating-point tick type also require duration_cast

    // split into hours,minutes,seconds, and milliseconds
    milliseconds ms { 7255042 };
    hours hh = duration_cast<hours>(ms);
    minutes mm = duration_cast<minutes>(ms % hours(1));
    seconds ss = duration_cast<seconds>(ms % minutes(1));
    milliseconds msec = duration_cast<milliseconds>(ms % seconds(1));

    cout << "raw: " << hh << "::" << mm << "::"
         << ss << "::" << msec << endl;

    cout << "     " << setfill('0') << setw(2) << hh.count() << "::"
         << setw(2) << mm.count() << "::"
         << setw(2) << ss.count() << "::"
         << setw(3) << msec.count() << endl;
}