//
// Created by xxzyjy on 07/11/2017.
//
#include <QSpinBox>
#include "catch.hpp"
#include "CustomStyle.h"
#include "fixture.h"

TEST_CASE("custom_style","[CustomStyle]"){
  int i = 0;
  QApplication application(i,nullptr);
  application.setStyle(new CustomStyle);
  QSpinBox spinBox;
  spinBox.show();
  application.exec();
}

