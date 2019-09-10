//
// Created by rick on 2019/9/10.
//

#include "async_file_storage.h"
#include <QDebug>
#include <QtCore/QSaveFile>
AsyncFileStorage::AsyncFileStorage(const QString& storageFolderPath,
                                   QObject* parent)
    : QObject(parent), m_storageDir(storageFolderPath),
      m_lockFile(
          m_storageDir.absoluteFilePath(QStringLiteral("storage.lock"))) {
    if (!m_storageDir.mkpath(m_storageDir.absolutePath())) {}

    if (!m_lockFile.open(QFile::WriteOnly)) {}
}
AsyncFileStorage::~AsyncFileStorage() {
    m_lockFile.close();
    m_lockFile.remove();
}

void AsyncFileStorage::store(const QString& fileName, const QByteArray& data) {
    QMetaObject::invokeMethod(
        this, [this, data, fileName]() { store_impl(fileName, data); },
        Qt::QueuedConnection);
}

QDir AsyncFileStorage::storageDir() const { return m_storageDir; }

void AsyncFileStorage::store_impl(const QString& fileName,
                                  const QByteArray& data) {
    const QString filePath = m_storageDir.absoluteFilePath(fileName);
    QSaveFile file(filePath);
    qDebug() << "AsyncFileStorage: Saving data to" << filePath;

    if (file.open(QIODevice::WriteOnly)) {
        file.write(data);
        if (!file.commit()) {
            qDebug() << "AsyncFileStorage: Failed to save data";
            emit failed(filePath, file.errorString());
        }
    }
}