//
// Created by rick on 2022/1/17.
//

#include "main_window.h"
extern "C" {
  void hello_world();
}

MainWindow::MainWindow(QWidget *parent) : QMainWindow{parent} {
  button = new QPushButton("Click Me", this);
  connect(button, &QPushButton::released,
          this, &MainWindow::onClick);

}


void MainWindow::onClick() {
  hello_world();
}