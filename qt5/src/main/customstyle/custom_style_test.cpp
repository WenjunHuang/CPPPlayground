//
// Created by xxzyjy on 07/11/2017.
//
#include <QSpinBox>
#include "catch.hpp"
#include "CustomStyle.h"
#include "fixture.h"

TEST_CASE_METHOD(TestFixture,"custom_style","[CustomStyle]"){
  application.setStyle(new CustomStyle);
  QSpinBox spinBox;
  spinBox.show();
  application.exec();
}

