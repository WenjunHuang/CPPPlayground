//
// Created by rick on 2022/1/10.
//
#include <algorithm>
#include <array>
#include <fmt/printf.h>

int main() {
  std::array<int, 10> ar{1, 2, 3, 4, 5, 6, 7, 8, 9};
  int valueToBeFound = 2;
  auto itr = std::find_if_not(std::begin(ar),
                              std::end(ar),
                              [=](auto &v) {
                                  if (v == valueToBeFound) return false;
                                  else return true;
                              });
  if (itr == std::end(ar))
    fmt::print("can not find value : {}", valueToBeFound);
  else
    fmt::print("vale {} found!", *itr);

}

