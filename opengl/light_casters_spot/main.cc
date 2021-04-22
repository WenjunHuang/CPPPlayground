#include "light_casters_spot_window.h"
#include <QGuiApplication>
//
// Created by rick on 2021/4/22.
//
int main(int argc,char * argv[]){
    QGuiApplication app{argc,argv};
    LightCastersSpotWindow window;
    window.show();
    return QGuiApplication::exec();

}