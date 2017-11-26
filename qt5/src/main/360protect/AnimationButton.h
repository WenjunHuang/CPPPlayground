#pragma once

#include <QPushButton>
#include <QVariantAnimation>
#include <QStateMachine>
#include <QPointer>
#include <QList>

class AnimationButtonPrivate;

class AnimationButton : public QPushButton {
Q_OBJECT
  Q_PROPERTY(QPixmap currentImage
               READ
                 currentImage
               WRITE
                 setCurrentImage
               NOTIFY
               currentImageChanged)
  Q_PROPERTY(QPixmap normalImage
               READ
                 normalImage
               WRITE
                 setNormalImage
               NOTIFY
               normalImageChanged)
  Q_PROPERTY(QPixmap enterAnimationImage
               READ
                 enterAnimationImage
               WRITE
                 setEnterAnimationImage
               NOTIFY
               enterAnimationImageChanged)
  Q_PROPERTY(int enterAnimationDuration
               READ
                 enterAnimationDuration
               WRITE
                 setEnterAnimationDuration
               NOTIFY
               enterAnimationDurationChanged)
  Q_PROPERTY(int enterAnimationFrameCount
               READ
                 enterAnimationFrameCount
               WRITE
                 setEnterAnimationFrameCount
               NOTIFY
               enterAnimationFrameCountChanged)
  Q_PROPERTY(QPixmap leaveAnimationImage
               READ
                 leaveAnimationImage
               WRITE
                 setLeaveAnimationImage
               NOTIFY
               leaveAnimationImageChanged)
  Q_PROPERTY(int leaveAnimationDuration
               READ
                 leaveAnimationDuration
               WRITE
                 setLeaveAnimationDuration
               NOTIFY
               leaveAnimationDurationChanged)
  Q_PROPERTY(int leaveAnimationFrameCount
               READ
                 leaveAnimationFrameCount
               WRITE
                 setLeaveAnimationFrameCount
               NOTIFY
               leaveAnimationFrameCountChanged)

public:
  AnimationButton(QWidget *parent = nullptr);

  ~AnimationButton();

  QPixmap currentImage() const;

  void setCurrentImage(const QPixmap &image);

  QPixmap normalImage() const;

  void setNormalImage(const QPixmap &image);

  QPixmap enterAnimationImage() const;

  void setEnterAnimationImage(const QPixmap &enter);

  int enterAnimationDuration() const;

  void setEnterAnimationDuration(int duration);

  int enterAnimationFrameCount() const;

  void setEnterAnimationFrameCount(int frameCount);

  QPixmap leaveAnimationImage() const;
  void setLeaveAnimationImage(const QPixmap &leave);

  int leaveAnimationDuration() const;
  void setLeaveAnimationDuration(int duration);

  int leaveAnimationFrameCount() const;
  void setLeaveAnimationFrameCount(int frameCount);

protected:
  void enterEvent(QEvent *) override;

  void leaveEvent(QEvent *) override;

  void paintEvent(QPaintEvent *) override;

signals:
  void currentImageChanged();

  void normalImageChanged();

  void enterAnimationImageChanged();

  void enterAnimationDurationChanged();

  void enterAnimationFrameCountChanged();

  void leaveAnimationImageChanged();

  void leaveAnimationDurationChanged();

  void leaveAnimationFrameCountChanged();

  void mouseEntered();
  void mouseLeaved();
private:
  QScopedPointer<AnimationButtonPrivate> d_ptr;
  Q_DECLARE_PRIVATE(AnimationButton)
};



