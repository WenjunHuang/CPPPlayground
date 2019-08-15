//
// Created by xxzyjy on 18/12/2017.
//
#include "catch.hpp"
#include "fixture.h"
#include "CalendarWindow.h"

TEST_CASE_METHOD(TestFixture,"calendar_widget","[calendar]"){
  CalendarWindow main;
  main.show();
  application.exec();
}
