//
// Created by HUANG WEN JUN on 2019/8/30.
//

#include "Fs.h"
#include <cstring>
#if defined(Q_OS_WIN)
#include <memory>
#endif

#include <sys/stat.h>
#include <sys/types.h>

#if defined(Q_OS_WIN)
#include <Windows.h>
#elif defined(Q_OS_MAC) || defined(Q_OS_FREEBSD) || defined(Q_OS_OPENBSD)
#include <sys/mount.h>
#include <sys/param.h>
#else
#include <sys/vfs.h>
#include <unistd.h>
#endif

#include <QDebug>
#include <QDir>
#include <QDirIterator>
#include <QFile>
#include <QFileInfo>
#include <QRegularExpression>
#include <QStorageInfo>
#include <sys/errno.h>

QString Utils::Fs::toNativePath(const QString& path) {
    return QDir::toNativeSeparators(path);
}

QString Utils::Fs::toUniformPath(const QString& path) {
    return QDir::fromNativeSeparators(path);
}

QString Utils::Fs::fileExtension(const QString& filename) {
    auto ext        = QString(filename).remove(QB_EXT);
    auto pointIndex = ext.lastIndexOf('.');
    return (pointIndex >= 0) ? ext.mid(pointIndex + 1) : QString();
}

QString Utils::Fs::fileName(const QString& filePath) {
    const auto path       = toUniformPath(filePath);
    const auto slashIndex = path.lastIndexOf('/');
    if (slashIndex == -1)
        return path;

    return path.mid(slashIndex + 1);
}

QString Utils::Fs::folderName(const QString& filePath) {
    const auto path       = toUniformPath(filePath);
    const auto slashIndex = path.lastIndexOf('/');
    if (slashIndex == -1)
        return path;
    return path.left(slashIndex);
}

bool Utils::Fs::smartRemoveEmptyFolderTree(const QString& path) {
    if (path.isEmpty() || !QDir(path).exists())
        return true;

    const QStringList deleteFilesList = {// Windows
                                         "Thumbs.db", "desktop.ini",
                                         // Linux
                                         ".directory",
                                         // Mac OS
                                         ".DS_Store"};

    QStringList dirList(path + '/');
    QDirIterator iter(path, (QDir::AllDirs | QDir::NoDotAndDotDot),
                      QDirIterator::Subdirectories);
    while (iter.hasNext())
        dirList << iter.next() + '/';

    // sort desc by directory depth
    std::sort(dirList.begin(), dirList.end(),
              [](const QString& l, const QString& r) {
                  return l.count('/') > r.count('/');
              });

    for (auto& p : std::as_const(dirList)) {
        // remove unwanted files
        for (auto& f : deleteFilesList) {
            forceRemove(p + f);
        }

        // remove temp files on linux,e.g.'filename~'
        const QDir dir(p);
        const auto tempFileList = dir.entryList(QDir::Files);
        for (auto& f : tempFileList) {
            if (f.endsWith('~'))
                forceRemove(p + f);
        }
        // remove directory if empty
        dir.rmdir(p);
    }

    return QDir(path).exists();
}

bool Utils::Fs::forceRemove(const QString& filePath) {
    QFile f(filePath);
    if (!f.exists())
        return true;

    // Make sure we have read/write permissions
    f.setPermissions(f.permissions() | QFile::ReadOwner | QFile::WriteOwner |
                     QFile::ReadUser | QFile::WriteUser);
    return f.remove();
}

void Utils::Fs::removeDirRecursive(const QString& path) {
    if (!path.isEmpty())
        QDir(path).removeRecursively();
}

qint64 Utils::Fs::computePathSize(const QString& path) {
    // check if it is a file
    const QFileInfo fi(path);
    if (!fi.exists())
        return -1;
    if (fi.isFile())
        return fi.size();

    // Compute folder size based on its content
    qint64 size = 0;
    QDirIterator iter(path, QDir::Files | QDir::Hidden | QDir::NoSymLinks,
                      QDirIterator::Subdirectories);
    while (iter.hasNext()) {
        iter.next();
        size += iter.fileInfo().size();
    }
    return size;
}

/**
 * Makes deep comparison of two files to make sure they are identical.
 * @param path1
 * @param path2
 * @return
 */
bool Utils::Fs::sameFiles(const QString& path1, const QString& path2) {
    QFile f1(path1), f2(path2);
    if (!f1.exists() || !f2.exists())
        return false;
    if (f1.size() != f2.size())
        return false;
    if (!f1.open(QIODevice::ReadOnly))
        return false;
    if (!f2.open(QIODevice::ReadOnly))
        return false;

    auto readSize = 1024 * 1024;
    while (!f1.atEnd() && !f2.atEnd()) {
        if (f1.read(readSize) != f2.read(readSize))
            return false;
    }
    return true;
}

QString Utils::Fs::toValidFileSystemName(const QString& name,
                                         const bool allowSeparators,
                                         const QString& pad) {
    const QRegularExpression regex(allowSeparators ? "[:?\"*<>|]+"
                                                   : "[\\\\/:?\"*<>|]+");

    QString validName = name.trimmed();
    validName.replace(regex, pad);
    qDebug() << "toValidFileSystemName:" << name << "=>" << validName;

    return validName;
}

bool Utils::Fs::isValidFileSystemName(const QString& name,
                                      const bool allowSeparators) {
    if (name.isEmpty())
        return false;

    const QRegularExpression regex(allowSeparators ? "[:?\"*<>|]"
                                                   : R"([\\/:?"*<>|])");
    return !name.contains(regex);
}

qint64 Utils::Fs::freeDiskSpaceOnPath(const QString& path) {
    if (path.isEmpty())
        return -1;
    return QStorageInfo(path).bytesAvailable();
}

std::pair<QString, QString> Utils::Fs::branchPath(const QString& filePath) {
    QString ret = toUniformPath(filePath);
    if (ret.endsWith('/'))
        ret.chop(1);
    auto slashIndex = ret.lastIndexOf('/');
    if (slashIndex >= 0) {
        return std::make_pair(ret.left(slashIndex), ret.mid(slashIndex + 1));
    } else
        return std::make_pair(ret, QString());
}

bool Utils::Fs::sameFileNames(const QString& first, const QString& second) {
#if defined(Q_OS_UNIX) || defined(Q_WS_QWS)
    return QString::compare(first, second, Qt::CaseSensitive) == 0;
#else
    return QString::compare(first, second, Qt::CaseInsensitive) == 0;
#endif
}

QString Utils::Fs::expandPath(const QString& path) {
    const auto ret = path.trimmed();
    if (ret.isEmpty())
        return ret;
    return QDir::cleanPath(ret);
}

QString Utils::Fs::expandPathAbs(const QString& path) {
    return QDir(expandPath(path)).absolutePath();
}

QString Utils::Fs::tempPath() {
    static const QString path = QDir::tempPath() + "/.qBittorrent/";
    QDir().mkdir(path);
    return path;
}

bool Utils::Fs::isRegularFile(const QString& path) {
    struct stat st;
    if (stat(path.toUtf8().constData(), &st) != 0) {
        const auto err = errno;
        qDebug("Could not get file stats for path '%s'. Error: %s",
               qUtf8Printable(path), qUtf8Printable(strerror(err)));
        return false;
    }

    return (st.st_mode & S_IFMT) == S_IFREG;
}