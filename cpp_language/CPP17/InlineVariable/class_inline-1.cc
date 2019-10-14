#include <iostream>
#include "class_inline.h"

void printIn1() {
    std::cout <<"1:" << MyClass::msg << std::endl;
}

void modifyInline() {
    MyClass::msg = "hello world";
}

int main() {
    printIn0();
    printIn1();

    modifyInline();

    printIn0();
    printIn1();
}