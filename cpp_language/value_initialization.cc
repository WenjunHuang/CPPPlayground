//
// Created by HUANG WEN JUN on 2021/12/31.
//
#include <iostream>
#include <string>

struct A {

    A() {} // user-provided default ctor, does not initialize i
//private:
    int i;
};

struct B {
    A a;
//    std::string str;
};

int main() {
//    A b{0};
//    std::cout << zero << std::endl;
//    A a{};
//    std::cout << a.x << std::endl;
//    return a.x;

    std::cout << A().i << '\n';
    std::cout << B{}.a.i << '\n';
    for (int i = 0; i < 10; ++i) {
        std::cout << B{}.a.i << '\n';
    }
}
