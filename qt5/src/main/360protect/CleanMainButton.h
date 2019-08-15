#pragma once


#include "AnimationButton.h"

class CleanMainButtonPrivate;

class CleanMainButton: public AnimationButton {
Q_OBJECT
Q_PROPERTY(QPixmap borderPixmap READ borderPixmap WRITE setBorderPixmap)
  Q_PROPERTY(QPixmap groupPixmap READ groupPixmap WRITE setGroupPixmap)
public:
  CleanMainButton(QWidget *parent = nullptr);
  ~CleanMainButton();

  QPixmap borderPixmap() const;
  void setBorderPixmap(const QPixmap& pixmap);
  QPixmap groupPixmap() const;
  void setGroupPixmap(const QPixmap& pixmap);
protected:
  void paintEvent(QPaintEvent *event) override;
private:
  QScopedPointer<CleanMainButtonPrivate> d_ptr;
  Q_DECLARE_PRIVATE(CleanMainButton)
};



