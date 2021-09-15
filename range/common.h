//
// Created by rick on 2021/9/15.
//

#pragma once
#include <iostream>
#include <range/v3/all.hpp>
#include <vector>
#include <fmt/format.h>
#include <map>
using namespace ranges;

template<typename Itr>void Print(Itr&& itr) {
//  std::cout << fmt::format("{}",fmt::join(itr,",")) << std::endl;
std::cout << itr << std::endl;
}