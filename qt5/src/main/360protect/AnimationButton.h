#pragma once

#include <QPushButton>
#include <QVariantAnimation>
#include <QStateMachine>
#include <QPointer>
#include <QList>

#define DECLARE_STATE(state) \
  Q_PROPERTY(QPixmap state##Image  READ state##Image WRITE set##state##Image) \
  Q_PROPERTY(QPixmap state##EnterAnimationImage READ state##EnterAnimationImage WRITE set ## state ## EnterAnimationImage) \
  Q_PROPERTY(int state ## EnterAnimationDuration READ state##EnterAnimationDuration WRITE set ## state ## EnterAnimationDuration) \
  Q_PROPERTY(int state ## EnterAnimationFrameCount READ state##EnterAnimationFrameCount WRITE set ## state ## EnterAnimationFrameCount) \
  Q_PROPERTY(QPixmap state ## LeaveAnimationImage READ state##LeaveAnimationImage WRITE set ## state ## LeaveAnimationImage) \
  Q_PROPERTY(int state ## LeaveAnimationDuration READ state ## LeaveAnimationDuration WRITE set ## state ## LeaveAnimationDuration) \
  Q_PROPERTY(int state ## LeaveAnimationFrameCount READ state ## LeaveAnimationFrameCount WRITE set ## state ## LeaveAnimationFrameCount) \
  public: \
  QPixmap state##Image() const; \
  void set ## state ##Image(const QPixmap &image); \
  QPixmap state##EnterAnimationImage() const; \
  void set ## state ## EnterAnimationImage(const QPixmap &enter); \
  int state ## EnterAnimationDuration() const; \
  void set ## state ## EnterAnimationDuration(int duration); \
  int state##EnterAnimationFrameCount() const; \
  void set##state##EnterAnimationFrameCount(int frameCount); \
  QPixmap state##LeaveAnimationImage() const; \
  void set##state##LeaveAnimationImage(const QPixmap &leave); \
  int state##LeaveAnimationDuration() const; \
  void set ## state ## LeaveAnimationDuration(int duration); \
  int state##LeaveAnimationFrameCount() const; \
  void set ## state ##LeaveAnimationFrameCount(int frameCount);


class AnimationButtonPrivate;

class AnimationButton : public QPushButton {
Q_OBJECT
  Q_PROPERTY(QPixmap currentImage READ currentImage WRITE setCurrentImage)

DECLARE_STATE(normal)
DECLARE_STATE(checked)

public:
  AnimationButton(QWidget *parent = nullptr);

  ~AnimationButton();

  QPixmap currentImage() const;

  void setCurrentImage(const QPixmap &image);

protected:
  bool eventFilter(QObject* obj,QEvent *event) override;
  void showEvent(QShowEvent *) override;
  void enterEvent(QEvent *) override;

  void leaveEvent(QEvent *) override;

  void paintEvent(QPaintEvent *) override;

signals:

  void mouseEntered();

  void mouseLeaved();

private:
  QScopedPointer<AnimationButtonPrivate> d_ptr;

  Q_DECLARE_PRIVATE(AnimationButton)
};



