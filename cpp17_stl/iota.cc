//
// Created by HUANG WEN JUN on 2021/12/29.
//
#include <vector>
#include <numeric>
#include <fmt/printf.h>
int main(){
    std::vector<int> v(10);
    std::iota(v.begin(),v.end(),1);
    fmt::print("{}",fmt::join(v,", "));

}

