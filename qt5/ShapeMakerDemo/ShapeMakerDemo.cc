//
// Created by rick on 2022/2/7.
//
#include <QtWidgets/QtWidgets>
#include "mainwindow.h"

int main(int argc, char *argv[]) {
  QApplication app{argc, argv};
  auto window = new MainWindow{};
  window->show();

  return QApplication::exec();
}
