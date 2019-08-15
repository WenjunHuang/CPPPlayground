//
// Created by xxzyjy on 11/12/2017.
//
#include "catch.hpp"
#include "fixture.h"
#include "AddressBookMainWindow.h"
#include "AddDialog.h"

TEST_CASE_METHOD(TestFixture,"main_window","[addressbook]"){
  AddressBookMainWindow main;
  main.show();
  application.exec();
}


TEST_CASE_METHOD(TestFixture,"add_dialog","[addressbook]"){
  AddDialog main;
  main.show();
  application.exec();
}


