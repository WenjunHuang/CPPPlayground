//
// Created by rick on 2022/2/8.
//

#pragma once

#include <QObject>
#include <QAbstractListModel>
#include <vector>
#include <memory>
#include <QVariant>
#include "person.h"

class PersonModel : public QAbstractListModel {
Q_OBJECT
enum PersonRoles {
    NamesRole = Qt::UserRole + 1,
    FavoriteColorRole,
    AgeRole
};
public:
    explicit PersonModel(QObject *parent = nullptr);

    QHash<int, QByteArray> roleNames() const override;

    [[nodiscard]] int rowCount(const QModelIndex &parent) const override;

    [[nodiscard]] QVariant data(const QModelIndex &index, int role) const override;

    bool setData(const QModelIndex &index, const QVariant &value, int role) override;

    [[nodiscard]] QVariant headerData(int section, Qt::Orientation orientation, int role) const override;

    [[nodiscard]] Qt::ItemFlags flags(const QModelIndex &index) const override;

    void addPerson(QString names,QString favoriteColor,int age);

    void removePerson(QModelIndex index);

private:
    std::vector<std::unique_ptr<Person>> _persons;
};


