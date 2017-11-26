#pragma once
#include <QWidget>

class MyWidget: public QWidget {
  Q_OBJECT
public:
  MyWidget(QWidget *parent = nullptr);
  ~MyWidget();
};



