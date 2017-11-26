//
// Created by xxzyjy on 26/10/2017.
//

#include "catch.hpp"
#include <QString>
#include <QDebug>

using namespace Catch::Matchers;

TEST_CASE("string_ops", "[QtCore]") {
  {
    QString str1(u8"Welcome ");
    str1 = str1 + u8"to you!";
    QString str2 = u8"你好, ";
    str2 += u8"World!";

    REQUIRE(str1 == "Welcome to you!");
    REQUIRE(str2 == "你好, World!");
  }
  {
    QString str("Welcome");
    str.append(" to you!");
    REQUIRE(str == "Welcome to you!");
  }
  {
    QString str;
    str.sprintf("%s %s", "Welcome", "to you!");
    REQUIRE(str == "Welcome to you!");
  }
  {
    QString str = QString("%1 was born in %2.")
      .arg("John")
      .arg(1982);
    REQUIRE(str == "John was born in 1982.");
  }
  {
    QString str = "   Welcome to you!    ";
    REQUIRE(str.trimmed() == "Welcome to you!");
  }
}

TEST_CASE("string to others","[QString]") {
  {
    QString str = "125";
    bool ok;
    int hex = str.toInt(&ok, 16);
    REQUIRE(hex == 293);
    REQUIRE(str.toInt(&ok,10) == 125);
  }
  {
    QString str = "你好，黄文俊";
    auto ba = str.toUtf8();
    qDebug() << ba;
  }
}

