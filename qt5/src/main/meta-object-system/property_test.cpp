//
// Created by xxzyjy on 29/10/2017.
//
#include "catch.hpp"
#include "MyClass.h"

TEST_CASE("property","[property]"){
  MyClass myinstance;
  QObject *object = &myinstance;

  myinstance.setPriority(MyClass::VeryHigh);
  REQUIRE(myinstance.priority() == MyClass::VeryHigh);

  object->setProperty("priority","High");
  REQUIRE(myinstance.priority()==MyClass::High);
}
