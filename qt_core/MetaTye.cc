//
// Created by HUANG WEN JUN on 2022/1/6.
//
#include <QMetaType>
#include <QDebug>

int main(int argc,char *argv[]){
    auto pointMT = QMetaType::type("QPoint");

    qInfo() << "QPoint's type index: " << pointMT;

}