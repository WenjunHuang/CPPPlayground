//
// Created by rick on 2021/9/16.
//
#include "common.h"
#include <sstream>
int main() {
  const auto s =  std::string{"8 9 2 6"};
  std::istringstream iss{s};
  auto rng = istream<int>(iss) | views::take(2);
  Print(rng);
}
