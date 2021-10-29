//
// Created by rick on 2021/10/29.
//

#pragma once
#include <QObject>
#include <memory>

class QQmlApplicationEngine;
namespace ScreenPlay {
class App : public QObject {
  Q_OBJECT
 public:
  App();
  void init();

 public slots:
  QString version() const;

 private:
  std::unique_ptr<QQmlApplicationEngine> mainWindowEngine_;
};
}  // namespace ScreenPlay
