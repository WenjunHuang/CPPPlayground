//
// Created by xxzyjy on 26/10/2017.
//
#include "catch.hpp"
#include "SimpleDialog.h"
#include <QtWidgets>

TEST_CASE("simple_dialog","[simple dialog]"){
  int argc = 1;
  char *argv[] = {"simple_dialog"};
  QApplication app(argc,argv);

  auto window = new SimpleDialog;
  window->show();
  app.exec();
}

