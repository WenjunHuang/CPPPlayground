//
// Created by xxzyjy on 03/12/2017.
//

#include <QStyleOptionButton>
#include <QStylePainter>
#include <QDebug>
#include "CleanMainButton.h"

struct CleanMainButtonPrivate {
  CleanMainButtonPrivate(CleanMainButton *button) : q_ptr{button} {}

  CleanMainButton *q_ptr;
  QPixmap borderPixmap;
  QPixmap groupPixmap;
};

CleanMainButton::CleanMainButton(QWidget *parent) : AnimationButton(parent),d_ptr{new CleanMainButtonPrivate(this)} {
}

void CleanMainButton::paintEvent(QPaintEvent *event) {
  Q_D(CleanMainButton);

  QStyleOptionButton option;
  initStyleOption(&option);
  QStylePainter painter(this);
//  painter.drawRect(rect());

  if (!d->borderPixmap.isNull()) {
    painter.drawPixmap((width() - d->borderPixmap.width()) / 2,
                       (height() - d->borderPixmap.height()) / 2,
                       d->borderPixmap);
  }
  QPixmap image = currentImage();
  if (!image.isNull()) {
    painter.drawPixmap((width() - image.width()) / 2,
                       (height() - image.height()) / 2,
                       image);
  }
  if (!d->groupPixmap.isNull()) {
    painter.drawPixmap((width() - d->groupPixmap.width()),
                       height() - 1.5 * (d->groupPixmap.height()),
                       d->groupPixmap);
  }
}

QPixmap CleanMainButton::borderPixmap() const {
  return d_func()->borderPixmap;
}

void CleanMainButton::setBorderPixmap(const QPixmap &pixmap) {
  d_func()->borderPixmap = pixmap;
}

QPixmap CleanMainButton::groupPixmap() const {
  return d_func()->groupPixmap;
}

void CleanMainButton::setGroupPixmap(const QPixmap &pixmap) {
  d_func()->groupPixmap = pixmap;
}

CleanMainButton::~CleanMainButton() = default;
