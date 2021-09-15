//
// Created by rick on 2021/9/14.
//
#include <fmt/ranges.h>
#include <range/v3/view.hpp>
#include <vector>

using namespace ranges;
int main() {
  auto const v1 = std::vector{0, 2, 1};
  auto const v2 = std::vector{1, 5, 7};
  auto rng = views::zip_with(std::plus{}, v1, v2);
  auto rng2 = views::zip_with([](int x, int y) { return x * y; }, v1, v2);

  auto rng3 = views::closed_iota(1, 5);
  auto v3 = rng2 | to<std::vector<int>>();
  fmt::print("{}", fmt::join(v3, ","));
}