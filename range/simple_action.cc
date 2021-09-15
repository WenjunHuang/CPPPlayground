//
// Created by rick on 2021/9/15.
//
#include <iostream>
#include <range/v3/all.hpp>
#include <vector>
#include <fmt/format.h>
using namespace ranges;
int main(){
  std::vector<int> v{1,3,6};
  auto val = accumulate(v,0);
  v |= actions::transform([](int x){return 2*x;});
  auto rng = v | views::transform([](int x){return 2*x;});

  std::cout << fmt::format("{}",fmt::join(v,",")) << std::endl;
  std::cout << fmt::format("{}",fmt::join(rng,",")) << std::endl;

  auto r_int = views::iota(1) | views::stride(2) | views::take(3);
  std::cout << fmt::format("{}",fmt::join(r_int,",")) << std::endl;

  const auto v2 = std::vector{7,9,-2,5};
  auto r_filt =  v2 | views::remove_if([](int x){return x , 0;}); // a view
  auto v3 = r_filt|to<std::vector<int>>();// materialization
}