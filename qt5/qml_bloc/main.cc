#include "foo.h"
#include "http.h"
#include <QApplication>
#include <QQmlApplicationEngine>
#include <QWidget>

int main(int argc, char* argv[]) {
    QGuiApplication app(argc, argv);
    QQmlApplicationEngine engine;
    qmlRegisterType<Foo>("Foo", 1, 0, "Foo");

    auto http = new Http();
    auto factory = new ExportedFactory();
    engine.rootContext()->setContextProperty("http",http);
    engine.rootContext()->setContextProperty("exportedFactory",factory);

    engine.load(QUrl("./async.qml"));

    return app.exec();
}
