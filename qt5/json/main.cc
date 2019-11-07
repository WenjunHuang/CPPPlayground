#include <QDebug>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <iostream>
#include <optional>

template <typename T> T fromJsonValue(const QJsonValue& value) {
    return T::fromJsonValue(value);
}

template <> QString fromJsonValue(const QJsonValue& value) {
    return value.toString();
}

template <> int fromJsonValue(const QJsonValue& value) { return value.toInt(); }

template <typename T> QVector<T> fromJsonArray(const QJsonValue& value) {
    auto array = value.toArray();
    QVector<T> result;
    for (const auto& v : array) {
        result.append(::fromJsonValue<T>(v));
    }
    return result;
}

template <typename T>
std::optional<T> fromOptionalJsonValue(const QJsonValue& value) {
    if (value.isNull() || value.isUndefined())
        return std::nullopt;
    else {
        return std::make_optional(fromJsonValue<T>(value));
    }
}

struct Address {
    QString street;
    bool straight;
    std::optional<int> blockCount;

    static Address fromJsonValue(const QJsonValue& json) {
        auto object = json.toObject();
        return {
            ::fromJsonValue<QString>(object.value(QLatin1Literal("street"))),
            object.value(QLatin1Literal("straight")).toBool(),
            fromOptionalJsonValue<int>(
                object.value(QLatin1Literal("block_count")))};
    }
};

struct Person {
    QString name;
    int age;
    QVector<Address> homeAddresses;

    static Person fromJsonValue(const QJsonValue& json) {
        auto object = json.toObject();
        return {::fromJsonValue<QString>(object.value(QLatin1Literal("name"))),
                ::fromJsonValue<int>(object.value(QLatin1Literal("age"))),
                ::fromJsonArray<Address>(object.value(QLatin1Literal("home_addresses")))};
    }
};
int main(int argc, char* argv[]) {
    QString jsonString("{\n"
                       "    \"name\":\"wenjun\",\n"
                       "    \"age\":39,\n"
                       "\"home_addresses\":[{\n"
                       "\n"
                       "    \"street\":\"zhonshan\",\n"
                       "    \"straight\":true\n"
                       "}]\n"
                       "}");
    QJsonDocument document = QJsonDocument::fromJson(jsonString.toUtf8());
    QJsonObject object     = document.object();
    auto person            = Person::fromJsonValue(object);

    qDebug() << "name: " << person.name;
    if (person.homeAddresses.size()) {
        qDebug() << "street" << person.homeAddresses[0].street;
    } else {
        qDebug() << "no address";
    }
}