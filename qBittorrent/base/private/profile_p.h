//
// Created by rick on 2019/8/30.
//

#pragma once
#include "Profile.h"
#include <QDir>
#include <QStandardPaths>

namespace Private {
    class Profile {
      public:
        virtual QString baseDirectory() const    = 0;
        virtual QString cacheLocation() const    = 0;
        virtual QString configLocation() const   = 0;
        virtual QString dataLocation() const     = 0;
        virtual QString downloadLocation() const = 0;
        virtual SettingsPtr applicationSettings(const QString& name) const;

        virtual ~Profile() = default;
        QString profileName() const;

      protected:
        explicit Profile(const QString& configurationName);
        QString configurationSuffix() const;

      private:
        QString m_configurationSuffix;
    };

    class DefaultProfile : public Profile {
      public:
        explicit DefaultProfile(const QString& configurationName);

        QString baseDirectory() const override;
        QString cacheLocation() const override;
        QString configLocation() const override;
        QString dataLocation() const override;
        QString downloadLocation() const override;
        SettingsPtr applicationSettings(const QString& name) const override;

      private:
        QString locationWithConfigurationName(
            QStandardPaths::StandardLocation location) const;
    };

    class CustomProfile : public Profile {
      public:
        CustomProfile(const QString& rootPath,
                      const QString& configurationName);

        QString baseDirectory() const override;
        QString cacheLocation() const override;
        QString configLocation() const override;
        QString dataLocation() const override;
        QString downloadLocation() const override;
        SettingsPtr applicationSettings(const QString& name) const override;

      private:
        QDir m_rootDirectory;
        static constexpr const char* kCacheDirName     = "cache";
        static constexpr const char* kConfigDirName    = "config";
        static constexpr const char* kDataDirName      = "data";
        static constexpr const char* kDownloadsDirName = "downloads";
    };

    class PathConverter {
      public:
        virtual QString toPortablePath(const QString &path) const = 0;
        virtual QString fromPortablePath(const QString &portablePath) const = 0;
        virtual ~PathConverter() = default;
    };

    class NoConvertConverter : public PathConverter {
      public:
        QString toPortablePath(const QString &path) const override;
        QString fromPortablePath(const QString &portablePath) const override;

    };

    class Converter: public PathConverter {
      public:
        explicit Converter(const QString &basePath);
        QString toPortablePath(const QString &path) const override ;
        QString fromPortablePath(const QString &portablePath) const override;
      private:
        QDir m_baseDir;
    };

} // namespace Private
