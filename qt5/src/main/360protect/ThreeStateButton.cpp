//
// Created by xxzyjy on 10/11/2017.
//

#include <QPainter>
#include <QMouseEvent>
#include <QStyleOptionButton>
#include <QDebug>
#include <QStylePainter>
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

  connect(this, SIGNAL(indexChanged()), this, SLOT(update()));
  connect(this, SIGNAL(pixmapUrlChanged()), this, SLOT(update()));
}

void ThreeStateButton::paintEvent(QPaintEvent *event) {
  if (_index >= 0 && !_pixmaps.isEmpty()) {
    QStyleOptionButton option;
    initStyleOption(&option);

    QStylePainter painter(this);
    auto currentPixmap = _pixmaps.at(_index);
    painter.drawPixmap((width() - currentPixmap.width()) / 2,
                       (height() - currentPixmap.height()) / 2,
                       size().width(),
                       size().height(),
                       currentPixmap);

    auto str = text();
    if (!str.isEmpty()) {
      auto w = option.fontMetrics.width(str);
      option.rect = QRect((width() - w) / 2 + 5, (height() - currentPixmap.height()) / 2, w,
                          option.fontMetrics.height());
      painter.drawControl(QStyle::CE_PushButtonLabel, option);
    }

  }

  event->accept();
}

void ThreeStateButton::enterEvent(QEvent *event) {
  setIndex(1);
  QPushButton::enterEvent(event);
}

void ThreeStateButton::leaveEvent(QEvent *event) {
  setIndex(0);
  QPushButton::leaveEvent(event);
}

void ThreeStateButton::mousePressEvent(QMouseEvent *event) {
  if (event->button() == Qt::LeftButton) {
    setIndex(2);
    QPushButton::mousePressEvent(event);
  }
}

void ThreeStateButton::mouseReleaseEvent(QMouseEvent *event) {
  if (event->button() == Qt::LeftButton) {
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
