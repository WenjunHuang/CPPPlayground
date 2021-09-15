//
// Created by HUANG WEN JUN on 2021/9/15.
//
#include "common.h"

int main() {
  auto semi_open = views::iota(0,4);
  std::cout << fmt::format("{}",fmt::join(semi_open,",")) << std::endl;

  auto closed_range = views::closed_iota(0,4);
  Print(closed_range);

  auto infinit = views::iota(0); // [0,1,2,....[

  auto linear = views::linear_distribute(1.0,2.0,5);
  Print(linear);
}