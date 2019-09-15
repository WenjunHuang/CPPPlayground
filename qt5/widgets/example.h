//
// Created by rick on 2019/9/7.
//

#pragma once

#include <QtWidgets/QDesktopWidget>
#include <QtWidgets/QWidget>
class Example : public QWidget {
    Q_OBJECT
  public:
    explicit Example(QWidget* parent = nullptr) : QWidget{parent} { initUI(); }

  protected:
    void closeEvent(QCloseEvent* event) override;

  private:
    void initUI() {
        resize(250, 150);
        setWindowTitle(QStringLiteral("Center"));
        center();
        show();
    }

    void center() {
        auto qr = frameGeometry();
        auto cp = QDesktopWidget().availableGeometry().center();
        qr.moveCenter(cp);
        move(qr.topLeft());
    }
};
