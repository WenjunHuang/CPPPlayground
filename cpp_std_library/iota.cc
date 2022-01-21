//
// Created by rick on 2022/1/4.
//
#include <algorithm>
#include <numeric>
#include <fmt/printf.h>
#include <string>
#include <vector>

int main() {
  std::vector<std::string> v{4};
  std::fill(v.begin(), v.end(), "hello");
  fmt::print("{}\n", fmt::join(v, ","));

  std::iota(v.begin(), v.end(), "hello");
  fmt::print("{}\n", fmt::join(v, ","));

  std::generate(v.begin(), v.end(), [i = 0]() mutable {
      return ++i % 2 ? "hello" : "world";
  });
  fmt::print("{}\n", fmt::join(v, ","));
}

