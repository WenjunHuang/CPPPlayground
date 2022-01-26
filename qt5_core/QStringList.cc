//
// Created by rick on 2021/12/31.
//
#include <QDebug>
#include <QStringList>

int main() {
  QString data = "Hello World how are you?";
  QStringList lst = data.split(" ");

  qInfo() << lst;

  foreach(QString str,lst) {
    qInfo() << str;
  }
  lst.sort();
  qInfo() << lst;

}

