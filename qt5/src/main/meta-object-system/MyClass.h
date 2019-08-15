#pragma once
#include <QtCore>

class MyClass :public QObject{
  Q_OBJECT
  Q_PROPERTY(Priority priority READ priority WRITE setPriority NOTIFY priorityChanged)
public:
  MyClass(QObject *parent = nullptr):QObject(parent){}
  ~MyClass(){}

  enum Priority{High,Low,VeryHigh,VeryLow};
  Q_ENUM(Priority)

  void setPriority(Priority priority){
    _priority = priority;
    emit priorityChanged(priority);
  }

  Priority priority() const{return _priority;}

  signals:
  void priorityChanged(Priority priority);
private:
  Priority _priority;

};



