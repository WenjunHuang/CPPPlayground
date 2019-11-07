//
// Created by rick on 2019/11/7.
//
#include <iostream>
#include <variant>

struct NoDefConstr {
    NoDefConstr(int i) { std::cout << "NoDefConstr::NoDefConstr"; }
};

int main() {
    std::variant<std::monostate, NoDefConstr, int> v2;
    std::cout << "index:" << v2.index() << "\n";

    if(std::holds_alternative<std::monostate>(v2))
        std::cout <<"has monostate\n";

    if (std::get_if<0>(&v2))
        std::cout <<"has monostate\n";

    if (std::get_if<std::monostate>(&v2))
        std::cout <<"has monstate\n";
}