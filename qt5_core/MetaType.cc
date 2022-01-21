//
// Created by rick on 2022/1/6.

#include <QDebug>
#include <QMetaType>
#include <QString>

struct Foo{

};

Q_DECLARE_METATYPE(Foo)

int main() {
  auto typeIdOfQString = qMetaTypeId<QString>();
  qInfo() << typeIdOfQString;

  auto typeIdOfListOfString = QMetaType::type(
    ("QList<" + QString{QMetaType::typeName(typeIdOfQString)} + ">").toLatin1());
  qInfo() << typeIdOfListOfString;

  auto typeId = qMetaTypeId<Foo>();
  qInfo() << typeId;

}