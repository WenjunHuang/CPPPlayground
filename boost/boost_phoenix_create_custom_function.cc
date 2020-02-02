//
// Created by rick on 2019/12/21.
//
#include <boost/phoenix/phoenix.hpp>
#include <vector>
#include <algorithm>
#include <iostream>

struct IsOddImpl {
    typedef bool result_type;
    template<typename T>
    bool operator()(T t) const {
        return t % 2 ==1;
    }
};

boost::phoenix::function<IsOddImpl> isOdd;

int main() {
    std::vector<int> v{1,2,3,4,5};
    using namespace boost::phoenix::placeholders;
    std::cout << std::count_if(v.begin(),v.end(),isOdd(arg1)) << std::endl;
}