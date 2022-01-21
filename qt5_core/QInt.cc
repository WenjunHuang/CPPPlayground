//
// Created by rick on 2021/12/31.
//
#include <QDebug>

int main() {
  qint8 value8 = 0;
  qint16 value16 = 0;
  qint32 value32 = 0;
  qint64 value64 = 0;
  qintptr valueptr = 0;

  qInfo() << "value8: " << sizeof(value8);
  qInfo() << "value16: " << sizeof(value16);
  qInfo() << "value32: " << sizeof(value32);
  qInfo() << "value64: " << sizeof(value64);
  qInfo() << "valueptr: " << sizeof(valueptr);


}
