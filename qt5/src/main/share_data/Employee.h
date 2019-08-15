#pragma once


#include <QSharedData>

class EmployeeData;

class Employee {
public:
  Employee();
  Employee(int id, const QString &name);

  void setId(int id);
  void setName(const QString&name);

  int id() const;
  QString name() const;
private:
  QSharedDataPointer<EmployeeData> d;
};



