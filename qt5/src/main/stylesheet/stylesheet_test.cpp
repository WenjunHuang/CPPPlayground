//
// Created by xxzyjy on 28/10/2017.
//
#include "catch.hpp"
#include "fixture.h"
#include <QPushButton>

TEST_CASE_METHOD(TestFixture,"style_sheet","[QStyleSheet]") {
  application.setStyleSheet("QPushButton{color:white}");
  auto window = new QPushButton("Style Me");
  window->setStyleSheet("* {color:blue}");

  window->show();
  application.exec();
}

