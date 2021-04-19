#pragma once

#include <string>
#include <vector>
#include <QtCore/private/qobject_p.h>
class MyFoo;

struct MyFooPrivate: QObjectPrivate {
  QString name;
  int age;
  std::vector<QString> habbits;

  MyFoo *foo;

  MyFooPrivate(MyFoo *p) : foo{p} {}
};