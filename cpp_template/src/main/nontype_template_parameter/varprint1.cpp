//
// Created by xxzyjy on 25/11/2017.
//
#include "catch.hpp"
#include <iostream>

void print(){}

template<typename T, typename... Types>
void print(T firstArg,Types... args){
  std::cout << sizeof...(Types) << '\n';
  std::cout << sizeof...(args) << '\n';
//  std::cout << firstArg << '\n';
  print(args...);
};

TEST_CASE("varprint","[varprint]"){
  std::string s("world");
  print(7.5,"hello",s);
}

template<typename... T>
constexpr auto foldLeft(T... s){
  return (... + s);
}

template<typename... T>
constexpr auto foldRight(T... s){
  return (s + ...);
}

template<typename T, typename... Ts>
constexpr auto initFoldLeft(T init,Ts... s){
  return (init + ... + s);
};

TEST_CASE("fold_expr","[fold_expr]"){
  auto result = foldLeft(1, 2, 3, 4, 5);
  REQUIRE(result == 15);

  auto result2 = foldRight(1,2,3,4,5);
  REQUIRE(result == 15);
}
