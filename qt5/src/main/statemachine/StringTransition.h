#pragma once


#include <QEvent>
#include <QString>
#include <QAbstractTransition>

struct StringEvent: public QEvent {
  StringEvent(const QString &val):QEvent(QEvent::Type(QEvent::User + 1)),value(val){}
  QString value;
};

class StringTransition:public QAbstractTransition {
  Q_OBJECT
public:
  StringTransition(const QString &value):_value(value){}

protected:
  bool eventTest(QEvent *e) override {
    if (e->type() != QEvent::Type(QEvent::User + 1))
      return false;

    auto se = static_cast<StringEvent *>(e);
    return _value == se->value;
  }

  void onTransition(QEvent *) override {}
private:
  QString _value;

};



