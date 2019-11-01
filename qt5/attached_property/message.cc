//
// Created by HUANG WEN JUN on 2019/11/1.
//

#include "message.h"
Message::Message(QObject* parent) : QObject{parent} {}
void Message::setAuthor(const QString& author) {
    _author = author;
    emit authorChanged();
}
void Message::setCreationDate(const QDateTime& creationDate) {
    _creationDate = creationDate;
    emit creationDateChanged();
}
