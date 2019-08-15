//
// Created by xxzyjy on 05/11/2017.
//

#include "AnimationButton.h"
#include <QSignalTransition>
#include <QDebug>
#include <QPainter>
#include <QStylePainter>
#include <QStyleOptionButton>

#include "AnimationButtonPrivate.h"

#define DECL_STATE_PRI(state) \
  State state;

#define IMPL_STATE(Class, State) \
  QPixmap Class::State##Image() const { \
    return d_func()->State.image; \
  } \
  void Class::set##State##Image(const QPixmap &image){ \
    d_func()->State.setImage(image); \
  } \
  QPixmap Class::State##EnterAnimationImage() const { \
    return d_func()->State.enterAnimationImage; \
  } \
  void Class::set##State##EnterAnimationImage(const QPixmap &enter) {\
    d_func()->State.enterAnimationImage = enter; \
  } \
  int Class::State##EnterAnimationDuration() const { \
    return d_func()->State.enterAnimationDuration; \
  } \
  void Class::set##State##EnterAnimationDuration(int duration) { \
    d_func()->State.enterAnimationDuration = duration; \
  } \
  int Class::State##EnterAnimationFrameCount() const { \
    return d_func()->State.enterAnimationFrameCount; \
  } \
  void Class::set##State##EnterAnimationFrameCount(int frameCount) { \
    d_func()->State.enterAnimationFrameCount = frameCount; \
  } \
  QPixmap Class::State##LeaveAnimationImage() const { \
    return d_func()->State.leaveAnimationImage; \
  } \
  void Class::set##State##LeaveAnimationImage(const QPixmap &leave) { \
    d_func()->State.leaveAnimationImage = leave; \
  } \
  int Class::State##LeaveAnimationDuration() const { \
    return d_func()->State.leaveAnimationDuration; \
  } \
  void Class::set##State##LeaveAnimationDuration(int duration) { \
    d_func()->State.leaveAnimationDuration = duration; \
  } \
  int Class::State##LeaveAnimationFrameCount() const { \
     return d_func()->State.leaveAnimationFrameCount; \
  } \
  void Class::set##State##LeaveAnimationFrameCount(int frameCount) { \
    d_func()->State.leaveAnimationFrameCount = frameCount; \
  }

struct AnimationButtonPrivate {
  AnimationButtonPrivate(AnimationButton *ptr) : q_ptr(ptr) {
    normal.setName("normal");
    checked.setName("checked");
  }

  AnimationButton *q_ptr;
  QPixmap currentImage;
  State *currentState = nullptr;

  DECL_STATE_PRI(normal)
  DECL_STATE_PRI(checked)

  void makeNormalCurrent() {
    if (currentState != nullptr)
      currentState->leaveState();

    currentState = &normal;
    currentState->enterState();
    q_ptr->update();
  }

  void makeCheckCurrent() {
    if (currentState != nullptr)
      currentState->leaveState();

    currentState = &checked;
    currentState->enterState();
    q_ptr->update();
  }

  void toggled(bool isChecked) {
    q_ptr->style()->unpolish(q_ptr);
    q_ptr->style()->polish(q_ptr);
    if (isChecked) {
      makeCheckCurrent();
    } else {
      makeNormalCurrent();
    }
  }
};

IMPL_STATE(AnimationButton, normal)

IMPL_STATE(AnimationButton, checked)

AnimationButton::AnimationButton(QWidget *parent) : QPushButton(parent), d_ptr(new AnimationButtonPrivate(this)) {
  setProperty("hover",false);
  setAttribute(Qt::WA_Hover);
  setCursor(Qt::PointingHandCursor);
  installEventFilter(this);
  using namespace std::placeholders;
  connect(this, &QAbstractButton::toggled, std::bind(&AnimationButtonPrivate::toggled, d_ptr.data(), _1));
}

AnimationButton::~AnimationButton() {
}

void AnimationButton::enterEvent(QEvent *) {
  style()->polish(this);
  emit mouseEntered();
}

void AnimationButton::leaveEvent(QEvent *) {
  style()->polish(this);
  emit mouseLeaved();
}

void AnimationButton::paintEvent(QPaintEvent *) {
  if (testAttribute(Qt::WA_Hover))
    qDebug() << "has hover";
  Q_D(AnimationButton);

  QStyleOptionButton option;
  initStyleOption(&option);
  QStylePainter painter(this);
  painter.drawRect(rect());

  if (!d->currentImage.isNull()) {
    painter.drawPixmap((width() - d->currentImage.width()) / 2,
                       (height() - d->currentImage.height()) / 2,
                       d->currentImage);
  }
}

QPixmap AnimationButton::currentImage() const {
  return d_func()->currentImage;
}

void AnimationButton::setCurrentImage(const QPixmap &image) {
  Q_D(AnimationButton);
  if (image.isNull())
    qDebug() << "null image";
  d->currentImage = image;
  update();
}

void AnimationButton::showEvent(QShowEvent *) {
  Q_D(AnimationButton);
  d->normal.initialize(this);
  d->checked.initialize(this);

  if (isChecked())
    d->makeCheckCurrent();
  else
    d->makeNormalCurrent();
}

bool AnimationButton::eventFilter(QObject *obj, QEvent *event) {
  if (event->type() == QEvent::HoverEnter) {
    setProperty("hover",true);
    style()->polish(this);
  }else if (event->type() == QEvent::HoverLeave){
    setProperty("hover",false);
    style()->polish(this);
  }

  return QObject::eventFilter(obj, event);
}
