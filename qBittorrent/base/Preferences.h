//
// Created by HUANG WEN JUN on 2019/8/29.
//

#pragma once
#include <QList>
#include <QObject>
#include <QVariant>

enum SchedulerDays {
    EVERY_DAY,
    WEEK_DAYS,
    WEEK_ENDS,
    MON,
    TUE,
    WED,
    THU,
    FRI,
    SAT,
    SUN
};

namespace TrayIcon
{
    enum Style
    {
        NORMAL = 0,
        MONO_DARK,
        MONO_LIGHT
    };
}

namespace DNS
{
    enum Service
    {
        DYNDNS,
        NOIP,
        NONE = -1
    };
}

class Preferences: public QObject {
   Q_OBJECT
   Q_DISABLE_COPY(Preferences)

   Preferences();

   const QVariant value(const QString& key,const QVariant& defaultValue = {}) const;
   void setValue(const QString& key,const QVariant& value);

   static Preferences *m_instance;

   signals:
    void changed();
  public:
    static void initInstance();
    static void freeInstance();
    static Preferences *instance();

    QString getLocale() const;
    void setLocale(const QString &locale);
    bool userCustomUITheme() const;
    void setUseCustomUITheme(bool use);
    QString customUIThemePath() const;
    void setCustomUIThemePath(const QString &path);
    bool deleteTorrentFilesAsDefault() const;
    void setDeleteTorrentFilesAsDefault(bool value);
    bool confirmOnExit() const;
    void setConfirmOnExit(bool value);

    bool speedInTitleBar() const;
    void setSpeedInTitleBar(bool value);

    bool useAlternatingRowColors() const;
    void setUseAlternatingRowColors(bool value);

    bool hideZeroValues() const;
    void setHideZeroValues(bool value);

    int hideZeroComboValues() const;
    void setHideZeroComboValues(int value);

    bool statusbarDisplayed() const;
    void setStatusbarDisplayed(bool value);

    bool toolbarDisplayed() const;
    void setToolbarDisplayed(bool value);

    bool startMinimized() const;
    void setStartMinimized(bool value);

    bool splashScreenDisabled() const;
    void setSplashScreenDisabled(bool value);

    bool preventFromSuspendWhenDownloading() const;
    void setPreventFromSuspendWhenDownloading(bool value);

    bool preventFromSuspendWhenSeeding() const;
    void setPreventFromSuspendWhenSeeding(bool value);

    // Downloads
    QString lastLocationPath() const;
    void setLastLocationPath(const QString& value);

    QVariantHash scanDirs() const;
    void setScanDirs(const QVariantHash& value);

    QString scanDirsLastPath() const;
    void setScanDirsLastPath(const QString& value);

    bool mailNotificationEnabled() const;
    void setMailNotificationEnabled(bool value);

    QString mailNotificationSender() const;
    void setMailNotificationSender(const QString& value);

    QString mailNotificationEmail() const;
    void setMailNotificationEmail(const QString& value);

    QString mailNotificationSMTP() const;
    void setMailNotificationSMTP(const QString& value);

    bool mailNotificationSMTPSSL() const;
    void setMailNotificationSMTPSSL(bool value);

    bool mailNotificationSMPTAuth() const;
    void setMailNotificationSMPTAuth(bool value);

    QString mailNotificationSMTPUsername() const;
    void setMailNotificationSMTPUsername(const QString& value);

    int actionOnDblClOnTorrentDl() const;
    void setActionOnDblClOnTorrentDl(int value);

    int actionOnDblClOnTorrentFn() const;
    void setActionOnDblClOnTorrentFn(int value);

    // Connection options
    QTime schedulerStartTime() const;
    void setSchedulerStartTime(const QTime& value);

    QTime schedulerEndTime() const;
    void setSchedulerEndTime(const QTime& value);

    SchedulerDays schedulerDays() const;
    void setSchedulerDays(SchedulerDays value);


    // Search
    bool searchEnabled() const;
    void setSearchEnabled(bool value);

    // HTTP Server
    bool webUiEnabled() const;
    void setWebUiEnabled(bool value);

    QString serverDomains() const;
    void setServerDomains(const QString& value);

    QString webUiAddress() const;
    void setWebUiAddress(const QString& value);

    quint16 webUiPort() const;
    void setWebUiPort(quint16 value);

    bool useUPnPForWebUiPort() const;
    void setUseUPnPForWebUiPort(bool value);

    // Authentication
    bool webUiLocalAuthEnabled() const;
    void setWebUiLocalAuthEnabled(bool value);

    bool webUiAuthSubnetWhitelistEnabled() const;
    void setWebUiAuthSubnetWhitelistEnabled(bool value);

    QVector<Utils::Net::Subnet> webUiAuthSubnetWhitelist() const;
    void setWebUiAuthSubnetWhitelist(QStringList subnets);

    QString webUiUsername() const;
    void setWebUiUsername(const QString& value);

    QByteArray webUiPassword() const;
    void setWebUiPassword(const QByteArray& value);
};
