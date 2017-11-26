#pragma once


#include <QWidget>
#include "UpMainWindow.h"
#include "LowMainWindow.h"

class MaskMainWidget : public QWidget{
  Q_OBJECT
public:
  MaskMainWidget(QWidget *parent = nullptr);

private:
  UpMainWindow *_upmain;
  LowMainWindow *_lowmain;

};



