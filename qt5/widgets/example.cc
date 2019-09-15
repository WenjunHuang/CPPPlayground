//
// Created by rick on 2019/9/7.
//

#include "example.h"
#include <QtWidgets/QMessageBox>
#include <QCloseEvent>

void Example::closeEvent(QCloseEvent* event) {
    auto reply = QMessageBox::question(this,
        QStringLiteral("Message"),
        QStringLiteral("Are you sure to quit?"),
        QMessageBox::Yes|QMessageBox::No,
        QMessageBox::No);

    if (reply == QMessageBox::Yes)
        event->accept();
    else
        event->ignore();
}
