//
// Created by rick on 2021/9/15.
//
#include <iostream>
#include <range/v3/all.hpp>
#include <vector>
#include <fmt/format.h>
#include <map>
using namespace ranges;
int main() {
  auto const m = std::vector{4, 7, 8};
  auto rng = m | views::addressof |views::transform([](auto p){return (void*)(p);});// | views::transform([](auto pointer){return fmt::format("{0:#x}",reinterpret_cast<unsigned int>(pointer));});
  std::cout << fmt::format("{}", fmt::join(rng, ",")) << std::endl;
}