//
// Created by HUANG WEN JUN on 2019/8/30.
//

#pragma once

#include <QString>
namespace Utils::Fs {
    QString toNativePath(const QString& path);

    QString toUniformPath(const QString& path);
    QString fileExtension(const QString& filename);
    QString fileName(const QString& filePath);
    QString folderName(const QString& filePath);
    qint64 computePathSize(const QString& path);
    bool sameFiles(const QString& path1, const QString& path2);
    QString toValidFileSystemName(const QString& name,
                                  bool allowSeparators = false,
                                  const QString& pad   = QLatin1String(" "));
    bool isValidFileSystemName(const QString& name,
                               bool allowSeparators = false);
    qint64 freeDiskSpaceOnPath(const QString& path);
    std::pair<QString,QString> branchPath(const QString& filePath);
    bool sameFileNames(const QString& first, const QString& second);
    QString expandPath(const QString& path);
    QString expandPathAbs(const QString& path);
    bool isRegularFile(const QString& path);

    bool smartRemoveEmptyFolderTree(const QString& path);
    bool forceRemove(const QString& filePath);
    void removeDirRecursive(const QString& path);

    QString tempPath();
    bool isNetworkFileSystem(const QString& path);

} // namespace Utils::Fs
