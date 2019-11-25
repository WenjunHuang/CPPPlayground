//
// Created by rick on 2019/11/14.
//

#include <QDate>
#include <QDateTime>
#include <QDir>
#include <QGenericReturnArgument>
#include <QMetaObject>
#include <QQmlContext>
#include <QQuickItem>
#include <QRandomGenerator>
#include <QVariant>
#include <QtGui/QGuiApplication>
#include <QtQml/QQmlApplicationEngine>
#include <QtQuick/QQuickView>
#include <variant>

QVector<QString> kTexts{
    QStringLiteral(u"每个人都有潜在的能量，只是很容易：被习惯所掩盖，被时间所迷"
                   u"离，被惰性所消磨。"),
    QStringLiteral(u"每个人都有潜在的能量，只是很容易：被习惯所掩盖，被时间所迷"
                   u"离，被惰性所消磨。"),
    QStringLiteral(u"无论你觉得自己多么的不幸，永远有人比你更加不幸。")
};
QString kSetuDir = "/Users/huangwenjun/Desktop/setu";

class Helper:public QObject {
    Q_OBJECT
  public:
    enum class StimulateType { SeTu, Text };
    Q_ENUM(StimulateType)
};

//Q_DECLARE_METATYPE(StimulateType)

struct SeTuStimulate {
    Q_GADGET
    Q_PROPERTY(QUrl setuUrl MEMBER setuUrl)
    Q_PROPERTY(Helper::StimulateType type READ type)
  public:
    QUrl setuUrl;
    Helper::StimulateType type() const { return Helper::StimulateType::SeTu; }
};

Q_DECLARE_METATYPE(SeTuStimulate)
bool operator==(const SeTuStimulate& first ,const SeTuStimulate& other) {
    return first.setuUrl == other.setuUrl;
}

struct TextStimulate {
    Q_GADGET
    Q_PROPERTY(QString text MEMBER text)
    Q_PROPERTY(Helper::StimulateType type READ type)
  public:
    QString text;
    Helper::StimulateType type() const { return Helper::StimulateType::Text; }
};

bool operator==(const TextStimulate& first,const TextStimulate& other) { return first.text == other.text; }

Q_DECLARE_METATYPE(TextStimulate)

class SetuViewModel : public QObject {
    Q_OBJECT
    Q_PROPERTY(QVariant stimulate READ stimulate NOTIFY stimulateChanged)
    Q_PROPERTY(QVector<SeTuStimulate> allSetu READ allSetu)

  public:
    explicit SetuViewModel(QObject* parent = nullptr) : QObject(parent) {
        randomStimulate();
    }

    Q_INVOKABLE void randomStimulate() {
        if (auto type = QRandomGenerator::global()->bounded(2); type == 0) {
            // setu
            QDir dir(kSetuDir);
            auto setuList = dir.entryList(QDir::Files);
            auto pick = QRandomGenerator::global()->bounded(setuList.size());
            QUrl setuUrl = QUrl::fromLocalFile(dir.absoluteFilePath(setuList.at(pick)));
            qDebug() << setuUrl;

            setStimulate(SeTuStimulate{setuUrl});
        } else {
            // text
            auto pick = QRandomGenerator::global()->bounded(kTexts.size());
            auto text = kTexts.at(pick);
            qDebug() << text;

            setStimulate(TextStimulate{text});
        }
    }

    QVector<SeTuStimulate> allSetu() const {
        QVector<SeTuStimulate> result;

        QDir dir(kSetuDir);
        auto setuList = dir.entryList(QDir::Files);
        for (const auto& file: setuList) {
            QUrl setuUrl = QUrl::fromLocalFile(dir.absoluteFilePath(file));
            result.append(SeTuStimulate{setuUrl});
        }

        return result;
    }

    QVariant stimulate() const {
        return QVariant::fromStdVariant(_stimulate);
//        return std::visit(_stimulate,[](auto &value) {
//            if constexpr (std::is_convertible_v< decltype(value),SeTuStimulate>) {
//
//            } else if (std::is_convertible_v< decltype(value),TextStimulate>) {
//
//            } else {
//                return QVariant();
//            }
//        });
    }

  signals:
    void stimulateChanged();

  public slots:
    void
    setStimulate(const std::variant<SeTuStimulate, TextStimulate>& stimulate) {
        if (_stimulate == stimulate)
            return;

        _stimulate = stimulate;
        emit stimulateChanged();
    }

  private:
    std::variant<SeTuStimulate, TextStimulate> _stimulate;
};

int main(int argc, char* argv[]) {

    // Singleton Type
    qmlRegisterUncreatableType<Helper>("ViewModel",1,0,"Helper","Helper not creatable");
    qmlRegisterType<SetuViewModel>("ViewModel",1,0,"SetuViewModel");
    qRegisterMetaType<SeTuStimulate>();
    qRegisterMetaType<TextStimulate>();
//    qRegisterMetaType<StimulateType>();

    QGuiApplication app(argc, argv);
    QQmlApplicationEngine engine;
    engine.load(QUrl("qrc:variant_state.qml"));
    if (engine.rootObjects().isEmpty()) {
        return -1;
    }

    return app.exec();
}

#include "variant_state.moc"