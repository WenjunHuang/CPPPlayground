//
// Created by xxzyjy on 2019-02-14.
//

#include <QtWidgets>
#include "RobotPart.h"


RobotPart::RobotPart(QGraphicsItem *parent) : QGraphicsObject(parent), color(Qt::lightGray), dragOver(false) {
  setAcceptDrops(true);

}

void RobotPart::dragEnterEvent(QGraphicsSceneDragDropEvent *event) {
  if (event->mimeData()->hasColor()) {
    event->setAccepted(true);
    dragOver = true;
    update();
  } else {
    event->setAccepted(false);
  }
}

void RobotPart::dragLeaveEvent(QGraphicsSceneDragDropEvent *event) {
  Q_UNUSED(event);
  dragOver = false;
  update();
}

void RobotPart::dropEvent(QGraphicsSceneDragDropEvent *event) {
  dragOver = false;
  if (event->mimeData()->hasColor())
    color = qvariant_cast<QColor>(event->mimeData()->colorData());
  update();
}

RobotHead::RobotHead(QGraphicsItem *parent) : RobotPart(parent) {

}

QRectF RobotHead::boundingRect() const {
  return QRectF(-15, -50, 30, 50);
}

void RobotHead::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {
  Q_UNUSED(option);
  Q_UNUSED(widget);
  if (pixmap.isNull()) {
    painter->setBrush(dragOver ? color.light(130) : color);
    painter->drawRoundedRect(-10,-30,20,30,25,25,Qt::RelativeSize);
  }
}
