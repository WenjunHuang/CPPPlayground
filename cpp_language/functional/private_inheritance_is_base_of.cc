//
// Created by rick on 2020/1/30.
//
#include <iostream>
#include <type_traits>
class Base {
  public:
    int a;
};

class PublicDerived: public Base{};
class PrivateDerived: private Base{};

template<typename Allocator>
class List {
     Allocator* allocptr;
     friend Allocator;
};

int main(){
    std::cout << std::boolalpha;

    std::cout << "PublicDerived is derived from Base:" << std::is_base_of_v<Base,PublicDerived> << std::endl;
    std::cout << "PrivateDerived is derived from Base:" << std::is_base_of_v<Base,PrivateDerived> << std::endl;
}