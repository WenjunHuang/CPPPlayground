#include <QWidget>
#include <QApplication>
#include <QPushButton>
#include "example.h"

int main(int argc,char *argv[]){
    QApplication app(argc,argv);
    Example ex;

    return app.exec();
}