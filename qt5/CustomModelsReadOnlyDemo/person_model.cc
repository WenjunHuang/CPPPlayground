//
// Created by rick on 2022/2/8.
//

#include "person_model.h"

// class template deduction guide
template<class T>
QScopedPointer(T *) -> QScopedPointer<T>;

PersonModel::PersonModel(QObject *parent) : QAbstractListModel{parent} {
  _persons.emplace_back(std::make_unique<Person>("Jamie Lannister", "red", 33));
  _persons.emplace_back(std::make_unique<Person>("Marry Lane", "cyan", 26));
  _persons.emplace_back(std::make_unique<Person>("Steve Moors", "yellow", 44));
}

int PersonModel::rowCount(const QModelIndex &parent) const {
  return _persons.size();
}

QVariant PersonModel::data(const QModelIndex &index, int role) const {
  if (!index.isValid() || index.row() >= _persons.size())
    return {};
  auto &person = _persons[index.row()];

  if (role == Qt::DisplayRole) {
    return person->getNames();
  }

  if (role == Qt::EditRole) {
    return person->getNames();
  }

  if (role == Qt::ToolTipRole) {
    return person->getNames() + " " + QString::number(index.row());
  }
  return {};
}

bool PersonModel::setData(const QModelIndex &index, const QVariant &value, int role) {
  if (!index.isValid())
    return false;

  auto &person = _persons[index.row()];
  bool somethingChanged = false;
  switch (role) {
    case Qt::EditRole: {
      if (person->getNames() != value.toString()) {
        person->setNames(value.toString());
        somethingChanged = true;
      }
      break;
    }
  }

  if (somethingChanged)
    emit dataChanged(index, index);
}

QVariant PersonModel::headerData(int section, Qt::Orientation orientation, int role) const {
  if (role != Qt::DisplayRole)
    return {};

  if (orientation == Qt::Horizontal)
    return QString{"Person names"};

  // vertical rows
  return QString{"Person %1"}.arg(section);

}

Qt::ItemFlags PersonModel::flags(const QModelIndex &index) const {
  if (!index.isValid())
    return QAbstractListModel::flags(index);

  return QAbstractListModel::flags(index) | Qt::ItemIsEditable;
}

void PersonModel::addPerson(QString names, QString favoriteColor, int age) {
  auto index = _persons.size();
  beginInsertRows(QModelIndex{}, index, index);
  _persons.emplace_back(std::make_unique<Person>(std::move(names), std::move(favoriteColor), age));
  endInsertRows();

}

void PersonModel::removePerson(QModelIndex index) {
  beginRemoveRows({}, index.row(), index.row());
  _persons.erase(_persons.begin() + index.row());
  endRemoveRows();

}
