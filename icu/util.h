//
// Created by HUANG WEN JUN on 2021/9/18.
//

#ifndef CPPPLAYGROUND_UTIL_H
#define CPPPLAYGROUND_UTIL_H
#include <utility>
// C++ 17
template <auto* P>
struct FunctionWrapper {
  template <typename... Args>
  auto operator()(Args&&... args) const {
    return P(std::forward<Args>(args)...);
  }
};

#endif  // CPPPLAYGROUND_UTIL_H
