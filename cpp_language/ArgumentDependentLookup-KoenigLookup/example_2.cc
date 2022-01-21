//
// Created by HUANG WEN JUN on 2021/12/25.
//
#include <iostream>
class C{
public:
    struct Y{};
    static void h(Y) {std::cout << "calling C::h()\n";}
};
int main(){
    C::Y y;
//    h(y); // compile error

}
