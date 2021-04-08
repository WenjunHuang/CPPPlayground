//
// Created by rick on 2021/4/8.
//
#include <QtWidgets>
int main(int argc,char *argv[]){
    QApplication app{argc,argv};
    QPushButton button{"Hello"};
    button.show();

    return app.exec();
}