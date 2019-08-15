//
// Created by xxzyjy on 19/11/2017.
//

#include <QPainter>
#include "WordLineButton.h"

WordLineButton::WordLineButton(QWidget *parent) : QPushButton(parent) {
  setCursor(Qt::PointingHandCursor);
  _enter = false;
}

void WordLineButton::paintEvent(QPaintEvent *) {
  QPainter painter(this);
  QFont font;
  QFontMetrics metrics(font);

  if (_enter)
    font.setUnderline(true);

  auto str = text();
  painter.setPen(Qt::white);
  painter.setFont(font);
  painter.drawText((width() - metrics.width(str)) / 2,
                   height() / 2,
                   str);
}

void WordLineButton::enterEvent(QEvent *) {
  _enter = true;
  update();
}

void WordLineButton::leaveEvent(QEvent *) {
  _enter = false;
  update();
}
