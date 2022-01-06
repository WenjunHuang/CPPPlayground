#include <qobject.h>

//
// Created by rick on 2021/12/31.
//
#include <QCoreApplication>
#include <QDebug>
#include <QObject>

class test : public QObject {
Q_OBJECT
public:
    explicit test(QObject *parent = nullptr);

    ~test();

    void makeChild(QString name);
};

test::test(QObject *parent) : QObject{parent} {
  qInfo() << this << "Constructed";
}

test::~test() {
  qInfo() << this << "Deconstructed";
}

void test::makeChild(QString name) {
  test *child = new test(this);
  child->setObjectName(name);
}

int main(int argc, char *argv[]) {
  test *p = new test();
  p->setObjectName("parent");
  for (int i = 0; i < 5; ++i) {
    p->makeChild(QString::number(i));
  }
  delete p;

}

#include "parent_children.moc"