//
// Created by rick on 2021/5/6.
//
// my is_subscription implementation with void_t
#include <type_traits>
#include <rx.hpp>
#include <iostream>
using namespace std;
using namespace rxcpp;

template <typename T, typename = void>
struct my_is_subscription : std::false_type {};

template <typename T>
struct my_is_subscription<T, std::void_t<std::enable_if_t<std::is_convertible_v<typename std::decay_t<T>::subscription_tag, tag_subscription>>>>
: std::true_type {};

template<typename T>using my_is_subscription_v = typename my_is_subscription<T>::value;

// to test
struct my_subscription : subscription_base{};

int main(){
    if constexpr (is_subscription<my_subscription>::value) {
        cout << "rxcpp is_subscription" << endl;
    }
    if constexpr (my_is_subscription<my_subscription>::value) {
        cout << "mine is_subscription" << endl;
    }

}
