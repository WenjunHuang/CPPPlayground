#pragma once


#include <QWidget>
#include "ThreeStateButton.h"
#include "HeadButton.h"
#include "WordLineButton.h"

namespace Ui {
  class UpMainWindow;
}
class UpMainWindow : public QWidget {
Q_OBJECT
public:
  UpMainWindow(QWidget *parent = nullptr);
  ~UpMainWindow();

protected:
  void mousePressEvent(QMouseEvent *) override;

  void mouseReleaseEvent(QMouseEvent *) override;

  void mouseMoveEvent(QMouseEvent *) override;

  void paintEvent(QPaintEvent *) override;

  bool _mousePressed;
  QPoint _windowPos;
  QPoint _mousePos;
private:
  Ui::UpMainWindow *_ui;
};



