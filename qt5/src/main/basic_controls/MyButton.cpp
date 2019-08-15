//
// Created by xxzyjy on 30/11/2017.
//

#include <QStyleOptionButton>
#include <QPainter>
#include <QStylePainter>
#include "MyButton.h"

MyButton::MyButton(QWidget *parent) : QPushButton(parent) {

}

void MyButton::paintEvent(QPaintEvent *) {
  QStyleOptionButton option;
  initStyleOption(&option);

  QStylePainter painter(this);
  painter.drawControl(QStyle::CE_PushButton,option);
}
