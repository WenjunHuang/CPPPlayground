#pragma once

#include <QObject>
#include <QDebug>
#include "wobjectdefs.h"

class MyObject : public QObject {
  W_OBJECT(MyObject)
public:
  void mySlot(const QString &name) {
    qDebug() << "hello " << name;
  }
  W_SLOT(mySlot)

public:
  void mySignal(const QString &name)
  W_SIGNAL(mySignal,name)
};



