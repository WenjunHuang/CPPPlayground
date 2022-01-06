//
// Created by HUANG WEN JUN on 2021/12/25.
//

#include <iostream>

namespace A {
    void g(int) {
        std::cout << " calling A::g\n";
    }

    struct X {
    };

    void g(X) {
        std::cout << "calling A::g(X)\n";
    }

    namespace C {
        void g() {
            std::cout << "Calling C::g()\n";
        }

        void j() {
//            g(8); // name hiding
            X x;
            g(x); // koenig lookup kicks in
        }
    }
}

int main() {

}