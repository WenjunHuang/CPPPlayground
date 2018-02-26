#pragma once
#include <QWidget>

class ResizeWidget: public QWidget {
  Q_OBJECT
public:
  ResizeWidget(QWidget *parent = nullptr);
  ~ResizeWidget();
};



