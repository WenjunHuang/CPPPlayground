//
// Created by rick on 2019/9/10.
//

#pragma once

#include <QtCore/QDir>
#include <QtCore/QObject>
class AsyncFileStorage : public QObject {
    Q_OBJECT
    Q_DISABLE_COPY(AsyncFileStorage)

  public:
    explicit AsyncFileStorage(const QString& storageFolderPath,
                              QObject* parent = nullptr);
    ~AsyncFileStorage() override;

    void store(const QString& fileName, const QByteArray& data);
    QDir storageDir() const;

  signals:
    void failed(const QString& fileName, const QString& errorString);

  private:
    Q_INVOKABLE void store_impl(const QString& fileName,
                                const QByteArray& data);

    QDir m_storageDir;
    QFile m_lockFile;
};
