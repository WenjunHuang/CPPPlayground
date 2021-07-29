//
// Created by rick on 2021/7/29.
//
#include "QSkiaOpenGlWindow.h"
#include "Scene.h"
#include <QApplication>
int main(int argc, char* argv[])
{
    QApplication app{argc,argv};
    Scene::initializeResources();

    QSkiaOpenGLWindow window;
    window.setWidth(800);
    window.setHeight(600);
    window.show();

    return app.exec();
}