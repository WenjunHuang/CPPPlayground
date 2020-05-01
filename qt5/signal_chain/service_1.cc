//
// Created by rick on 2020/4/5.
//

#include <QTimer>
#include <chrono>
#include <QDebug>
#include <QEvent>
#include <QtCore/QtCore>
#include "service_1.h"

static int kService1EventType = QEvent::registerEventType();
class Service1Event : public QEvent {
  public:
    Service1Event(Service1Handler* handler):QEvent{(QEvent::Type)kService1EventType},handler_{handler}{}

    Service1Handler *handler_;

};
Service1* Service1::instance_ = nullptr;

Service1::Service1(){
    thread_ = new QThread(this);
    moveToThread(thread_);
    thread_->start();
}

void Service1::initialize() {
    if (instance_ == nullptr) {
        instance_ = new Service1();
    }
}

Service1* Service1::instance() {
    return instance_;
}

Service1Handler * Service1::doSomething() {
    auto handler = new Service1Handler();

    connect(handler,&Service1Handler::finished,handler,&QObject::deleteLater);

    // 5 m秒后结束
    QTimer::singleShot(std::chrono::seconds(1),[this,handler](){
        qDebug() << "Post Event";
        QCoreApplication::postEvent(this,new Service1Event(handler));
    });


    return handler;

}

bool Service1::event(QEvent* event) {
    if (event->type() == kService1EventType){
        qDebug("Service1Event in Thread:%d", QThread::currentThreadId());
        auto serviceEvent = dynamic_cast<Service1Event*>(event);
        emit serviceEvent->handler_->finished();
        return true;
    }

    return QObject::event(event);
}

Service1Handler::Service1Handler(QObject* parent):QObject{parent} {}

void Service1Handler::finish() {
    emit finished();
}

Service1Handler::~Service1Handler(){
    qDebug() << "destruct Service1Handler";
}