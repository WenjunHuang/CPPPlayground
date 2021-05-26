//
// Created by rick on 2021/5/25.
//
#include <algorithm>
#include <cassert>
#include <iterator>
#include <stack>
using namespace std;

void foo(stack<int>&& v) { }

int main()
{
    stack<int> vec;
    vec.push(100);
    foo(std::move(vec));
    vec.push(42);

    int i = vec.top();
    assert(i == 42);
}

