//
// Created by rick on 2021/7/29.
//
#include "QSkiaOpenGlWindow.h"
#include <QApplication>
int main(int argc, char* argv[])
{
    QApplication app{argc,argv};
    QSkiaOpenGLWindow window;
    window.setWidth(800);
    window.setHeight(600);
    window.show();

    return app.exec();
}