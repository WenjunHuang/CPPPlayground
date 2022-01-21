//
// Created by rick on 2021/12/31.
//
#include <QDebug>
#include <QDateTime>
#include <QTime>
#include <QDate>

int main() {
  QDate today = QDate::currentDate();
  qInfo() << today;
  qInfo() << today.addDays(1);
  qInfo() << today.addYears(20);
  qInfo() << today.toString(Qt::DateFormat::SystemLocaleDate);
  qInfo() << today.toString(Qt::DateFormat::TextDate);


  QTime now = QTime::currentTime();
  qInfo() << now;
  qInfo() << now.toString(Qt::DateFormat::DefaultLocaleLongDate);
  qInfo() << now.toString(Qt::DateFormat::DefaultLocaleShortDate);

  QDateTime current = QDateTime::currentDateTime();
  qInfo() << "current: " << current;

  QDateTime expire = current.addDays(45);
  qInfo() << "expire: " << expire;

  if (current > expire) {
    qInfo() << "Expired";
  } else {
    qInfo() << "Not expired";
  }

}

