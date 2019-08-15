#pragma once


#include <QWidget>
#include "TopWidget.h"
#include "BottomWidget.h"

class AbstractMainWidget: public QWidget {
  Q_OBJECT
public:
  AbstractMainWidget(QWidget *parent = nullptr);

  TopWidget* topWidget() const {
    return _topWidget;
  }

  BottomWidget* bottomWidget() const{
    return _bottomWidget;
  }

private:
  TopWidget *_topWidget;
  BottomWidget *_bottomWidget;
};



