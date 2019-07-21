//
// Created by rick on 2017/7/29.
//

#include <gtest/gtest.h>
#include <QtWidgets/QApplication>
#include "mainwindow.h"

char program[] = "test";
char* argv[] ={program};
int argc = 1;

TEST(QTWidgets, LoginWindow) {

  QApplication a(argc,argv);
  MainWindow w;
  w.show();

  a.exec();
}

