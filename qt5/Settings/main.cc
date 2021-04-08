//
// Created by rick on 2021/2/22.
//
#include <QCoreApplication>
#include <QDebug>
#include <QSettings>

int main(int argc,char *argv[]) {

    QCoreApplication app(argc,argv);

    QCoreApplication::setOrganizationName("Foo");

    return app.exec();

}