//
// Created by xxzyjy on 02/11/2017.
//

#include "MouseEvent.h"
#include <QStatusBar>
#include <QMouseEvent>

MouseEvent::MouseEvent(QWidget *parent) : QMainWindow(parent) {
//  setWindowTitle(tr("鼠标事件"));

  setWindowFlags(Qt::FramelessWindowHint);
  statusLabel = new QLabel;
  statusLabel->setText(tr("当前位置:"));
  statusLabel->setFixedWidth(100);
  mousePosLabel = new QLabel;
  mousePosLabel->setText(tr(""));
  mousePosLabel->setFixedWidth(100);

  statusBar()->addPermanentWidget(statusLabel);
  statusBar()->addPermanentWidget(mousePosLabel);

  setMouseTracking(true);
  resize(400, 200);

}

void MouseEvent::mousePressEvent(QMouseEvent *event) {
  QString str = "(" + QString::number(event->x()) + "," + QString::number(event->y()) + ")";
  if (event->button() == Qt::LeftButton) {
    statusBar()->showMessage(tr("左键:") + str);
  } else if (event->button() == Qt::RightButton) {
    statusBar()->showMessage(tr("右键:") + str);
  } else if (event->button() == Qt::MidButton) {
    statusBar()->showMessage(tr("中键:") + str);
  }
}

void MouseEvent::mouseMoveEvent(QMouseEvent *event) {
  mousePosLabel->setText("(" + QString::number(event->x()) + "," + QString::number(event->y()) + ")");
}

void MouseEvent::mouseReleaseEvent(QMouseEvent *event) {
  QString str = "(" + QString::number(event->x()) + "," + QString::number(event->y()) + ")";
  statusBar()->showMessage(tr("释放在:")+str,3000);
}

void MouseEvent::mouseDoubleClickEvent(QMouseEvent *event) {
  QWidget::mouseDoubleClickEvent(event);
}

MouseEvent::~MouseEvent() = default;
