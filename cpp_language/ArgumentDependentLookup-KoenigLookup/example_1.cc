//
// Created by HUANG WEN JUN on 2021/12/25.
//
#include <iostream>
namespace A {
    struct X{};
    void g(X){std::cout << "calling A::g()\n";}
};


//void g(A::X){std::cout <<"calling ::g()\n";}
int main(){
    A::X x1;
    g(x1);

}