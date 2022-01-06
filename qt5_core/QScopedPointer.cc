//
// Created by rick on 2021/12/31.
//
#include <QObject>
#include <QDebug>
#include <QScopedPointer>

class Test : public QObject {
Q_OBJECT
public:
    explicit Test(QObject *parent = nullptr) : QObject(parent) {
      qInfo() << "Constructed!";
    }

    ~Test() override {
      qInfo() << "Destructed!";
    }
};

void useIt(Test *obj) {
  if (!obj) return;
  qInfo() << "Using" << obj;
}

void doStuff() {
  QScopedPointer<Test> myPointer(new Test());
  useIt(myPointer.data());
}

int main() {
  doStuff();

}

#include "QScopedPointer.moc"