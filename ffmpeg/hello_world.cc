//
// Created by HUANG WEN JUN on 2019/12/8.
//

#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <QtCore>

int main(int argc,char *argv[]) {
    if (argc < 2) {
        qDebug() << "You need to specify a media file";
        return -1;
    }

}