//
// Created by xxzyjy on 05/11/2017.
//

#include <QStackedWidget>
#include "MainWindow.h"

MainWindow::MainWindow(QWidget *parent):QMainWindow(parent) {
  setMinimumSize(QSize(910,610));
  setMaximumSize(QSize(910,610));
  setWindowFlags(Qt::FramelessWindowHint);
  setAttribute(Qt::WA_TranslucentBackground,true);
  setWindowIcon(QIcon(":/image/360logo.ico"));

  _stackwid = new QStackedWidget(this);
}
MainWindow::~MainWindow() {}
