//
// Created by HUANG WEN JUN on 2022/1/3.
//

#include <QFileInfo>
#include <QDebug>
#include <QDir>
#include <algorithm>
#include <QDateTime>

void list(QString path) {
    qInfo() << "Listing: " << path;
    QDir dir{path};
    qInfo() << dir.absolutePath();

    QFileInfoList dirs = dir.entryInfoList(QDir::Dirs | QDir::NoDotAndDotDot);
    QFileInfoList files = dir.entryInfoList(QDir::Files);

    std::for_each(std::begin(dirs), std::end(dirs), [](auto const &info) {
        qInfo() << info.fileName();
    });
    std::for_each(std::begin(files), std::end(files), [](auto const &info) {
        qInfo() << "..Name: " << info.fileName();
        qInfo() << "..Size: " << info.size();
        qInfo() << "..Created: " << info.birthTime();
        qInfo() << "..Modified: " << info.lastModified();
    });

    std::for_each(std::begin(dirs),std::end(dirs),[](auto const& info){
       list(info.absolutePath());
    });
}

int main() {
    list(QDir::tempPath());

}
