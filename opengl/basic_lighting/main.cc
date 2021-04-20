//
// Created by rick on 2021/4/20.
//
#include "basic_lighting_window.h"
#include <QGuiApplication>

int main(int argc,char *argv[]){
    QGuiApplication app{argc,argv};
    BasicLightingWindow window;
    window.resize(800, 600);
    window.show();
    return app.exec();
}
