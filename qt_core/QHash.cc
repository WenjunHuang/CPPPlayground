//
// Created by HUANG WEN JUN on 2022/1/2.
//
#include <QHash>
#include <QDebug>

int main() {
    QHash<QString, int> ages;
    ages.insert("Bryan", 44);
    ages.insert("Tammy", 49);
    ages.insert("Heather", 25);
    ages.insert("Chris", 27);
    ages.insert("Rango", 15);

    qInfo() << "Bryan is " << ages["Bryan"] << "years old";
    qInfo() << "Keys: " << ages.keys();
    qInfo() << "Sizes: " << ages.size();

    foreach(QString key, ages.keys()) {
        qInfo() << key << " = " << ages[key];
    }

}