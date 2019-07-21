//
// Created by rick on 19-3-16.
//

#include "DraggableButton.h"

DraggableButton::DraggableButton(QWidget *parent) : QPushButton(parent) {
}

void DraggableButton::dragEnterEvent(QDragEnterEvent *event) {
  event->accept();
//  QWidget::dragEnterEvent(event);
}

void DraggableButton::dragMoveEvent(QDragMoveEvent *event) {
  event->
  QWidget::dragMoveEvent(event);
}
