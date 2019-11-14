#include <QWidget>
#include <QApplication>
#include <QPushButton>
#include <QDebug>
#include "example.h"

int main(int argc,char *argv[]){
    QApplication app(argc,argv);
    Example ex;


    QHash<QString,QString> hash;
    hash["abc"]  ="foo";

    for (auto&&  [key,value]: hash) {
      qDebug() << value.key() << value.value();
    }

    return app.exec();
}