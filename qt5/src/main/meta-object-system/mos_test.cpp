//
// Created by xxzyjy on 29/10/2017.
//
#include "catch.hpp"
#include <QtCore>
#include <QtWidgets>
#include "MyWidget.h"
#include "MyWidgetWithQOBJECT.h"
#include "fixture.h"

TEST_CASE_METHOD(TestFixture,"qobject_cast", "[qobject_cast]") {
  MyWidget widget;
  QObject *object = &widget;
  REQUIRE(qobject_cast<QWidget *>(object) != nullptr);
  REQUIRE(qobject_cast<QLabel *>(object) == nullptr);
}

TEST_CASE_METHOD(TestFixture,"classname","[classname]"){
  MyWidget widget;
  QString name = widget.metaObject()->className();
  REQUIRE(name == "MyWidget");
  REQUIRE(std::strcmp(MyWidget::staticMetaObject.className(),"MyWidget")==0);
}

TEST_CASE_METHOD(TestFixture,"classname_of_no_Q_OBJECT","[classname]"){
  MyWidgetWithQOBJECT widget;
  QString name = widget.metaObject()->className();
  REQUIRE(name == "QWidget");

}