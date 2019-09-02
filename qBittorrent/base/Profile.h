//
// Created by rick on 2019/8/30.
//

#pragma once
#include <QSettings>
#include <memory>
class QString;
class Application;

namespace Private {
    class Profile;
    class PathConverter;
} // namespace Private

using SettingsPtr = std::unique_ptr<QSettings>;

enum class SpecialFolder { Cache, Config, Data, Downloads };

class Profile {
  public:
    QString location(SpecialFolder folder) const;
    SettingsPtr applicationSettings(const QString& name) const;

    QString profileName() const;

    QString toPortablePath(const QString& absolutePath) const;
    QString fromPortablePath(const QString& portablePath) const;

    static const Profile& instance();

  private:
    Profile(Private::Profile* impl, Private::PathConverter* pathConverter);
    ~Profile();

    friend class ::Application;
    static void initialize(const QString& rootProfilePath,
                           const QString& configurationName,
                           bool convertPathsToProfileRelative);

    void ensureDirectoryExists(SpecialFolder folder);
    const std::unique_ptr<Private::Profile> m_profileImpl;
    const std::unique_ptr<Private::PathConverter> m_pathConverterImple;
    static Profile* m_instance;
};

QString specialFolderLocation(SpecialFolder folder);