//
// Created by xxzyjy on 02/11/2017.
//

#include "EventChild.h"
#include <QDebug>
#include <QEvent>

bool EventChild::event(QEvent *event) {
  if (event->type() == (QEvent::Type) 1234) {
    qDebug() << "received event:" << event->type() << " but not handle by Child";
    event->ignore();
    return false;
  }
  return QObject::event(event);
}
