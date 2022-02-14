//
// Created by rick on 2022/1/26.
//
#include <QObject>
#include <QDebug>

class Parent : public QObject {
Q_OBJECT
public:
    Parent() {
      qDebug() << "Parent constructor";
    }

    ~Parent() override {
      qDebug() << "Parent destructor";
    }
};

class Child : public QObject {
Q_OBJECT
public:
    Child() {
      qDebug() << "Child constructor";
    }

    ~Child() override {
      qDebug() << "Child destructor";
    }
};

int main(int argc, char *argv[]) {
  Child child{};
  Parent parent{};
  child.setParent(&parent);

}

#include "objects_hierachy_on_stack.moc"