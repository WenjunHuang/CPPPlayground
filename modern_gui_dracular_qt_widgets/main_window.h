//
// Created by rick on 2021/4/9.
//

#pragma once

#include <QMainWindow>
namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow {
    Q_OBJECT
  public:
    MainWindow(QWidget* parent = nullptr);

    void foo();

  private:
    Ui::MainWindow* _ui;
};
