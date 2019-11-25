//
// Created by rick on 2019/11/14.
//

#include <QDate>
#include <QDateTime>
#include <QGenericReturnArgument>
#include <QMetaObject>
#include <QQmlContext>
#include <QQuickItem>
#include <QRandomGenerator>
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

Q_DECLARE_METATYPE(Friend)

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

Q_DECLARE_METATYPE(Data)
// Q_DECLARE_METATYPE(QVector<Friend>)

QColor randomColor() {
    auto rg = QRandomGenerator::global();
    auto r  = rg->bounded(256);
    auto g  = rg->bounded(256);
    auto b  = rg->bounded(256);

    return {r, g, b};
}

class DataMaker : public QObject {
    Q_OBJECT
    Q_PROPERTY(QVariantList colors READ colors NOTIFY colorsChanged)

  public:
    DataMaker(QObject* parent = nullptr) : QObject(parent) {}

    Q_INVOKABLE void resetColors() {
        auto genCount = QRandomGenerator::global()->bounded(1000);
        _data.clear();
        for (auto i = 0; i < genCount; i++)
            _data.append(randomColor());

        emit colorsChanged();
    }

    QVariantList colors() const { return _data; }

  signals:
    void colorsChanged();

  private:
    QVariantList _data;
};

class Theme : public QObject {
    Q_OBJECT
    Q_PROPERTY(QColor primaryColor READ primaryColor WRITE setPrimaryColor NOTIFY primaryColorChanged)
    Q_PROPERTY(int spacing READ spacing  WRITE setSpacing NOTIFY spacingChanged)

  public:
    Theme(QObject* parent = nullptr)
        : QObject(parent), _primaryColor{Qt::blue}, _spacing{8} {}

    QColor primaryColor() const { return _primaryColor; }

    void setPrimaryColor(QColor color) {
        if (color != _primaryColor) {
            _primaryColor = color;
            emit primaryColorChanged();
        }
    }

    int spacing() const { return _spacing; }
    void setSpacing(int spacing) {
        if (spacing != _spacing) {
            _spacing = spacing;
            emit spacingChanged();
        }
    }

  signals:
    void primaryColorChanged();
    void spacingChanged();

public:
    static QObject* makeSingleton(QQmlEngine* qmlEngine, QJSEngine* jsEngine) {
        Q_UNUSED(qmlEngine)
        Q_UNUSED(jsEngine)

        return new Theme;
    }

  private:
    QColor _primaryColor;
    int _spacing;
};

int main(int argc, char* argv[]) {
    //    qmlRegisterUncreatableType<Data>("Foo",1,0,"Data","Not creatable");
    qmlRegisterType<DataMaker>();
    qmlRegisterSingletonType<Theme>("Foo", 1, 0, "Theme",
                                    &Theme::makeSingleton);

    //    qRegisterMetaType<Friend>();
    qRegisterMetaType<QVector<Friend>>();
    qRegisterMetaType<Data>();

    QGuiApplication app(argc, argv);
    QQmlApplicationEngine engine;
    DataMaker* maker = new DataMaker;
    maker->resetColors();
    engine.rootContext()->setContextProperty("colorMaker", maker);
    engine.load(QUrl("qrc:example.qml"));

    return app.exec();
}
#include "main.moc"
