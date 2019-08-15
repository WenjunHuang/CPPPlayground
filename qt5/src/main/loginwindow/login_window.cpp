//
// Created by rick on 2017/7/29.
//

#include "catch.hpp"
#include <QtWidgets/QApplication>
#include "LoginWindow.h"


TEST_CASE("loginwindow", "[QTWidgets]") {

  char program[] = "test";
  char* argv[] ={program};
  int argc = 1;
  QApplication a(argc,argv);
  LoginWindow w;
  w.show();

  a.exec();
}

