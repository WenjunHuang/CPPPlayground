//
// Created by HUANG WEN JUN on 2020/11/18.
//
#include "rx_eventfilter.h"
#include <QApplication>
#include <QtWidgets>

int main(int argc, char* argv[]) {
    QApplication app(argc, argv);
    auto widget = std::unique_ptr<QWidget>(new QWidget());
    widget->resize(280, 200);
    auto labelMouseArea = new QLabel("Mouse Area");
    labelMouseArea->setMouseTracking(true);
    labelMouseArea->setAlignment(Qt::AlignCenter | Qt::AlignHCenter);
    labelMouseArea->setFrameStyle(2);

    auto labelCoordinates = new QLabel("X=0, Y=0");
    labelCoordinates->setAlignment(Qt::AlignCenter | Qt::AlignHCenter);
    labelCoordinates->setFrameStyle(2);

    labelMouseArea->setSizePolicy(QSizePolicy::Expanding,
                                  QSizePolicy::Expanding);
    labelCoordinates->setSizePolicy(QSizePolicy::Expanding,
                                    QSizePolicy::Expanding);

    auto layout = new QVBoxLayout;
    layout->addWidget(labelMouseArea);
    layout->addWidget(labelCoordinates);
    layout->setStretch(0, 4);
    layout->setStretch(1, 1);
    widget->setLayout(layout);

    rxevt::from(labelMouseArea, QEvent::MouseMove)
        .subscribe([labelCoordinates](const QEvent* e) {
            auto me = static_cast<const QMouseEvent*>(e);
            labelCoordinates->setText(QString("Mouse Moving: X = %1, Y = %2")
                                          .arg(me->x())
                                          .arg(me->y()));
        });
    rxevt::from(labelMouseArea, QEvent::MouseButtonPress)
        .subscribe([labelCoordinates](const QEvent* e) {
          auto me = static_cast<const QMouseEvent*>(e);
          labelCoordinates->setText(QString("Mouse Single click at X = %1, Y = %2")
                                        .arg(me->x())
                                        .arg(me->y()));
        });
    rxevt::from(labelMouseArea, QEvent::MouseButtonDblClick)
        .subscribe([labelCoordinates](const QEvent* e) {
          auto me = static_cast<const QMouseEvent*>(e);
          labelCoordinates->setText(QString("Mouse Double click at X = %1, Y = %2")
                                        .arg(me->x())
                                        .arg(me->y()));
        });

    widget->show();
    return app.exec();
}