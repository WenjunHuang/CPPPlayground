#pragma once


#include <QWidget>

class TopWidget : public QWidget{
  Q_OBJECT
public:
  TopWidget(QWidget *parent = nullptr);

protected:
  void paintEvent(QPaintEvent *) override;

};



