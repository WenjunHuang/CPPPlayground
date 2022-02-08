//
// Created by rick on 2022/2/8.
//
#include <QApplication>
#include <QPointer>
#include "mainwindow.h"

int main(int argc, char *argv[]) {
  QApplication app{argc, argv};
  QPointer<MainWindow> window = new MainWindow;
  window->show();
  return QApplication::exec();
}