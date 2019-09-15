//
// Created by rick on 2019/9/7.
//

#pragma once

#include <QApplication>
#include <QMenuBar>
#include <QStatusBar>
#include <QtWidgets/QAction>
#include <QtWidgets/QMainWindow>

class MenuAndStatusBar : public QMainWindow {
    Q_OBJECT
  public:
    MenuAndStatusBar() : QMainWindow{nullptr} { initUI(); }

  private:
    void initUI() {
        auto action = new QAction(QStringLiteral("&Exit"), this);
        action->setShortcuts({QKeySequence("Ctrl+Q")});
        action->setStatusTip("Exit application");
        connect(action, &QAction::triggered, qApp, &QApplication::quit);

        auto menubar  = menuBar();
        auto fileMenu = menubar->addMenu("&File");
        fileMenu->addAction(action);

        auto statusbar = statusBar();
        statusbar->showMessage(QStringLiteral("Ready"));

        setGeometry(300, 300, 300, 200);
        setWindowTitle(QStringLiteral("Simple menu"));
        show();
    }
};
