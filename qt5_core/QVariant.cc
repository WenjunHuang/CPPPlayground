//
// Created by rick on 2021/12/31.
//
#include <QDebug>
#include <QVariant>

void test(QVariant value) {
  qInfo() << value;
  int test = 0;
  bool ok = false;

  test = value.toInt(&ok);
  if (ok) {
    qInfo() << "INT = " << test;
  } else {
    qInfo() << "Not a number";
  }

}

int main() {
  QVariant value = 1;
  QVariant value2 = "Hello World";

  qInfo() << sizeof(value);
  qInfo() << sizeof(value2);
  test(value);
  test(value2);

}