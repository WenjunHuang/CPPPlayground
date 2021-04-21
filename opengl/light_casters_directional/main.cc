//
// Created by rick on 2021/4/21.
//

#include "main_window.h"
#include <QGuiApplication>
int main(int argc,char *argv[]){
    QGuiApplication app{argc,argv};
    LightCastersDirectionalWindow window;
    window.show();
    return QGuiApplication::exec();
}
