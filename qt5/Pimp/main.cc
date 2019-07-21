//
// Created by xxzyjy on 2019-03-07.
//

#include <QStringView>
#include <QDebug>
#include "MyFoo.h"

int main(int argc, char *argv[]) {
  QString name{"Wenjun Huang"};
  MyFoo myFoo{QStringView(name)};
  myFoo.addHabit("programming");
  qDebug() << myFoo.personalSign();
}