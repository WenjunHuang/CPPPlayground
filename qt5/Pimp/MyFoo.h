#pragma once

#include <memory>
#include <string>
#include <QObject>

class MyFooPrivate;

class MyFoo : public QObject {
Q_OBJECT
public:
  MyFoo(QStringView name);

  ~MyFoo() override;

  void setName(QStringView name);

  QString getName() const;

  QString personalSign() const;

  void addHabit(QStringView habbit);

private:
  std::unique_ptr<MyFooPrivate> d_ptr;

  Q_DECLARE_PRIVATE(MyFoo)
};



