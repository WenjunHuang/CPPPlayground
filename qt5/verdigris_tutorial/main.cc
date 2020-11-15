//
// Created by rick on 2020/4/26.
//
#include "wobjectdefs.h"
#include <QCoreApplication>
#include <QObject>
#include <QThread>
#include <optional>
#include <variant>

class MyObject : public QObject {
    W_OBJECT(MyObject)
  public:
    void mySlot(const QString& name) { qDebug("Hello %s", qPrintable(name)); }
    W_SLOT(mySlot)

  public:
    void mySignal(const QString& name) W_SIGNAL(mySignal, name)
};

#include "wobjectimpl.h"
W_OBJECT_IMPL(MyObject)

void aaa(MyObject* obj1) {
    bool ok = true;
    ok      = ok &&
         QObject::connect(obj1, &MyObject::mySignal, obj1, &MyObject::mySlot);
}

class SlotTutorial : public QObject {
    W_OBJECT(SlotTutorial)

  protected:
    void protectedSlot() {}
    W_SLOT(protectedSlot)

  private:
    void privateSlot() {}
    W_SLOT(privateSlot)

  public:
    void overload() {}
    W_SLOT(overload, ())

    void overload(int) {}
    W_SLOT(overload, (int))

  private:
    void overload(double) {}
    W_SLOT(overload, (double))

    void overload(int, int) {}
    W_SLOT(overload, (int, int))
};

W_OBJECT_IMPL(SlotTutorial)

class SignalTutorial : public QObject {
    W_OBJECT(SignalTutorial)
  public:
    void sig1(int a, int b) W_SIGNAL(sig1, a, b)

        void sig2(int a, int b) W_SIGNAL(sig2, a, b)

            void overload(int a, int b) W_SIGNAL(overload, (int, int), a, b)
};

W_OBJECT_IMPL(SignalTutorial)

class InvokableTutorial {
    W_GADGET(InvokableTutorial)
  public:
    void myInvokable() {}
    W_INVOKABLE(myInvokable)

    InvokableTutorial(int, int) {}
    W_CONSTRUCTOR(int, int)

    InvokableTutorial(void*, void* = nullptr) {}
    W_CONSTRUCTOR(void*, void*)
    W_CONSTRUCTOR(void*)
};

W_GADGET_IMPL(InvokableTutorial)

#include <QtCore/QMap>
class PropertyTutorial : public QObject {
    W_OBJECT(PropertyTutorial)

  public:
    QString m_value;
    QString value() const { return m_value; }
    void setValue(const QString& value) {
        m_value = value;
        emit valueChanged();
    }

    void valueChanged() W_SIGNAL(valueChanged)

        W_PROPERTY(QString, prop1 READ value WRITE setValue NOTIFY valueChanged)
            W_PROPERTY(QString, prop2, &PropertyTutorial::value,
                       &PropertyTutorial::setValue, W_Notify,
                       &PropertyTutorial::valueChanged)

        // By member
        W_PROPERTY(QString, prop3 MEMBER m_value NOTIFY valueChanged)
            W_PROPERTY(QString, prop4, &PropertyTutorial::m_value, W_Notify,
                       &PropertyTutorial::valueChanged)

                QMap<int, int> m_map;
    W_PROPERTY((QMap<int, int>), map MEMBER m_map)
};

W_OBJECT_IMPL(PropertyTutorial)

class EnumTutorial {
    W_GADGET(EnumTutorial)
  public:
    enum MyEnum { Blue, Red, Green, Yellow = 45, Violet = Blue + Green * 3 };
    W_ENUM(MyEnum, Blue, Red, Green, Yellow)
};

W_GADGET_IMPL(EnumTutorial)

struct CustomType1 {};
struct CustomType2 {};
struct CustomType3 {};
W_REGISTER_ARGTYPE(CustomType1)
W_REGISTER_ARGTYPE(CustomType1*)
W_REGISTER_ARGTYPE(CustomType2)

// try template
template <typename T> using Response = std::variant<T, int>;
W_REGISTER_ARGTYPE(Response<QString>)

class ArgTypes : public QObject {
    W_OBJECT(ArgTypes)
  public:
    void slot1(CustomType1, CustomType2) {}
    W_SLOT(slot1)

    void slot2(CustomType1*, CustomType2*) {}
    W_SLOT(slot2, (CustomType1*, CustomType2*))
};

W_OBJECT_IMPL(ArgTypes)

#include <QtCore/QDebug>
template <typename T> class MyTemplate : public QObject {
    W_OBJECT(MyTemplate)
  public:
    void slot(T t) { qDebug() << "templated slot" << t; }
    W_SLOT(slot)

    void signal(T t) W_SIGNAL(signal, t)
};

W_OBJECT_IMPL((MyTemplate<T>), template <typename T>)

template <typename A, typename B> class MyTemplate2 : public QObject {
    W_OBJECT(MyTemplate2)
};

W_OBJECT_IMPL((MyTemplate2<A, B>), template <typename A, typename B>)

void temp1() {
    bool ok = true;
    MyTemplate<QString> obj;
    ok = ok && QObject::connect(&obj, &MyTemplate<QString>::signal, &obj,
                                &MyTemplate<QString>::slot);
    Q_ASSERT(ok);
    emit obj.signal("Hallo");
}

template <typename A> class ResponseTemplate : public QObject {
    W_OBJECT(ResponseTemplate)
  public:
    void slot(const Response<A>& value) {
        qDebug("slot in thread: %d, pointer: %p",QThread::currentThreadId(),&value);
    }

    W_SLOT(slot, (const Response<A>&))

    void signal(const Response<A>& value) W_SIGNAL(signal, value)
};
W_OBJECT_IMPL((ResponseTemplate<A>), template <typename A>)

class MyHandler : public QObject {
    Q_OBJECT
  public slots:
    void somethingFinished(const std::variant<QString, int>& result) {
        qDebug("finshed in thread: %d,value pointer: %p", QThread::currentThreadId(),&result);
    }
};
int main(int argc, char* argv[]) {
    QCoreApplication app{argc, argv};
    qRegisterMetaType<Response<QString>>("Response<QString>");
    qDebug() << QMetaType::type("Response<QString>");
    ResponseTemplate<QString> response;
    QObject::connect(&response, &ResponseTemplate<QString>::signal, &response,
                     &ResponseTemplate<QString>::slot);

    QThread *thread1 = new QThread;
    thread1->start();
    MyHandler *handler = new MyHandler;
    handler->moveToThread(thread1);
    QObject::connect(&response,&ResponseTemplate<QString>::signal,handler,&MyHandler::somethingFinished);

    emit response.signal("HELLO");
    emit response.signal(0);

    qDebug() << ResponseTemplate<QString>::staticMetaObject.className();

    return app.exec();
}
#include "main.moc"
