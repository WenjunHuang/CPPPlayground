//
// Created by HUANG WEN JUN on 2019/11/1.
//

#pragma once

#include <QObject>
#include <QtCore>

class Message : public QObject {
    Q_OBJECT
    Q_PROPERTY(QString author READ author WRITE setAuthor NOTIFY authorChanged);
    Q_PROPERTY(QDateTime creationDate READ creationDate WRITE setCreationDate
                   NOTIFY creationDateChanged);

  public:
    Message(QObject* parent = nullptr);

    QString author() const { return _author; }

    void setAuthor(const QString& author);

    QDateTime creationDate() const { return _creationDate; }

    void setCreationDate(const QDateTime& creationDate);

  signals:
    void authorChanged();
    void creationDateChanged();

  private:
    QString _author;
    QDateTime _creationDate;
};
