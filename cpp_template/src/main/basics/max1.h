#pragma once

#include "catch.hpp"

template<typename T1,typename T2>
auto max(T1 a, T2 b)->decltype(true?a:b) {
  return b < a ? a : b;
}

template<typename T>
T foo(T*){}
