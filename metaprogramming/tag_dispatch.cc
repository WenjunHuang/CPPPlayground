//
// Created by rick on 2021/4/26.
//
#include <vector>

struct MyTag{};

class MyClass {
public:
    static struct ConstructThisWay{} constructThisWay;
    static struct ConstructThatWay{} constructThatWay;

    explicit MyClass(ConstructThisWay);
    explicit MyClass(ConstructThatWay);
};