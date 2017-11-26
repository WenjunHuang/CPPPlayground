#pragma once


#include <QPushButton>

// 下划线按钮
class WordLineButton : public QPushButton {
Q_OBJECT
public:
  explicit WordLineButton(QWidget *parent = nullptr);

protected:
  void paintEvent(QPaintEvent *) override;

  void enterEvent(QEvent *) override;

  void leaveEvent(QEvent *) override;

private:
  bool _enter;
};



