#pragma once

#include <QApplication>

class TestFixture {
public:
  int argc = 1;
  const char *argv[1] = {"splitter_test"};
  QApplication application;

  TestFixture():application(argc,const_cast<char**>(argv)){}
};