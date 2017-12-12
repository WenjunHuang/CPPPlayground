#pragma once

#include <QDebug>
#include <tuple>


struct State {
  QScopedPointer<QStateMachine> stateMachine;
  QScopedPointer<QVariantAnimation> enteringAnimation;
  QScopedPointer<QVariantAnimation> leavingAnimation;

  QList<QPixmap> enterList;
  QList<QPixmap> leaveList;

  QPixmap image;
  QPixmap enterAnimationImage;
  QPixmap leaveAnimationImage;
  int enterAnimationDuration = 0;
  int enterAnimationFrameCount = 0;
  int leaveAnimationDuration = 0;
  int leaveAnimationFrameCount = 0;

  AnimationButton *button;

  QString name;

  State() : stateMachine{new QStateMachine},
            enteringAnimation{new QVariantAnimation},
            leavingAnimation{new QVariantAnimation} {}

  void setName(const QString& name) {
    this->name = name;
  }

  void setImage(const QPixmap &image) {
    this->image = image;
  }

  void enterState() {
    stateMachine->start();
  }

  void leaveState() {
    stateMachine->stop();
  }

  void updateEnterAnimation() {
    if (!enterAnimationImage.isNull() && enterAnimationFrameCount > 0 && enterAnimationDuration > 0) {
      enterList.clear();
      for (int i = 0; i < enterAnimationFrameCount; ++i) {
        enterList << enterAnimationImage.copy(i * (enterAnimationImage.width() / enterAnimationFrameCount),
                                              0,
                                              enterAnimationImage.width() / enterAnimationFrameCount,
                                              enterAnimationImage.height());
      }
      enteringAnimation->setStartValue(0);
      enteringAnimation->setEndValue(enterAnimationFrameCount - 1);
    }
  }

  void updateLeaveAnimation() {
    if (!leaveAnimationImage.isNull() && leaveAnimationFrameCount > 0 && leaveAnimationDuration > 0) {
      leaveList.clear();
      for (int j = 0; j < leaveAnimationFrameCount; j++) {
        leaveList << leaveAnimationImage.copy(j * (leaveAnimationImage.width() / leaveAnimationFrameCount),
                                              0,
                                              leaveAnimationImage.width() / leaveAnimationFrameCount,
                                              leaveAnimationImage.height());
      }

      leavingAnimation->setStartValue(0);
      leavingAnimation->setEndValue(leaveList.size() - 1);
    }
  }

  void initialize(AnimationButton *button) {
    this->button = button;
    updateEnterAnimation();
    updateLeaveAnimation();
    initAnimations();
    initStates();
  }

  void initStates() {
    QObject::connect(stateMachine.data(),&QStateMachine::stopped,[=](){
      qDebug() << "stateMachine "<< name << " Stopped";
    });
    QObject::connect(stateMachine.data(),&QStateMachine::started,[=](){
      qDebug() << "stateMachine " << name << " Started";
    });

    auto startState = new QState;
    auto leavedState = new QState;
    auto enteredState = new QState;
    startState->addTransition(button, &AnimationButton::mouseEntered, enteredState);
    startState->addTransition(button, &AnimationButton::mouseLeaved, leavedState);
    leavedState->addTransition(button, &AnimationButton::mouseEntered, enteredState);
    leavedState->addTransition(leavingAnimation.data(), &QVariantAnimation::finished, startState);
    enteredState->addTransition(button, &AnimationButton::mouseLeaved, leavedState);

    stateMachine->addState(startState);
    stateMachine->addState(leavedState);
    stateMachine->addState(enteredState);
    stateMachine->setInitialState(startState);

    QObject::connect(startState, &QState::entered, [=]() {
      button->setCurrentImage(image);
    });

    QObject::connect(enteredState, &QState::entered, [=]() {
      if (!enterList.isEmpty())
        enteringAnimation->start();
    });
    QObject::connect(enteredState, &QState::exited, [=]() {
      enteringAnimation->stop();
    });

    QObject::connect(leavedState, &QState::entered, [=]() {
      if (leaveList.isEmpty()) {
        // 没有动画可以显示，转移到startState
        emit leavingAnimation->finished();
      } else {
        leavingAnimation->start();
      }
    });
    QObject::connect(leavedState, &QState::exited, [=]() {
      leavingAnimation->stop();
    });
  }

  void initAnimations() {
    enteringAnimation.reset(new QVariantAnimation());
    enteringAnimation->setStartValue(0);
    enteringAnimation->setDuration(enterAnimationDuration);
    enteringAnimation->setEndValue(enterAnimationFrameCount - 1);

    QObject::connect(enteringAnimation.data(), &QVariantAnimation::valueChanged, [=](auto value) {
      if (!enterList.empty()) {
        int current = value.toInt();
        if (current >= 0 && current < enterList.size())
          button->setCurrentImage(enterList.at(current));
      }
    });

    leavingAnimation.reset(new QVariantAnimation());
    leavingAnimation->setStartValue(0);
    leavingAnimation->setDuration(leaveAnimationDuration);
    leavingAnimation->setEndValue(leaveAnimationFrameCount - 1);

    QObject::connect(leavingAnimation.data(), &QVariantAnimation::valueChanged, [=](auto value) {
      if (!leaveList.empty()) {
        int current = value.toInt();
        if (current >= 0 && current < leaveList.size())
          button->setCurrentImage(leaveList.at(current));
      }
    });
  }
};