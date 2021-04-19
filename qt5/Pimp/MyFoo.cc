//
// Created by xxzyjy on 2019-03-07.
//

#include "MyFoo.h"
#include "MyFoo_p.h"

MyFoo::MyFoo(QStringView name) : d_ptr{new MyFooPrivate(this)} {
  d_ptr->name = name.toString();
}

void MyFoo::setName(QStringView name) {
  Q_D(MyFoo);
  d->name = name.toString();
}

QString MyFoo::getName() const {
  auto d = d_func();
  return d->name;
}

QString MyFoo::personalSign() const {
  auto d = d_func();
  return QString("name: %1, habbit count: %2")
    .arg(d->name)
    .arg(d->habbits.size());
}

MyFoo::~MyFoo() {

}

void MyFoo::addHabit(QStringView habbit) {
  Q_D(MyFoo);
  d->habbits.push_back(habbit.toString());
}
MyFoo::MyFoo(MyFooPrivate& d):QObject(d) {}
