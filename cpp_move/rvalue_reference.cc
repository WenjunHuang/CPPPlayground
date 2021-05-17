//
// Created by rick on 2021/5/7.
//
#include <utility>
#include <string>
using namespace std;

wstring foo(){
    return L"Hello";
}
int main() {
    wstring str = L"hello world";
    wstring&& r1 = std::move(str);
    wstring&& r2 = foo();

//    wstring&& r3 = str; // rvalue reference can not point to a lvalue
}