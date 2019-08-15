#pragma once


#include <QObject>

class EventParent: public QObject {
  Q_OBJECT
public:
  EventParent():QObject(nullptr){}

  bool event(QEvent *event) override;
};



