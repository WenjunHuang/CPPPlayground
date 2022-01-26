//
// Created by HUANG WEN JUN on 2022/1/2.
//

#include <type_traits>
#include <iostream>
#include <iomanip>

struct Trivial {
    int i;
private:
    int j;
};

struct Trivial2 {
    int i;

    Trivial2(int a, int b) : i(a), j(b) {}

    Trivial2() = default;

private:
    int j;
};

struct Base {
    int i;
    int j;
};

struct Derived : public Base {
    int x;
    int y;
};

struct DerivedWithoutMember: public Base {
    void foo(){}
};

struct Composite {
    Base b;
    int z;
};

int main() {
    std::cout.setf(std::ios_base::boolalpha);
    std::cout << "Is Trivial trivial?" << std::is_trivial_v<Trivial>
              << std::endl;
    std::cout << "Is Trivial2 trivial?" << std::is_trivial_v<Trivial2>
              << std::endl;


    std::cout << "Is Base standard-layout?"
              << std::is_standard_layout_v<Base>
              << std::endl;
    std::cout << "Is Derived standard-layout?"
              << std::is_standard_layout_v<Derived>
              << std::endl;
    std::cout << "Is Composite standard-layout?"
              << std::is_standard_layout_v<Composite>
              << std::endl;
    std::cout << "Is DerivedWithoutMember standard-layout?"
              << std::is_standard_layout_v<DerivedWithoutMember>
              << std::endl;
}