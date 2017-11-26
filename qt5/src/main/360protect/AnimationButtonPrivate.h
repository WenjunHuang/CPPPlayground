#pragma once
struct AnimationButtonPrivate {
  AnimationButtonPrivate(AnimationButton *ptr) : q_ptr(ptr) {}

  AnimationButton *q_ptr;
  QPixmap currentImage;
  QPixmap normalImage;
  QPixmap enterAnimationImage;
  QPixmap leaveAnimationImage;

  QScopedPointer<QStateMachine> stateMachine;
  QScopedPointer<QVariantAnimation> enteringAnimation;
  QScopedPointer<QVariantAnimation> leavingAnimation;

  QList<QPixmap> enterList;
  QList<QPixmap> leaveList;

  int enterAnimationDuration;
  int enterAnimationFrameCount;
  int leaveAnimationDuration;
  int leaveAnimationFrameCount;

  Q_DECLARE_PUBLIC(AnimationButton)

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
      for (int j = 0; j < 8; j++) {
        leaveList << leaveAnimationImage.copy(j * (leaveAnimationImage.width() / leaveAnimationFrameCount),
                                              0,
                                              leaveAnimationImage.width() / leaveAnimationFrameCount,
                                              leaveAnimationImage.height());
      }

      leavingAnimation->setStartValue(0);
      leavingAnimation->setEndValue(leaveList.size() - 1);
    }

  }

  void initialize() {
    initAnimations();
    initStates();
    setupUpdates();

    Q_Q(AnimationButton);
    QObject::connect(q, SIGNAL(currentImageChanged()), q, SLOT(update()));

  }

  void initStates(){
    Q_Q(AnimationButton);

    stateMachine.reset(new QStateMachine);
    auto startState = new QState;
    auto leavedState = new QState;
    auto enteredState = new QState;
    startState->addTransition(q, &AnimationButton::mouseEntered, enteredState);
    leavedState->addTransition(q, &AnimationButton::mouseEntered, enteredState);
    enteredState->addTransition(q, &AnimationButton::mouseLeaved, leavedState);
    stateMachine->addState(startState);
    stateMachine->addState(leavedState);
    stateMachine->addState(enteredState);
    stateMachine->setInitialState(startState);

    QObject::connect(startState, &QState::entered, [=]() {
      q->setCurrentImage(q->normalImage());
    });

    QObject::connect(enteredState, &QState::entered, [=]() {
      enteringAnimation->start();
    });
    QObject::connect(enteredState, &QState::exited, [=]() {
      enteringAnimation->stop();
    });

    QObject::connect(leavedState, &QState::entered, [=]() {
      leavingAnimation->start();
    });
    QObject::connect(leavedState, &QState::exited, [=]() {
      leavingAnimation->stop();
    });
    stateMachine->start();
  }

  void initAnimations(){
    Q_Q(AnimationButton);

    enteringAnimation.reset(new QVariantAnimation());
    enteringAnimation->setStartValue(0);
    QObject::connect(q, &AnimationButton::enterAnimationDurationChanged, [=]() {
      enteringAnimation->setDuration(enterAnimationDuration);
    });
    QObject::connect(q, &AnimationButton::enterAnimationFrameCountChanged, [=]() {
      enteringAnimation->setEndValue(enterAnimationFrameCount - 1);
    });
    QObject::connect(enteringAnimation.data(), &QVariantAnimation::valueChanged, [=](auto value) {
      if (!enterList.empty()) {
        int current = value.toInt();
        if (current >= 0 && current < enterList.size())
          q->setCurrentImage(enterList.at(current));
      }
    });

    leavingAnimation.reset(new QVariantAnimation());
    leavingAnimation->setStartValue(0);
    QObject::connect(q, &AnimationButton::leaveAnimationDurationChanged, [=]() {
      leavingAnimation->setDuration(leaveAnimationDuration);
    });
    QObject::connect(q, &AnimationButton::leaveAnimationFrameCountChanged, [=]() {
      leavingAnimation->setEndValue(leaveAnimationFrameCount - 1);
    });
    QObject::connect(leavingAnimation.data(), &QVariantAnimation::valueChanged, [=](auto value) {
      if (!leaveList.empty()) {
        int current = value.toInt();
        if (current >= 0 && current < leaveList.size())
          q->setCurrentImage(leaveList.at(current));
      }
    });
  }
  void setupUpdates(){
    Q_Q(AnimationButton);
    using MF = void (AnimationButton::*)();
    constexpr MF ea[] = {&AnimationButton::enterAnimationFrameCountChanged,
                         &AnimationButton::enterAnimationDurationChanged,
                         &AnimationButton::enterAnimationImageChanged};
    auto bindEnter = std::bind(&AnimationButtonPrivate::updateEnterAnimation, this);
    for (auto me : ea) {
      QObject::connect(q,
                       me,
                       bindEnter);
    }

    auto bindLeave = std::bind(&AnimationButtonPrivate::updateLeaveAnimation, this);
    MF la[] = {&AnimationButton::leaveAnimationFrameCountChanged,
               &AnimationButton::leaveAnimationDurationChanged,
               &AnimationButton::leaveAnimationImageChanged};
    for (auto me : la) {
      QObject::connect(q,
                       me,
                       bindLeave);
    }
  }
};