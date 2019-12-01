//
// Created by rick on 2019/11/7.
//
#include <iostream>
#include <variant>

struct NoDefConstr {
    NoDefConstr(int i) { std::cout << "NoDefConstr::NoDefConstr"; }

    bool operator==(const NoDefConstr&other) const {
        return true;
    }

    bool operator!=(const NoDefConstr&other) const {
        return false;
    }
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


    std::variant<std::monostate, NoDefConstr, int> v3;
    v3 = 1;

    NoDefConstr a(10);
    NoDefConstr b(20);
    if (a == b)
        std::cout <<"ok";
    if (v2 == v3)
        std::cout << "equal" << std::endl;
}