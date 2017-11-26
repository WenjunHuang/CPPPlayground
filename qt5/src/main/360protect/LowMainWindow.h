#pragma once


#include <QWidget>
#include "AnimationButton.h"
#include "TwoStateButton.h"
#include "ThreeStateButton.h"

class LowMainWindow : public QWidget{
  Q_OBJECT
public:
  LowMainWindow(QWidget *parent = nullptr);
protected:
  void paintEvent(QPaintEvent *) override;

private:
  AnimationButton *_btn;
  AnimationButton *_btn1;
  AnimationButton *_btn2;
  TwoStateButton *_btn3;
  TwoStateButton *_btn4;
  TwoStateButton *_btn5;
  TwoStateButton *_btn6;
  ThreeStateButton *_btn7;



};



