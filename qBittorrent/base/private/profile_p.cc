//
// Created by rick on 2019/8/30.
//

#include "profile_p.h"
#include <QCoreApplication>
#include <memory>

Private::Profile::Profile(const QString& configurationName)
    : m_configurationSuffix{configurationName.isEmpty()
                                ? QString()
                                : QLatin1Char('_') + configurationName} {}

QString Private::Profile::configurationSuffix() const {
    return m_configurationSuffix;
}

QString Private::Profile::profileName() const {
    return QCoreApplication::applicationName() + configurationSuffix();
}

Private::DefaultProfile::DefaultProfile(const QString& configurationName)
    : Profile(configurationName) {}

QString Private::DefaultProfile::baseDirectory() const {
    return QDir::homePath();
}

QString Private::DefaultProfile::cacheLocation() const {
    return locationWithConfigurationName(QStandardPaths::CacheLocation);
}

QString Private::DefaultProfile::configLocation() const {
#if defined(Q_OS_WIN)
    return locationWithConfigurationName(QStandardPaths::AppDataLocation);
#else
    return locationWithConfigurationName(QStandardPaths::AppConfigLocation);
#endif
}

QString Private::DefaultProfile::dataLocation() const {
#if defined(Q_OS_WIN) || defined(Q_OS_MAC)
    return locationWithConfigurationName(QStandardPaths::AppLocalDataLocation);
#else
    return QStandardPaths::writableLocation(
               QStandardPaths::GenericDataLocation) +
           QLatin1String("/data/") + profileName() + QLatin1Char('/');
#endif
}

QString Private::DefaultProfile::downloadLocation() const {
    return QStandardPaths::writableLocation(QStandardPaths::DownloadLocation);
}

SettingsPtr
Private::DefaultProfile::applicationSettings(const QString& name) const {
#if defined(Q_OS_WIN) || defined(Q_OS_MAC)
    return std::make_unique<QSettings>(
        QSettings::IniFormat, QSettings::UserScope, profileName(), name);
#else
    return std::make_unique<QSettings>(profileName(), name);
#endif
}

QString Private::DefaultProfile::locationWithConfigurationName(
    QStandardPaths::StandardLocation location) const {
    return QStandardPaths::writableLocation(location) + configurationSuffix();
}

Private::CustomProfile::CustomProfile(const QString& rootPath,
                                      const QString& configurationName)
    : Profile{configurationName},
      m_rootDirectory{QDir(rootPath).absoluteFilePath(this->profileName())} {}

QString Private::CustomProfile::baseDirectory() const {
    return m_rootDirectory.canonicalPath();
}

QString Private::CustomProfile::cacheLocation() const {
    return m_rootDirectory.absoluteFilePath(QLatin1String(kCacheDirName));
}

QString Private::CustomProfile::configLocation() const {
    return m_rootDirectory.absoluteFilePath(QLatin1String(kConfigDirName));
}

QString Private::CustomProfile::dataLocation() const {
    return m_rootDirectory.absoluteFilePath(QLatin1String(kDataDirName));
}

QString Private::CustomProfile::downloadLocation() const {
    return m_rootDirectory.absoluteFilePath(QLatin1String(kDownloadsDirName));
}

SettingsPtr
Private::CustomProfile::applicationSettings(const QString& name) const {
#if defined(Q_OS_WIN) || defined(Q_OS_MAC)
    QLatin1String kConfFileExt(".ini");
#else
    QLatin1String kConfFileExt(".conf");
#endif
    const QString settingsFileName{
        QDir(configLocation()).absoluteFilePath(name + kConfFileExt)};
}

QString Private::NoConvertConverter::fromPortablePath(
    const QString& portablePath) const {
    return portablePath;
}

QString Private::NoConvertConverter::toPortablePath(const QString& path) const {
    return path;
}

Private::Converter::Converter(const QString& basePath):m_baseDir{basePath} {
    m_baseDir.makeAbsolute();
}

QString Private::Converter::toPortablePath(const QString& path) const {
    if (path.isEmpty() || m_baseDir.path().isEmpty()){
        return path;
    }

    return m_baseDir.relativeFilePath(path);
}

QString Private::Converter::fromPortablePath(const QString& portablePath) const {
    if (QDir::isAbsolutePath(portablePath))
        return portablePath;

    return QDir::cleanPath(m_baseDir.absoluteFilePath(portablePath));
}