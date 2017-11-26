//
// Created by xxzyjy on 05/11/2017.
//

#include "AnimationButton.h"
#include <QSignalTransition>
#include <QDebug>
#include <QPainter>

#include "AnimationButtonPrivate.h"

AnimationButton::AnimationButton(QWidget *parent) : QPushButton(parent), d_ptr(new AnimationButtonPrivate(this)) {
  setCursor(Qt::PointingHandCursor);
  d_ptr->initialize();
}

AnimationButton::~AnimationButton() {
}

void AnimationButton::enterEvent(QEvent *) {
  emit mouseEntered();
}

void AnimationButton::leaveEvent(QEvent *) {
  emit mouseLeaved();
}

void AnimationButton::paintEvent(QPaintEvent *) {
  Q_D(AnimationButton);
  QPainter painter(this);
  painter.drawPixmap(rect(), d->currentImage);
}

void AnimationButton::setEnterAnimationImage(const QPixmap &enter) {
  Q_D(AnimationButton);

  d->enterAnimationImage = enter;

  emit enterAnimationImageChanged();
}

QPixmap AnimationButton::currentImage() const {
  return d_func()->currentImage;
}

void AnimationButton::setCurrentImage(const QPixmap &image) {
  Q_D(AnimationButton);
  d->currentImage = image;
  emit currentImageChanged();
}

QPixmap AnimationButton::normalImage() const {
  return d_func()->normalImage;
}

void AnimationButton::setNormalImage(const QPixmap &image) {
  Q_D(AnimationButton);
  d->normalImage = image;
  emit normalImageChanged();
}

QPixmap AnimationButton::enterAnimationImage() const {
  return d_func()->enterAnimationImage;
}

int AnimationButton::enterAnimationDuration() const {
  return d_func()->enterAnimationDuration;
}

void AnimationButton::setEnterAnimationDuration(int duration) {
  Q_D(AnimationButton);
  if (duration > 0 && duration != d->enterAnimationDuration) {
    d->enterAnimationDuration = duration;
    emit enterAnimationDurationChanged();
  }
}

int AnimationButton::enterAnimationFrameCount() const {
  return d_func()->enterAnimationFrameCount;
}

void AnimationButton::setEnterAnimationFrameCount(int frameCount) {
  Q_D(AnimationButton);
  if (frameCount > 0 && frameCount != d->enterAnimationFrameCount) {
    d->enterAnimationFrameCount = frameCount;
    emit enterAnimationFrameCountChanged();
  }
}

QPixmap AnimationButton::leaveAnimationImage() const {
  return d_func()->leaveAnimationImage;
}

void AnimationButton::setLeaveAnimationImage(const QPixmap &leave) {
  Q_D(AnimationButton);
  d->leaveAnimationImage = leave;

}

int AnimationButton::leaveAnimationDuration() const {
  return d_func()->leaveAnimationDuration;
}

void AnimationButton::setLeaveAnimationDuration(int duration) {
  Q_D(AnimationButton);
  if (duration > 0 && duration != d->leaveAnimationDuration) {
    d->leaveAnimationDuration = duration;
    emit leaveAnimationDurationChanged();
  }

}

int AnimationButton::leaveAnimationFrameCount() const {
  return d_func()->leaveAnimationFrameCount;
}

void AnimationButton::setLeaveAnimationFrameCount(int frameCount) {
  Q_D(AnimationButton);
  if (frameCount > 0 && frameCount != d->leaveAnimationFrameCount) {
    d->leaveAnimationFrameCount = frameCount;
    emit leaveAnimationFrameCountChanged();
  }

}
