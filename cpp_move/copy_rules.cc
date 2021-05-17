//
// Created by rick on 2021/5/8.
//
#include <utility>
#include <string>
class Foo {
public:
    Foo(const std::string& n):name{n}{}
    Foo(Foo&&) = default;

private:
    std::string name;
};

int main(){
   Foo foo{"abc"};
   Foo foo2{"def"};

   foo = std::move(foo2);
}