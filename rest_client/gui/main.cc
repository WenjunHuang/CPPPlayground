//
// Created by rick on 2022/1/17.
//
#include <QtWidgets/QtWidgets>
#include "main_window.h"

int main(int argc,char *argv[]){
  QApplication app(argc,argv);
  MainWindow mainWindow;
  mainWindow.show();

  return app.exec();

}