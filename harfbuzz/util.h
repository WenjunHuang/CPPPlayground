//
// Created by rick on 2021/9/22.
//

#pragma once
// C++ 17
template <auto* P>
struct FunctionWrapper {
  template <typename... Args>
  auto operator()(Args&&... args) const {
    return P(std::forward<Args>(args)...);
  }
};

template <int n, auto* P>
struct GLDeleteFunctionWrapper {
  template <typename... Args>
  auto operator()(Args&&... args) const {
    return P(n, std::forward<Args>(args)...);
  }
};