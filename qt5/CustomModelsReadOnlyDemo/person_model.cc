//
// Created by rick on 2022/2/8.
//

#include "person_model.h"

// class template deduction guide
template<class T>
QScopedPointer(T *) -> QScopedPointer<T>;

PersonModel::PersonModel(QObject *parent) : QAbstractTableModel{parent} {
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

  if (role == Qt::DisplayRole || role == Qt::EditRole) {
    if (index.column() == 0)
      return person->getNames();
    if (index.column() == 1)
      return person->getAge();
    if (index.column() == 2)
      return person->getFavoriteColor();
  }


  if (role == NamesRole) {
    return person->getNames();
  }

  if (role == FavoriteColorRole) {
    return person->getFavoriteColor();
  }

  if (role == AgeRole) {
    return person->getAge();
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
      if (index.column() == 0) {
        // Names
        if (person->getNames() != value.toString()) {
          person->setNames(value.toString());
          somethingChanged = true;
        }
      }
      if (index.column() == 1) {
        // Age
        if (person->getAge() != value.toInt()) {
          person->setAge(value.toInt());
          somethingChanged = true;
        }
      }
      if (index.column() == 2) {
        // FavoriteColor
        if (person->getFavoriteColor() != value.toString()) {
          person->setFavoriteColor(value.toString());
          somethingChanged = true;
        }
      }

      break;
    }
    case NamesRole: {
      if (person->getNames() != value.toString()) {
        person->setNames(value.toString());
        somethingChanged = true;
      }
      break;
    }
    case AgeRole: {
      if (person->getAge() != value.toInt()) {
        person->setAge(value.toInt());
        somethingChanged = true;
      }
      break;
    }
    case FavoriteColorRole: {
      if (person->getFavoriteColor() != value.toString()) {
        person->setFavoriteColor(value.toString());
        somethingChanged = true;
      }
      break;
    }
    default:
      break;
  }

  if (somethingChanged) {
    emit dataChanged(index, index);
    return true;
  }
  return false;
}

QVariant PersonModel::headerData(int section, Qt::Orientation orientation, int role) const {
  if (role != Qt::DisplayRole)
    return {};

  if (orientation == Qt::Horizontal) {
    switch (section) {
      case 0:
        return QString{"Names"};
      case 1:
        return QString{"Age"};
      case 2:
        return QString{"Favorite color"};
      default:
        return QString{};
    }
  } else
    // vertical rows
    return QString{"Person %1"}.arg(section);

}

Qt::ItemFlags PersonModel::flags(const QModelIndex &index) const {
  if (!index.isValid())
    return QAbstractTableModel::flags(index);

  return QAbstractTableModel::flags(index) | Qt::ItemIsEditable;
}

void PersonModel::addPerson(QString names, QString favoriteColor, int age) {
//  auto index = _persons.size();
//  beginInsertRows(QModelIndex{}, index, index);
//  _persons.emplace_back(std::make_unique<Person>(std::move(names), std::move(favoriteColor), age));
//  endInsertRows();
  insertRows(_persons.size(), 1);
  setData(index(_persons.size() - 1, 0), names, Qt::EditRole);
  setData(index(_persons.size() - 1, 1), age, Qt::EditRole);
  setData(index(_persons.size() - 1, 2), favoriteColor, Qt::EditRole);
}

void PersonModel::removePerson(QModelIndex index) {
//  beginRemoveRows({}, index.row(), index.row());
//  _persons.erase(_persons.begin() + index.row());
//  endRemoveRows();
  removeRows(index.row(), 1);

}

QHash<int, QByteArray> PersonModel::roleNames() const {
  return {{NamesRole,         "names"},
          {FavoriteColorRole, "favoriteColor"},
          {AgeRole,           "age"}};
}

bool PersonModel::insertRows(int row, int count, const QModelIndex &parent) {
  beginInsertRows(parent, row, row + count - 1);

  for (int i = 0; i < count; ++i) {
    _persons.push_back(std::make_unique<Person>());
  }
  endInsertRows();
  return true;
}

bool PersonModel::removeRows(int row, int count, const QModelIndex &parent) {
  beginRemoveRows(parent, row, row + count - 1);
  auto from = _persons.begin() + row;
  auto to = from + count;
  _persons.erase(from, to);
  endRemoveRows();
  return true;
}

int PersonModel::columnCount(const QModelIndex &parent) const {
  Q_UNUSED(parent);
  return 3;
}

bool PersonModel::hasChildren(const QModelIndex &parent) const {
  if (parent.column() == 0)
    return false;
  return true;
}

