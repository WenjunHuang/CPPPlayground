//
// Created by HUANG WEN JUN on 2022/1/3.
//
#include <QIODevice>
#include <QBuffer>
#include <QDebug>

int main() {
    QBuffer buffer{};

    if (buffer.open(QIODevice::ReadWrite)) {
        qInfo() << "Buffer Opened!";
        QByteArray data{"Hello World"};

        for (int i = 0; i < 5; ++i) {
            buffer.write(data);
            buffer.write("\r\n");
        }

        // Move to the first position
        buffer.seek(0);

        qInfo() << buffer.readAll();
        qInfo() << "closing the buffer";

        //ALWAYS close your device
        buffer.close();
    } else {
        qInfo() << "Could not open buffer!!";
    }
}
