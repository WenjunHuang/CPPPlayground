#include "catch.hpp"
#include <QtWidgets/QApplication>
#include "CustomChildrenWindow.h"

//
// Created by rick on 2017/7/29.
//
TEST_CASE("custom_children", "[QTWidgets]") {

  char program[] = "test";
  char* argv[] ={program};
  int argc = 1;

  QApplication a(argc,argv);
  CustomChildrenWindow w;
  w.show();

  a.exec();
}

