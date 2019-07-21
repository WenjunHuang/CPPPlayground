#pragma once

#include <string>
#include <vector>

class MyFoo;

struct MyFooPrivate {
  QString name;
  int age;
  std::vector<QString> habbits;

  MyFoo *foo;

  MyFooPrivate(MyFoo *p) : foo{p} {}
};