//
// Created by xxzyjy on 18/12/2017.
//
#include "catch.hpp"
#include "fixture.h"
#include "ChipMainWindow.h"

TEST_CASE_METHOD(TestFixture,"chip","[chip]"){
  ChipMainWindow main;
  main.show();
  main.dumpObjectTree();
  application.exec();
}
