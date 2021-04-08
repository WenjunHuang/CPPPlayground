//
// Created by rick on 2021/4/8.
//

#pragma once
#include <QtWidgets>

namespace Ui { class MainWindow; }

class MainWindow : public QMainWindow{
    Q_OBJECT
  public:
    MainWindow();
  private:
    Ui::MainWindow *ui;
};
