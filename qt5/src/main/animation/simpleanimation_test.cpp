//
// Created by xxzyjy on 29/10/2017.
//
#include "catch.hpp"
#include "fixture.h"
#include <QtWidgets>

TEST_CASE_METHOD(TestFixture,"simple_animation","[Animation]") {
  QPushButton button("Animated Button");
  QObject *obj = &button;
  QWidget* widget = qobject_cast<QWidget *>(obj);

  REQUIRE(widget != nullptr);
  auto label = qobject_cast<QLabel *>(obj);
  REQUIRE(label == nullptr);

  button.show();

  QPropertyAnimation animation(&button,"geometry");
  animation.setDuration(2000);
  animation.setKeyValueAt(0,QRect(0,0,100,50));
  animation.setKeyValueAt(0.8,QRect(250,250,100,50));
  animation.setKeyValueAt(1,QRect(0,0,100,50));
  animation.start();

  application.exec();
}

