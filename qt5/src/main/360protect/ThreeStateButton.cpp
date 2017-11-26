//
// Created by xxzyjy on 10/11/2017.
//

#include <QPainter>
#include <QMouseEvent>
#include "ThreeStateButton.h"

qint8 ThreeStateButton::index() const {
  return _index;
}

void ThreeStateButton::setIndex(qint8 index) {
  _index = index;
  emit indexChanged();
}

ThreeStateButton::ThreeStateButton(QWidget *parent) : QPushButton(parent) {
  setCursor(Qt::PointingHandCursor);
  _index = 0;

  connect(this,SIGNAL(indexChanged()), this, SLOT(update()));
  connect(this,SIGNAL(pixmapUrlChanged()),this,SLOT(update()));
}

void ThreeStateButton::paintEvent(QPaintEvent *event) {
  if (_index >=0 && !_pixmaps.isEmpty()) {
    QPainter painter(this);
    auto currentPixmap = _pixmaps.at(_index);
    painter.drawPixmap((width() - currentPixmap.width()) / 2,
                       (height() - currentPixmap.height()) / 2,
                       currentPixmap.width(),
                       currentPixmap.height(),
                       currentPixmap);
    event->accept();
  }
}

void ThreeStateButton::enterEvent(QEvent *) {
  setIndex(1);
}

void ThreeStateButton::leaveEvent(QEvent *) {
  setIndex(0);
}

void ThreeStateButton::mousePressEvent(QMouseEvent *event) {
  if (event->button() == Qt::LeftButton) {
    setIndex(2);
    QPushButton::mousePressEvent(event);
  }
}

void ThreeStateButton::mouseReleaseEvent(QMouseEvent *event) {
  if (event->button() == Qt::LeftButton){
    setIndex(1);
    QPushButton::mouseReleaseEvent(event);
  }
}

QString ThreeStateButton::pixmapUrl() const {
  return _pixmapUrl;
}

void ThreeStateButton::setPixmapUrl(const QString &url) {
  _pixmapUrl = url;

  QPixmap pixmap(_pixmapUrl);
  for (int i = 0; i < 4; ++i)
    _pixmaps << pixmap.copy(i * (pixmap.width() / 4), 0, pixmap.width() / 4, pixmap.height());

  emit pixmapUrlChanged();
}
