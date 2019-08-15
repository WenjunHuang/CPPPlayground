//
// Created by xxzyjy on 21/11/2017.
//

#include "MyObject.h"
#include "wobjectimpl.h"
#include "catch.hpp"


W_OBJECT_IMPL(MyObject)

TEST_CASE("verdigris","[verdigris]"){
  auto myObj1 = new MyObject;
  bool ok = true;
  ok = ok && QObject::connect(myObj1,&MyObject::mySignal,myObj1,&MyObject::mySlot);
  REQUIRE(ok == true);
}