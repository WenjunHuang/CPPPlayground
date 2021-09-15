//
// Created by rick on 2021/9/15.
//
#include <range/v3/all.hpp>
#include <iostream>
#include <string>
#include <vector>
#include <fmt/format.h>
using namespace ranges;

struct Planet {
  std::string name;
  double gravity;
};

int main(){
  auto const v = std::vector<Planet>{{"Venus",8.87},{"Mars",3.711}};
  auto rng = v | views::transform(&Planet::name);
  std::cout << fmt::format("{}",fmt::join(rng,","));

}