//
// Created by rick on 2020/4/30.
//
#include <QDebug>
#include <QMetaMethod>
#include <QObject>
#include <QString>
#include <type_traits>

#ifndef Q_MOC_RUN
#define MY_CUSTOM_TAG
#endif
class Base : public QObject {
    Q_OBJECT
    Q_PROPERTY(QString name MEMBER name_)
  public:
    Q_INVOKABLE Base(QObject* parent1 = nullptr) : QObject{parent1} {}

    enum MyEnum { A = 100, B, C };

    enum class MyScopedEnum:int { D = 200, E, F };
    Q_ENUM(MyEnum)
    Q_ENUM(MyScopedEnum)
  private slots:
    void someSignal() {}
  public slots:
    void foo(int a,float b,const QString& c) {}
    MY_CUSTOM_TAG void foo2(double a){}

  private:
    QString name_;
  signals:
    void someSignal(int a,int b);
};

class Derived : public Base {
    Q_OBJECT
  public:
    Q_INVOKABLE void bar() {}
};

void printEnumerator(const QMetaEnum& me, int indent) {
    QString strIndent(indent, ' ');
    QString subIndent(indent + 1, ' ');
    qDebug() << strIndent << me.name();
    qDebug() << subIndent << "isScoped:" << me.isScoped();
    if (me.isScoped())
        qDebug() << subIndent << "scope:" << me.scope();
    qDebug() << subIndent << "isFlag:" << me.isFlag();
    qDebug() << subIndent << "isValid:" << me.isValid();
    qDebug() << subIndent << "keyCount:" << me.keyCount();
    for (auto i = 0;i<me.keyCount();i++) {
        qDebug() << subIndent << ' ' << "key: " << me.valueToKey(me.value(i)) << ",value: " << me.value(i);
    }
}

void printProperty(const QMetaProperty& property, int indent) {
    QString strIndent(indent, ' ');
    QString subIndent(indent + 1, ' ');
    qDebug() << strIndent << property.name();
    qDebug() << subIndent << "propertyIndex:" << property.propertyIndex();
    qDebug() << subIndent
             << "relativePropertyIndex:" << property.relativePropertyIndex();
    qDebug() << subIndent << "type:" << property.typeName();
    qDebug() << subIndent << "hasNotifySignal: " << property.hasNotifySignal();
    qDebug() << subIndent << "isConstant:" << property.isConstant();
    qDebug() << subIndent << "isEnumType:" << property.isEnumType();
    qDebug() << subIndent << "isFinal:" << property.isFinal();
    qDebug() << subIndent << "isFlagType:" << property.isFlagType();
    qDebug() << subIndent << "isReadable:" << property.isReadable();
    qDebug() << subIndent << "isResettable:" << property.isResettable();
    qDebug() << subIndent << "isValid:" << property.isValid();
    qDebug() << subIndent << "isWritable:" << property.isWritable();
    qDebug() << subIndent << "notifySignal:"
             << QString::fromLatin1(property.notifySignal().methodSignature());
    qDebug() << subIndent
             << "notifySignalIndex:" << property.notifySignalIndex();
}

template <typename T>
std::enable_if_t<std::is_base_of_v<QObject, T>> printMetaData() {
    auto mo = &T::staticMetaObject;
    qDebug("class name:%s", mo->className());
//    qDebug() << "revision:" << mo->re
    qDebug() << "super class name: " << mo->superClass()->className();
    qDebug("method count: %d", mo->methodCount());
    for (auto i = 0; i < mo->methodCount(); i++) {
        auto m = mo->method(i);
        qDebug() << "\tmethod index:" << i << ",method name: "
                 << QString::fromLatin1(m.methodSignature());
    }

    qDebug("constructor count: %d", mo->constructorCount());
    for (auto i = 0; i < mo->constructorCount(); i++) {
        auto c = mo->constructor(i);
        qDebug() << "\tconstructor index:" << i << ",constructor name: "
                 << QString::fromLatin1(c.methodSignature());
    }

    qDebug("property count: %d", mo->propertyCount());
    for (auto i = 0; i < mo->propertyCount(); i++) {
        printProperty(mo->property(i), 1);
    }

    qDebug("enumerator count: %d", mo->enumeratorCount());
    for (auto i = 0; i < mo->enumeratorCount(); i++) {
        printEnumerator(mo->enumerator(i), 1);
    }
}

int main() {
    printMetaData<Base>();
    printMetaData<Derived>();
}

#include "meta_object.moc"