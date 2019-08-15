//
// Created by xxzyjy on 01/10/2017.
//
#include "basics/deduced_type_helper.h"
#include "basics/decltype.h"
#include "catch.hpp"
#include <deque>

TEST_CASE("use compiler to report deduced type","[decltype]"){
  auto i = 10;

//  TD<decltype(authAndAccess(std::deque<int>(),10))> iType;
}

