#pragma once

#include <QtCore>

class Counter : public QObject {
Q_OBJECT
public:
  Counter() { _value = 0; }

  int value() const { return _value; }

public slots:
  void setValue(int value);
signals:
  void valueChanged(int newValue);
  void changed();
public:
  int _value;
};



