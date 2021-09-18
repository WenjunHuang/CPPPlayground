//
// Created by rick on 2021/9/16.
//
#include "common.h"
#include <iostream>

int main(){
  // extract words from the standard input
  auto rng = istream<std::string>(std::cin) |
      views::take(2);
  Print(rng);

  std::cout << std::endl;
  // extract multiple lines from the standard input
  auto rng2 = getlines(std::cin) | views::take(2);
  Print(rng2);

}