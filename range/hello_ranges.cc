//
// Created by rick on 2021/7/30.
//
#include <iostream>
#include <range/v3/all.hpp>
#include <string>
using std::cout;

int main(){
  std::string s{"hello"};
  ranges::for_each(s,[](char c){cout << c << ' ';});
  cout << '\n';

}