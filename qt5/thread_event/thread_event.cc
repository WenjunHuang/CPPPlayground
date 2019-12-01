//
// Created by HUANG WEN JUN on 2019/11/27.
//
#include <thread>
#include <QtCore>
#include <iostream>
#include <QtWidgets>

int kMyFooEventType = QEvent::registerEventType();

class MyFooEvent : public QEvent {
  public:
    MyFooEvent():QEvent((QEvent::Type)kMyFooEventType){}
};

class WorkerEvent : public QObject {
    Q_OBJECT
  public:
    WorkerEvent() {}

    bool eventFilter(QObject* obj, QEvent* ev) override {
        if (ev->type() == kMyFooEventType) {
            auto threadId = std::this_thread::get_id();
            std::cout << "thread id:" << threadId << "got foo event" << std::endl;
            return true;
        }

        return QObject::eventFilter(obj,ev);
    }

    bool event(QEvent* ev) override {
        if (ev->type() == kMyFooEventType) {
            auto threadId = std::this_thread::get_id();
            std::cout << "thread id:" << threadId << "got foo event" << std::endl;
            return true;
        }

        return QObject::event(ev);

    }
};

int main(int argc, char* argv[]) {
    QApplication app(argc, argv);
    auto threadId = std::this_thread::get_id();
    std::cout << "thread id:" << threadId << " main thread" << std::endl;

    QThread* thread = new QThread;
    QObject* worker = new WorkerEvent;
//    worker->installEventFilter(new WorkerEvent);

//    auto filter = new WorkerEvent;
    worker->moveToThread(thread);
    thread->start();

    QPushButton *button = new QPushButton("Push me");
    QObject::connect(button,&QPushButton::clicked,[](){
      auto threadId = std::this_thread::get_id();
      std::cout << "thread id:" << threadId << " clickIn" << std::endl;
    });
    button->show();
    QCoreApplication::postEvent(worker,new MyFooEvent);

    return app.exec();
}
#include "thread_event.moc"