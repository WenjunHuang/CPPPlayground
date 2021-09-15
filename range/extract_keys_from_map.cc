//
// Created by rick on 2021/9/15.
//
#include <map>
#include <range/v3/all.hpp>
#include <string>
#include <fmt/format.h>
#include <iostream>

using namespace ranges;

int main() {
  const auto m = std::map<std::string, std::string>{{"red", "#ff0000"},
                                                    {"green", "#00ff00"}};
  auto rng = m | views::keys;
  std::cout << fmt::format("{}",fmt::join(rng,",")) << std::endl;

  auto values = m | views::values;
  std::cout << fmt::format("{}",fmt::join(values,",")) << std::endl;

}