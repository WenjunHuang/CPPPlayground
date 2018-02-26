//
// Created by xxzyjy on 20/12/2017.
//

#include <QLineEdit>
#include "TestGui.h"
#include "catch.hpp"
#include "fixture.h"

TEST_CASE_METHOD(TestFixture,"gui_test","[gui_test]") {
  QLineEdit lineEdit;
  QTest::keyClicks(&lineEdit,"hello world");
  REQUIRE(lineEdit.text() == QString("hello world1"));
}

void TestGui::testGui() {
  QLineEdit lineEdit;
  QTest::keyClicks(&lineEdit,"hello world");
  QCOMPARE(lineEdit.text(),QString("hello world"));
}
