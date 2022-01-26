//
// Created by HUANG WEN JUN on 2022/1/2.
//

struct A {
    explicit A(int i = 0) {}
};

int main() {
//    A a[2](A(1));
    A b[2]{A(1)};

}

