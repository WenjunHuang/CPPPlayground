
#include "clock_circle.h"
#include <QtGui/QGuiApplication>
#include <QtQml/QQmlApplicationEngine>
#include <QtQml/qqml.h>

int main(int argc,char * argv[]){
    QGuiApplication app(argc,argv);
    qmlRegisterType<ClockCircle>("ClockCircle",1,0,"ClockCircle");

    QQmlApplicationEngine engine;
    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));

    return app.exec();
}