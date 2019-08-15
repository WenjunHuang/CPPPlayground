//
// Created by xxzyjy on 18/12/2017.
//
#include "catch.hpp"
#include "fixture.h"
#include "ApplicationMainWindow.h"

TEST_CASE_METHOD(TestFixture,"ApplicationMainWindow","[Application]") {
  ApplicationMainWindow main;
  main.show();
  application.exec();
}

