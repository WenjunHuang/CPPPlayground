//
// Created by xxzyjy on 01/10/2017.
//
#include "basics/decltype.h"
#include "catch.hpp"
#include <deque>

TEST_CASE("decltype","[decltype]"){
  std::vector<int> d(10);
}

class MyClass {
public:
  void myFunc();
};

#define METHOD(Class,Func) \
  void Class::my##Func() { }

#define Ide(name) i##name##Hello

METHOD(MyClass,Func)

TEST_CASE("macro","[macro]"){
  auto Ide(tt) = 1;

}

