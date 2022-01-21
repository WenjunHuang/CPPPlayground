//
// Created by rick on 2022/1/4.
//
#include <QObject>
#include <QDebug>
#include <QMetaClassInfo>
#include <QMetaObject>

class Test : public QObject {
Q_OBJECT
    Q_CLASSINFO("Author", "Wenjun")
public:
    explicit Test(QObject *parent = nullptr) : QObject{parent} {}
};

void listClassInfo(QObject *obj){
  qInfo() << obj->metaObject()->className();
  for (int i = 0;i < obj->metaObject()->classInfoCount();++i){
    auto c = obj->metaObject()->classInfo(i);
    qInfo() << "Property: " << c.name() << " = " << c.value();
  }
}
int main() {

  Test test{};
  listClassInfo(&test);
}

#include "ClassInfo.moc"
