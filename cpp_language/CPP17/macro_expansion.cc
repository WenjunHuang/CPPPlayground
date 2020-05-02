//
// Created by rick on 2020/5/1.
//
#include <iostream>
#define TO_STR_INTERNAL(x) #x
#define FOO_STR_LITERAL(x) TO_STR_INTERNAL(x)
#define FOO_CONCAT(x,y) x##y
#define IDENTITY(x) x

int main() {
    std::cout << FOO_STR_LITERAL(a) << std::endl;
    std::cout << FOO_CONCAT(12,34) << std::endl;
    std::cout << FOO_CONCAT(FOO_STR_LITERAL(12),FOO_STR_LITERAL(34)) << std::endl;
    std::cout << FOO_STR_LITERAL(FOO_CONCAT(12,34)) << std::endl;

    std::cout << FOO_STR_LITERAL("bcd") << std::endl;

    std::cout << IDENTITY(FOO_STR_LITERAL(IDENTITY(FOO_STR_LITERAL(abc)))) << std::endl;

}