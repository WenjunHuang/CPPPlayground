#pragma once
#include <QtCore>

class Listener : public QObject{
  Q_OBJECT
public slots:
  void onValue(int value);
  void onValue(QString sender);

public:
  int _value;
  QString _sender;
};



