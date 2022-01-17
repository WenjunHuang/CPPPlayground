//
// Created by rick on 2022/1/6.
//
#include <QObject>
#include <QDebug>
#include <type_traits>

class Counter : public QObject {
Q_OBJECT
    Q_CLASSINFO("Size", "Big")
    Q_CLASSINFO("Color", "Green")
public:
    explicit Counter(QObject *parent = nullptr) : QObject{parent} {}
};

struct FooAtomicInt {
    explicit FooAtomicInt(int v) : value{v} {}

    int value;
};

class FooCounter {
public:
    FooAtomicInt ref;
};

QByteArrayData data = {{-1}, 0, 0};

int main() {
  Counter counter;
  qInfo() << "Is QMetaObject POD? " << std::is_pod_v<QMetaObject>;
  FooCounter r{{-1}};

}

#include "MOC.moc"