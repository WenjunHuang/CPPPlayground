//
// Created by HUANG WEN JUN on 2022/1/3.
//
#include <QStorageInfo>
#include <QDebug>
#include <QFileInfo>
#include <algorithm>

int main() {
    auto storages = QStorageInfo::mountedVolumes();
    std::for_each(storages.cbegin(),
                  storages.cend(),
                  [](auto &storage) {
                      qInfo() << storage;
                  });

    auto root = QStorageInfo::root();

}

