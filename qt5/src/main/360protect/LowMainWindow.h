#pragma once


#include <QWidget>
#include "AnimationButton.h"
#include "TwoStateButton.h"
#include "ThreeStateButton.h"

class LowMainWindow : public QWidget {
Q_OBJECT
public:
  LowMainWindow(QWidget *parent = nullptr);

  QPushButton *killAndFixBtn() const { return _killAndFixBtn; }

  QPushButton *cleanBtn() const { return _cleanBtn; }

  QPushButton *speedUpBtn() const { return _speedUpBtn; }

protected:
  void paintEvent(QPaintEvent *) override;

private:
  QPushButton *_killAndFixBtn;
  QPushButton *_cleanBtn;
  QPushButton *_speedUpBtn;
};



