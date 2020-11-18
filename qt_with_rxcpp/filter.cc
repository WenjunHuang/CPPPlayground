//
// Created by HUANG WEN JUN on 2020/11/18.
//
#include <QtWidgets>
#include <QApplication>
#include "rx_eventfilter.h"

int main(int argc,char *argv[]){
    QApplication app{argc,argv};

    auto widget = new QWidget();
    widget->resize(350,300);
    widget->setCursor(Qt::OpenHandCursor);

    auto xDock = new QLabel(widget);
    xDock->setStyleSheet(QStringLiteral("QLabel{background-color:red;}"));
    xDock->resize(9,9);
    xDock->setGeometry(0,0,9,9);

    auto yDock = new QLabel(widget);
    yDock->setStyleSheet(QStringLiteral("QLabel{background-color:blue;}"));
    yDock->resize(9,9);
    yDock->setGeometry(0,0,9,9);


    rxcpp::composite_subscription subs;
    rxevt::from(widget,QEvent::MouseButtonPress)
    .filter([](const QEvent* e){
            auto me = static_cast<const QMouseEvent*>(e);
            return (Qt::LeftButton == me->buttons());
        })
    .subscribe(subs,[&](const QEvent* e){
            auto me = static_cast<const QMouseEvent*>(e);
            widget->setCursor(Qt::ClosedHandCursor);
            xDock->move(me->x(),0);
            yDock->move(0,me->y());
        });
    rxevt::from(widget,QEvent::MouseMove)
        .filter([](const QEvent* e){
          auto me = static_cast<const QMouseEvent*>(e);
          return (Qt::LeftButton == me->buttons());
        })
        .subscribe(subs,[&](const QEvent* e){
          auto me = static_cast<const QMouseEvent*>(e);
          widget->setCursor(Qt::ClosedHandCursor);
          xDock->move(me->x(),0);
          yDock->move(0,me->y());
        });

    rxevt::from(widget,QEvent::MouseButtonRelease)
        .subscribe([widget](const QEvent* e){
            widget->setCursor(Qt::OpenHandCursor);
        });

    widget->show();
    return app.exec();
}
