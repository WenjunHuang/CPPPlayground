//
// Created by HUANG WEN JUN on 2021/12/25.
//
#include <iostream>

class Base {
public:
    void foo(int i) {
        std::cout << "Base::foo(int)\n";
    }
};

class Derived : public Base {
public:
    void foo() {
        std::cout << "Derived::foo()\n";
    }
};

int main() {
    Derived d;

    /*
     * Not compile:
     * - Base::foo(int) was hidden by Derived::foo
     */
//    d.foo(10);


// Compiles: use qualified name
    d.Base::foo(10);
}