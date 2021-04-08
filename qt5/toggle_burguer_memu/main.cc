//
// Created by rick on 2021/4/8.
//
#include "main_window.h"
#include <QtWidgets>
int main(int argc,char *argv[]){
    QApplication app{argc,argv};
    MainWindow window;
    window.show();

    return app.exec();
}