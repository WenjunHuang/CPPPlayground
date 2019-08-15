//
// Created by xxzyjy on 12/11/2017.
//

#include "StringListModel.h"

int StringListModel::rowCount(const QModelIndex &parent) const {
  return stringList.count();
}

QVariant StringListModel::data(const QModelIndex &index, int role) const {
  if (!index.isValid())
    return QVariant();

  if (index.row() >= stringList.size())
    return QVariant();

  if (role == Qt::DisplayRole)
    return stringList.at(index.row());
  else
    return QVariant();
}
