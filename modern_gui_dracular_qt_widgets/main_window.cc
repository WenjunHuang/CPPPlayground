//
// Created by rick on 2021/4/9.
//

#include "main_window.h"
#include "ui_mainwindow.h"

void MainWindow::foo() {
    auto width     = _ui->leftMenuBg->width();
    auto maxExtend = 240;
    auto standard  = 60;

    auto widthExtended = 0;
    if (width == standard) {
        widthExtended = maxExtend;
    } else {
        widthExtended = standard;
    }
//    _ui->leftMenuBg->setMaximumWidth(widthExtended);
//    _ui->leftMenuBg->update();

    auto animation = new QPropertyAnimation(_ui->leftMenuBg, "minimumWidth");
    animation->setDuration(500);
    animation->setStartValue(width);
    animation->setEndValue(widthExtended);
    animation->setEasingCurve(QEasingCurve::InOutQuart);
    animation->start();
}
MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent), _ui(new Ui::MainWindow) {
    _ui->setupUi(this);
    setWindowFlags(Qt::FramelessWindowHint);
    setAttribute(Qt::WA_TranslucentBackground);
    _ui->titleRightInfo->setWindowToMove(this);

    // drop shadow
    auto shadow = new QGraphicsDropShadowEffect(this);
    shadow->setBlurRadius(17);
    shadow->setXOffset(0);
    shadow->setYOffset(0);
    shadow->setColor(QColor(0, 0, 0, 150));
    _ui->centralContent->setGraphicsEffect(shadow);

    // 右上的按钮
    connect(_ui->minimizeAppBtn, &QPushButton::clicked,
            [=]() { this->showMinimized(); });
    connect(_ui->maximizeRestoreAppBtn, &QPushButton::clicked, [=]() {
        if (this->isMaximized())
            this->showNormal();
        else
            this->showMaximized();
    });
    connect(_ui->closeAppBtn, &QPushButton::clicked, [=]() { this->close(); });
}
