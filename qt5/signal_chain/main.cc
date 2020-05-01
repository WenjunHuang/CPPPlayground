//
// Created by rick on 2020/4/5.
//

#include "event_handler.h"
#include "service_1.h"
#include <QApplication>
#include <QDebug>
#include <QThread>
int main(int argc, char* argv[]) {
    QApplication app{argc, argv};

    Service1::initialize();

    qDebug("MainThread: %d", QThread::currentThreadId());
    auto service1 = Service1::instance();

    auto handler         = service1->doSomething();
    auto finishedHandler = new FinishedHandler();
    QThread* thread = new QThread;
    finishedHandler->moveToThread(thread);
    thread->start();

    QObject::connect(handler, &Service1Handler::finished, [=]() {
        qDebug("finished 1 in Thread:%d", QThread::currentThreadId());
        auto handler2 = service1->doSomething();

        QObject::connect(handler2, QOverload<>::of(&Service1Handler::finished),
                         finishedHandler,
                         QOverload<>::of(&FinishedHandler::handleFinished));
    });

    return app.exec();
}