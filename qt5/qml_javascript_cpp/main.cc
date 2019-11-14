//
// Created by rick on 2019/11/14.
//

#include <QDate>
#include <QMetaObject>
#include <QQuickItem>
#include <QtGui/QGuiApplication>
#include <QtQml/QQmlApplicationEngine>
#include <QtQuick/QQuickView>
#include <QGenericReturnArgument>
#include <QVariant>
#include <QDateTime>
struct Data {
    Q_GADGET
    Q_PROPERTY(QString name MEMBER name)
    Q_PROPERTY(QVector<QString> friends MEMBER friends)
  public:
    QString name;
    QVector<QString> friends;
};

int main(int argc, char* argv[]) {
    QGuiApplication app(argc, argv);
    QQmlApplicationEngine engine;
    QQuickView view(QUrl("qrc:example.qml"));

    QVariantList list;
    list << 10 << QColor(Qt::green) << "bottles";

    QVariantMap map;
    map.insert("language", "QML");
    map.insert("released", QDate(2010, 9, 21));

    QMetaObject::invokeMethod(view.rootObject(), "readValues",
                              Q_ARG(QVariant, QVariant::fromValue(list)),
                              Q_ARG(QVariant, QVariant::fromValue(map)));

    QDateTime dateTime = QDateTime::currentDateTime();
    QVariant retValue;
    QMetaObject::invokeMethod(view.rootObject(), "readDate",
                              Q_RETURN_ARG(QVariant, retValue),
                              Q_ARG(QVariant, QVariant::fromValue(dateTime)));
    qDebug() << "Value returned from readDate():" << retValue.toDateTime();
}