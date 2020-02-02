//
// Created by rick on 2019/12/21.
//
#include <algorithm>
#include <boost/phoenix/phoenix.hpp>
#include <iostream>
#include <vector>
using namespace boost::phoenix;
using namespace boost::phoenix::placeholders;
bool isOdd(int i) { return i % 2 == 1; }
int main() {
    std::vector<int> v{1, 2, 3, 4, 5};

    auto phoenix = arg1 > val(2) && arg1 % val(2) == val(1);

    std::cout << std::count_if(v.begin(), v.end(), phoenix) << '\n';
}
