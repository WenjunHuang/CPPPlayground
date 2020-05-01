//
// Created by rick on 2020/4/25.
//

#include "event_handler.h"
void FinishedHandler::handleFinished() {
    qDebug("finished FinishedHandler in Thread:%d", QThread::currentThreadId());
    qApp->exit();
}
FinishedHandler::FinishedHandler(QObject* parent) : QObject(parent) {}
