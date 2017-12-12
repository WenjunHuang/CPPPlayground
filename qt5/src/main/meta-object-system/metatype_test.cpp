//
// Created by xxzyjy on 09/12/2017.
//
#include <QObject>
#include <QDebug>
#include "catch.hpp"

class MyClass {
};

Q_DECLARE_METATYPE(MyClass);

TEST_CASE("meta_type", "[meta_type]") {
  int registerId = qRegisterMetaType<MyClass>();
  int id = QMetaType::type("MyClass");

  REQUIRE(id != QMetaType::UnknownType);
  REQUIRE(id == registerId);

  auto myClassPtr = QMetaType::create(id);
  REQUIRE(myClassPtr != nullptr);

  QMetaType::destroy(id, myClassPtr);
}

TEST_CASE("destroyed_event", "[meta_type]") {
  QObject *parent = new QObject;
  QObject *child = new QObject(parent);

  qDebug() << child;

  QObject::connect(child, &QObject::destroyed, [=](QObject *toBeDestroyed) {
    qDebug() << toBeDestroyed;
    REQUIRE(toBeDestroyed == child);
  });

  delete child;
  delete parent;
}
