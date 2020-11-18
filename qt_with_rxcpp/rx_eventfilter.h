//
// Created by HUANG WEN JUN on 2020/11/18.
//

#ifndef CPPPLAYGROUND_RX_EVENTFILTER_H
#define CPPPLAYGROUND_RX_EVENTFILTER_H
#include <rx.hpp>
#include <QObject>
#include <QEvent>

namespace rxevt {
    class EventEater : public QObject {
      private:
        QEvent::Type eventType;
        rxcpp::subscriber<QEvent*> eventSubscriber;
      public:
        EventEater(QObject* parent,QEvent::Type type,rxcpp::subscriber<QEvent*> s):QObject{parent},eventType{type} ,eventSubscriber{s}{}
        ~EventEater(){
            eventSubscriber.on_completed();
        }

        bool eventFilter(QObject* obj,QEvent *event){
            if (event->type() == eventType){
                eventSubscriber.on_next(event);
            }
            return QObject::eventFilter(obj,event);
        }
    };

    rxcpp::observable<QEvent*> from(QObject* qobject,QEvent::Type type) {
        if (!qobject)return rxcpp::sources::never<QEvent*>();

        return rxcpp::observable<>::create<QEvent*>([qobject,type](rxcpp::subscriber<QEvent*> s){
            qobject->installEventFilter(new EventEater(qobject,type,s));
        });
    }
}

#endif // CPPPLAYGROUND_RX_EVENTFILTER_H
