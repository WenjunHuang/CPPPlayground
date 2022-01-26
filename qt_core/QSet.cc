//
// Created by HUANG WEN JUN on 2022/1/3.
//
#include <QSet>
#include <QDebug>

int main() {
    QSet<QString> people{};
    people << "Bryan" << "Heather" << "Tammy" << "Chris";
    people.insert("Rango");

    foreach(const QString& person, people){
        qInfo() << person;
    }

}

