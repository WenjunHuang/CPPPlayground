//
// Created by xxzyjy on 02/11/2017.
//

#include "EventParent.h"
#include <QEvent>
#include <QDebug>

bool EventParent::event(QEvent *event) {
  if (event->type() == (QEvent::Type) 1234) {
    qDebug() << "event:" << event->type() << " handle by EventParent";
    event->accept();
    return true;
  }
  return QObject::event(event);
}
