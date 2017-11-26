//
// Created by xxzyjy on 01/10/2017.
//
#include "basics/decltype.h"
#include "catch.hpp"
#include <deque>

TEST_CASE("decltype","[decltype]"){
  std::vector<int> d(10);
  authAndAccess(d, 5) = 10;
}

