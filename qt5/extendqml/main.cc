//
// Created by rick on 2019/11/13.
//
#include <QtCore>
#include <QtGui>
#include <QtQml/QQmlComponent>
#include <QtQml/QQmlEngine>
#include <QtQml/QQmlListProperty>
#include <QtQml/QQmlContext>

class ShoeDescription : public QObject {
    Q_OBJECT
    Q_PROPERTY(int size READ size WRITE setSize)
    Q_PROPERTY(QColor color READ color WRITE setColor)
    Q_PROPERTY(QString brand READ brand WRITE setBrand)
    Q_PROPERTY(float price READ price WRITE setPrice)
  public:
    explicit ShoeDescription(QObject* parent = nullptr) : QObject(parent) {}

    QString brand() const { return _brand; }

    void setBrand(QString brand) { _brand = brand; }

    float price() const { return _price; }

    void setPrice(float price) { _price = price; }

    int size() const { return _size; }

    void setSize(int size) { _size = size; }

    QColor color() const { return _color; }

    void setColor(QColor color) { _color = color; }

  private:
    int _size;
    QColor _color;
    QString _brand;
    float _price;
};

class Person : public QObject {
    Q_OBJECT
    Q_PROPERTY(QString name READ name WRITE setName)
    Q_PROPERTY(ShoeDescription* shoe READ shoe WRITE setShoe)
    Q_PROPERTY(QString favor READ favor WRITE setFavor)
  public:
    explicit Person(QObject* parent = nullptr) : QObject(parent) {
        _shoe = new ShoeDescription(this);
        _shoe->setSize(0);
        _shoe->setColor(Qt::white);
        _shoe->setPrice(0.0f);
        _shoe->setBrand("");
    }

    QString favor() const { return _favor; }

    void setFavor(const QString& favor) { _favor = favor; }

    QString name() const { return _name; }

    void setName(const QString& name) { _name = name; }

    ShoeDescription* shoe() const { return _shoe; }

    void setShoe(ShoeDescription* shoe) {
        if (_shoe == shoe)
            return;

        if (_shoe)
            _shoe->deleteLater();

        _shoe = shoe;
        if (_shoe)
            _shoe->setParent(this);
    }

  private:
    QString _name;
    ShoeDescription* _shoe;
    QString _favor;
};

class Boy : public Person {
    Q_OBJECT
  public:
    Boy(QObject* parent = nullptr) : Person(parent) {}
};
class Girl : public Person {
    Q_OBJECT
  public:
    Girl(QObject* parent = nullptr) : Person(parent) {}
};

enum class BirthdayPartySize { Small, Medium, Large };

enum class BirthdayPartyTheme { Classic, Modern, Future };

enum class FuckType {
    A,
    B
};
struct FuckTypeA {
    Q_GADGET
    Q_PROPERTY(QString name MEMBER name)
    Q_PROPERTY(FuckType type READ type)
  public:
    QString name;
    FuckType type () const {
        return FuckType::A;
    }
};
struct FuckTypeB {
  Q_GADGET
    Q_PROPERTY(QString name MEMBER name)
    Q_PROPERTY(FuckType type READ type)
  public:
    QString name;
    FuckType type () const {
        return FuckType::B;
    }
};

Q_DECLARE_METATYPE(FuckTypeA)
Q_DECLARE_METATYPE(FuckTypeB)


class BirthdayPartyAttached : public QObject {
    Q_OBJECT
    Q_PROPERTY(QDate rsvp READ rsvp WRITE setRsvp)
  public:
    BirthdayPartyAttached(QObject* object) : QObject(object) {}
    QDate rsvp() const { return _rsvp; }

    void setRsvp(const QDate& rsvp) { _rsvp = rsvp; }

  private:
    QDate _rsvp;
};
class BirthdayParty : public QObject {
    Q_OBJECT
    Q_CLASSINFO("DefaultProperty", "guests")
    Q_PROPERTY(QQmlListProperty<Person> guests READ guests)
    Q_PROPERTY(Person* host READ host WRITE setHost)
    Q_PROPERTY(QVariant fuck READ fuck)

