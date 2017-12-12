//
// Created by xxzyjy on 02/12/2017.
//
#include "catch.hpp"

template<typename T,std::size_t L1,std::size_t L2>
void foo(T (&arg1)[L1],T (&arg2)[L2]) {
  T* pa = arg1;
  T* pb = arg2;
};
