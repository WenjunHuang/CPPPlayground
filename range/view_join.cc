//
// Created by rick on 2021/9/15.
//
#include <range/v3/view.hpp>
#include <vector>
#include <iostream>

using namespace ranges;

int main(){
  std::vector<std::string> col{"a","b","c"};
  auto result = col | views::join(",") | to<std::string>();
  std::cout << result << std::endl;

}