//
// Created by rick on 19-7-11.
//

#include "HttpClientWindow.h"
#include <QtWidgets/QApplication>
int main(int argc,char *argv[]){
  QApplication application(argc,argv);

  HttpClientWindow window;
  window.show();
  return application.exec();
}

