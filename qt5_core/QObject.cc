//
// Created by rick on 2021/12/31.
//
#include <QDebug>

class Cat : public QObject{
    Q_OBJECT
};

void test(QObject* obj){
  qInfo() << obj;
}

int main(){
  Cat kitty;
  kitty.setObjectName("Fully!");
  test(&kitty);
}

#include "simple.moc"
