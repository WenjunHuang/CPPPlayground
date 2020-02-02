//
// Created by rick on 2020/1/19.
//
#include <algorithm>
#include <iostream>
#include <numeric>
#include <vector>
using namespace std;

auto minValue(vector<int>& c) {
    sort(begin(c), end(c));
    return transform_reduce(
        cbegin(c), --cend(c), ++cbegin(c), numeric_limits<int>::max(),
        [](auto a, auto b) { return min(a, b); },
        [](auto a, auto b) { return abs(a - b); });
}
int main() {}