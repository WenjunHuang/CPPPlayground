//
// Created by xxzyjy on 12/10/2017.
//
#include "catch.hpp"
#include <string>
#include <complex>
#include <iostream>

int max(int a,int b){
  std::cout << "nontemplate"<<std::endl;
  return b < a ? a:b;
}

template<typename T>
std::decay_t<T> max(T a,T b){
  std::cout << "template"<<std::endl;
  return b < a?a:b;
}

TEST_CASE("test template resolution","[template resolution]"){
  ::max(7,42);
  ::max(7.0,42.0);
  ::max('a','b');
  ::max<>(7,42);
  ::max<double>(7,42);
  ::max('a',42.7);
}

