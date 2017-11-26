//
// Created by xxzyjy on 12/10/2017.
//
#include "catch.hpp"

#include <cstring>
#include <string>

template<typename T>
T max(T a,T b) {
  return b< a?a:b;
}

template<typename T>
T* max(T *a, T* b){
  return *b < *a ?a:b;
}

char const * max(char const *a, char const* b){
  return std::strcmp(b,a) < 0?a:b;
}

TEST_CASE("resolution","[resolution]"){
  int a = 7;
  int b = 42;
  auto m1 = ::max(a,b);

  std::string s1 = "hey";
  std::string s2 = "you";
  auto m2 = ::max(s1,s2);

  int* p1 = &b;
  int* p2 = &a;
  auto m3 = ::max(p1,p2);

  char const* x = "hello";
  char const* y = "world";
  auto m4 = ::max(x,y);
}
