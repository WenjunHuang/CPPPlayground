//
// Created by rick on 2021/5/6.
//
#include <chrono>
#include <iostream>
using namespace std::chrono;
using namespace std;
template <typename V, typename R>
ostream& operator<<(ostream& s, const duration<V, R>& d)
{
    s << "[" << d.count() << " of " << R::num << "/" << R::den << "]";
    return s;
}
int main()
{
    duration<int> twentySeconds { 20 };
    duration<double, std::ratio<60>> halfAMinute(0.5);
    duration<long, std::ratio<1, 1000>> oneMillisecond { 1 };

    seconds tenSeconds { 10 };
    hours aDay { 24 };
    milliseconds twoMilliseconds { 2 };

    milliseconds ms;
    ms += twentySeconds + aDay;
    --ms;
    ms *= 2;
//    std::cout << ms.count() << " ms" << std::endl;
//    std::cout << nanoseconds { ms }.count() << " ns" << std::endl;
    std::cout << ms << std::endl;

}