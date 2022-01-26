//
// Created by rick on 2021/12/10.
//
#include <QApplication>
#include <QPushButton>
#include <QPropertyAnimation>
int main(int argc,char *argv[]) {
  QApplication app(argc,argv);

  QPushButton button{"I am moving"};
  button.show();

  QPropertyAnimation animation(&button,"geometry");
  animation.setDuration(2000);

  animation.setStartValue(QRect(0,0,100,30));
  animation.setEndValue(QRect(250,250,100,30));
  animation.setEasingCurve(QEasingCurve::OutBounce);
  animation.start();

  return app.exec();

}