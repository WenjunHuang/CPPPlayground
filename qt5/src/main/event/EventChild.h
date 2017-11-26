#pragma once


#include <QObject>

class EventChild : public QObject{
  Q_OBJECT
public:
  EventChild(QObject *parent = nullptr):QObject(parent){}

  bool event(QEvent *event) override;
};



