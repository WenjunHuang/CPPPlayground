//
// Created by rick on 2021/9/16.
//
#include "common.h"

int main() {
  const auto v = std::vector{-8, -4, -2, 1, 2, 4};
  auto rng1 = v | views::group_by([](int x, int y) { return x * y > 0; });
  Print(rng1);

  const auto v2 = std::vector{-8,32,4,1 -4, -2, 1, 2, 4};
  auto rng2 = v2 | views::group_by([](int x, int y) { return x * y > 0; });
  Print(rng2);
}