    Q_ENUM(BirthdayPartySize)
    Q_ENUM(BirthdayPartyTheme)
    Q_ENUM(FuckType)

  public:
    explicit BirthdayParty(QObject* parent = nullptr) : QObject(parent) {}

    Q_INVOKABLE void invite(const QString& name, bool boy) {
        Person* person = nullptr;
        if (boy)
            person = new Boy;
        else
            person = new Girl;
        person->setName(name);
    }

    Person* host() const { return _host; }

    void setHost(Person* host) { _host = host; }

    QQmlListProperty<Person> guests() {
        return QQmlListProperty<Person>(
            this, this, &BirthdayParty::appendGuest, &BirthdayParty::guestCount,
            &BirthdayParty::guest, &BirthdayParty::clearGuests);
    }

    void appendGuest(Person* person) { _guests.append(person); }

    int guestCount() const { return _guests.size(); }

    Person* guest(int index) { return _guests.at(index); }

    void clearGuests() { _guests.clear(); }

    QVariant fuck() const {
//        return "foo";
        return QVariant::fromValue(Fuck{"Wenjun Huang"});
    }


    static BirthdayPartyAttached* qmlAttachedProperties(QObject* object) {
        return new BirthdayPartyAttached(object);
    }

  private:
    static void appendGuest(QQmlListProperty<Person>* property,
                            Person* person) {
        reinterpret_cast<BirthdayParty*>(property->data)->appendGuest(person);
    }

    static int guestCount(QQmlListProperty<Person>* property) {
        return reinterpret_cast<BirthdayParty*>(property->data)->guestCount();
    }

    static Person* guest(QQmlListProperty<Person>* property, int index) {
        return reinterpret_cast<BirthdayParty*>(property->data)->guest(index);
    }

    static void clearGuests(QQmlListProperty<Person>* property) {
        reinterpret_cast<BirthdayParty*>(property->data)->clearGuests();
    }

    Person* _host;
    QVector<Person*> _guests;
};

QML_DECLARE_TYPEINFO(BirthdayParty, QML_HAS_ATTACHED_PROPERTIES)

int main(int argc, char* argv[]) {
    QCoreApplication app(argc, argv);

    qRegisterMetaType<Fuck>();
    qmlRegisterType<BirthdayPartyAttached>();
    qmlRegisterType<BirthdayParty>("People", 1, 0, "BirthdayParty");
    qmlRegisterType<Person>("People", 1, 0, "Person");
    qmlRegisterType<Boy>("People", 1, 0, "Boy");
    qmlRegisterType<Girl>("People", 1, 0, "Girl");
    qmlRegisterType<ShoeDescription>("People", 1, 0, "ShoeDescription");

    QQmlEngine engine;
    QQmlComponent component(&engine, QUrl("qrc:example.qml"));

    QVariantMap food ; // qml engine will convert QVariantMap to javascript object
    food.insert("china",QStringLiteral("guangdong dish"));
    food.insert("japan",QStringLiteral("soushi"));
    engine.rootContext()->setContextProperty("food",food);

    auto party = qobject_cast<BirthdayParty*>(component.create());

    if (party && party->host()) {
        qDebug() << party->host()->name() << "is having a birthday!";
        if (qobject_cast<Boy*>(party->host()))
            qDebug() << "He is inviting:";
        else
            qDebug() << "She is inviting:";

        for (auto i = 0; i < party->guestCount(); i++) {
            auto guest = party->guest(i);
            QDate rsvpDate;
            auto attached =
                qmlAttachedPropertiesObject<BirthdayParty>(guest, false);
            if (attached)
                rsvpDate = attached->property("rsvp").toDate();

            if (rsvpDate.isNull())
                qDebug() << "    " << guest->name()
                         << "RSVP date: Hasn't RSVP'd";
            else
                qDebug() << "    " << guest->name()
                         << "RSVP date:" << rsvpDate.toString();

            if (qobject_cast<Boy*>(guest))
                qDebug() << "He like " << guest->favor() << " food.";
            else
            qDebug() << "She like " << guest->favor() << " food.";
        }
    } else {
        qDebug() << component.errors();
    }
}

#include "main.moc"