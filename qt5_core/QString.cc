//
// Created by rick on 2021/12/31.
//
#include <QString>
#include <QDebug>

int main() {
  QString name = "Bryan Cairns";

  qInfo() << name;
  qInfo() << name.mid(1,3);
  qInfo() << name.insert(0,"Mr. ");
  qInfo() << name.split(" ");

  int index = name.indexOf(".");
  qInfo() << name.remove(0,index + 1).trimmed();

  QString title = "teacher";
  QString full = name.trimmed()+ " " + title;
  qInfo() << full;

}

