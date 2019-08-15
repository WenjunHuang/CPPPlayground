//
// Created by xxzyjy on 25/11/2017.
//

#include <string>
#include <type_traits>
#include "Stack.h"
#include "catch.hpp"

TEST_CASE("stack","[stack]"){
  Stack<int,20u> int20Stack;
  Stack<std::string, 40> stringStack;

  int20Stack.push(7);
  REQUIRE(int20Stack.top() == 7);
  auto size1 = int20Stack.size();
  REQUIRE(size1 == 1);

  stringStack.push("hello");
  REQUIRE(stringStack.top() == "hello");
  auto size2 = stringStack.size();
  REQUIRE(!(std::is_same_v<decltype(size1),decltype(size2)>));
}
