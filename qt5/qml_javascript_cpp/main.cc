//
// Created by rick on 2019/11/14.
//

#include <QDate>
#include <QDateTime>
#include <QGenericReturnArgument>
#include <QMetaObject>
#include <QQmlContext>
#include <QQuickItem>
#include <QVariant>
#include <QtGui/QGuiApplication>
#include <QtQml/QQmlApplicationEngine>
#include <QtQuick/QQuickView>

    struct Friend {
        Q_GADGET
        Q_PROPERTY(QString name MEMBER name)
        Q_PROPERTY(QVector<QString> hobbies MEMBER hobbies)
      public:
        QString name;
        QVector<QString> hobbies;
    };

    bool operator==(const Friend& first, const Friend& second) {
        return first.name == second.name && first.hobbies == second.hobbies;
    }

    struct Data {
      Q_GADGET
        Q_PROPERTY(QString name MEMBER name)
        Q_PROPERTY(QVector<QString> alias MEMBER alias)
        Q_PROPERTY(QVariantList friends MEMBER friends)
      public:
        QString name;
        QVector<QString> alias;
        QVariantList friends;
    };

Q_DECLARE_METATYPE(Friend)

Q_DECLARE_METATYPE(Data)
//Q_DECLARE_METATYPE(QVector<Friend>)

class DataMaker : public QObject {
    Q_OBJECT
  public:
    DataMaker(QObject* parent = nullptr) : QObject(parent) {
        QVariant f1 = QVariant::fromValue(Friend{"God", {"Make Human", "Play thunder"}});
        QVariant f2 = QVariant::fromValue(Friend{"Budda", {"Save People", "Teach"}});
        _data = {"HuangWenJun",
                 {"HWJ"},
                 {f1,f2}};
    }

    Q_INVOKABLE Data getData() { return _data; }

    Q_INVOKABLE QVector<Friend> friends() {
        return {{"God", {"Make Human", "Play thunder"}}};
//        return {"God","Budda"};
    }

  private:
    Data _data;
};


int main(int argc, char* argv[]) {
    //    qmlRegisterUncreatableType<Data>("Foo",1,0,"Data","Not creatable");
//    qmlRegisterType<DataMaker>();
    qmlRegisterUncreatableType<DataMaker>("Foo",1,0,"DataMaker","notl");
    qmlRegisterUncreatableType<Data>("Foo",1,0,"Data","notl");

    qRegisterMetaType<Friend>("Friend");
    qRegisterMetaType<QVector<Friend>>();
    qRegisterMetaType<Data>("Data");


    // Singleton Type

    QGuiApplication app(argc, argv);
    QQmlApplicationEngine engine;
    auto dataMaker = new DataMaker;
    engine.rootContext()->setContextProperty("dataMaker", dataMaker);
    engine.load(QUrl("qrc:example.qml"));
    if (engine.rootObjects().isEmpty()) {
        return -1;
    }

    auto root = engine.rootObjects().first();

    QVariantList list;
    list << 10 << QColor(Qt::green) << "bottles";

    QVariantMap map;
    map.insert("language", "QML");
    map.insert("released", QDate(2010, 9, 21));

    QMetaObject::invokeMethod(root, "readValues",
                              Q_ARG(QVariant, QVariant::fromValue(list)),
                              Q_ARG(QVariant, QVariant::fromValue(map)));

    QDateTime dateTime = QDateTime::currentDateTime();
    QVariant retValue;
    QMetaObject::invokeMethod(root, "readDate",
                              Q_RETURN_ARG(QVariant, retValue),
                              Q_ARG(QVariant, QVariant::fromValue(dateTime)));
    qDebug() << "Value returned from readDate():" << retValue.toDateTime();
}
#include "main.moc"