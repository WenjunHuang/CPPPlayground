//
// Created by rick on 2021/4/8.
//

#include "main_window.h"
#include "./ui_mainwindow.h"
#include "../../modern_gui_dracular_qt_widgets/main_window.h"


MainWindow::MainWindow() : ui(new Ui::MainWindow) {
    ui->setupUi(this);
    QObject::connect(ui->toggle_button, &QPushButton::clicked, [&]() {
        auto width        = ui->frame_left_menu->width();
        auto maxExtend    = 250;
        auto standard     = 70;
        int widthExtended = 0;
        if (width == 70) {
            widthExtended = maxExtend;
        } else {
            widthExtended = standard;
        }

        auto animation =
            new QPropertyAnimation(ui->frame_left_menu, "minimumWidth");
        animation->setDuration(400);
        animation->setStartValue(width);
        animation->setEndValue(widthExtended);
        animation->setEasingCurve(QEasingCurve::InOutQuart);
        animation->start(QAbstractAnimation::DeleteWhenStopped);
    });
    QObject::connect(ui->page_button_1,&QPushButton::clicked,[&](){
        ui->stackedWidget->setCurrentWidget(ui->page_1);
    });
    QObject::connect(ui->page_button_2,&QPushButton::clicked,[&](){
      ui->stackedWidget->setCurrentWidget(ui->page_2);
    });
    QObject::connect(ui->page_button_3,&QPushButton::clicked,[&](){
      ui->stackedWidget->setCurrentWidget(ui->page_3);
    });
}
