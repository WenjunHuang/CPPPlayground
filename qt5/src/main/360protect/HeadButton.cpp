//
// Created by xxzyjy on 10/11/2017.
//

#include <QPainter>
#include "HeadButton.h"

HeadButton::HeadButton(QWidget *parent) : QPushButton(parent) {
  setCursor(Qt::PointingHandCursor);
  _index = 0;

  QPixmap border(":/image/head_bkg.png");
  _pixmaps << border.copy(0, 0, border.width() / 2, border.height());
  _pixmaps << border.copy(border.width() / 2, 0, border.width() / 2, border.height());


  connect(this, SIGNAL(indexChanged()), this, SLOT(update()));

}

void HeadButton::paintEvent(QPaintEvent *) {
  QPainter painter(this);
  painter.drawPixmap(rect(), _pixmaps.at(_index));

  QPixmap pixhead(":/image/head.png");
  painter.drawPixmap((width() - pixhead.width()) / 2,
                     (height() - pixhead.height()) / 2,
                     pixhead.width(),
                     pixhead.height(),
                     pixhead);
}

void HeadButton::enterEvent(QEvent *) {
  setIndex(1);
}

qint8 HeadButton::index() const {
  return _index;
}

void HeadButton::setIndex(qint8 index) {
  _index = index;
  emit indexChanged();
}

void HeadButton::leaveEvent(QEvent *) {
  setIndex(0);
}
