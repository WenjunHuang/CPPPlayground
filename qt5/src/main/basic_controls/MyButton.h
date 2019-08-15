#pragma once


#include <QWidget>
#include <QPushButton>

class MyButton : public QPushButton {
Q_OBJECT
public:
  MyButton(QWidget *parent = nullptr);

protected:
  void paintEvent(QPaintEvent *) override;

};



