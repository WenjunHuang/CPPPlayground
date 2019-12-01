//
// Created by HUANG WEN JUN on 2019/11/28.
//
#include <QtCore>
int main(int argc, char* argv[]) {
    QVariantHash varHash;
    varHash.insert("a", "microsoft");
    varHash.insert("b", "google");

    for (auto p = varHash.cbegin(); p != varHash.cend(); p++) {
        qDebug() << p.key() << p.value();
    }
}
