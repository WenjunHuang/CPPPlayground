//
// Created by rick on 2022/2/8.
//

#pragma once

#include <QObject>

class Person : public QObject {
Q_OBJECT
public:
    explicit Person(QObject *parent = nullptr);

    Person(QString name,
           QString favoriteColor,
           int age,QObject *parent = nullptr);

    const QString &getNames() const;

    void setNames(const QString &names);

    const QString &getFavoriteColor() const;

    void setFavoriteColor(const QString &favoriteColor);

    int getAge() const;

    void setAge(int age);

signals:

    void namesChanged(QString names);

    void favoriteColorChanged(QString favoriteColor);

    void ageChanged(int age);

private:
    QString _names;
    QString _favoriteColor;
    int _age;
};


