//
// Created by rick on 2022/2/8.
//

#include "person.h"

Person::Person(QObject *parent) : QObject{parent} {
}

const QString &Person::getNames() const {
  return _names;
}

void Person::setNames(const QString &names) {
  if (_names == names)
    return;

  _names = names;
  emit namesChanged(names);
}

const QString &Person::getFavoriteColor() const {
  return _favoriteColor;
}

void Person::setFavoriteColor(const QString &favoriteColor) {
  if (_favoriteColor == favoriteColor)
    return;

  _favoriteColor = favoriteColor;
  emit favoriteColorChanged(_favoriteColor);
}

int Person::getAge() const {
  return _age;
}

void Person::setAge(int age) {
  if (_age == age)
    return;
  _age = age;
  emit ageChanged(_age);
}

Person::Person(QString names, QString favoriteColor, int age) : _names(std::move(names)),
                                                                _favoriteColor{std::move(favoriteColor)},
                                                                _age{age} {

}
