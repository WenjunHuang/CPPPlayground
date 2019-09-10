//
// Created by HUANG WEN JUN on 2019/8/29.
//

#pragma once
#include <QList>
#include <QObject>
#include <QVariant>
#include <QtNetwork/QNetworkCookie>
#include <utils/net.h>

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

namespace TrayIcon {
    enum Style { NORMAL = 0, MONO_DARK, MONO_LIGHT };
}

namespace DNS {
    enum Service { DYNDNS, NOIP, NONE = -1 };
}

class Preferences : public QObject {
    Q_OBJECT
    Q_DISABLE_COPY(Preferences)

    Preferences();

    QVariant value(const QString& key,
                         const QVariant& defaultValue = {}) const;
    void setValue(const QString& key, const QVariant& value);

    static Preferences* m_instance;

  signals:
    void changed();

  public:
    static void initInstance();
    static void freeInstance();
    static Preferences* instance();

    QString getLocale() const;
    void setLocale(const QString& locale);
    bool userCustomUITheme() const;
    void setUseCustomUITheme(bool use);
    QString customUIThemePath() const;
    void setCustomUIThemePath(const QString& path);
    bool deleteTorrentFilesAsDefault() const;
    void setDeleteTorrentFilesAsDefault(bool value);
    bool confirmOnExit() const;
    void setConfirmOnExit(bool value);

    bool speedInTitleBar() const;
    void setSpeedInTitleBar(bool value);

    void showSpeedInTitleBar(bool show);

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

#ifdef Q_OS_WIN
    bool winStartup() const;
    void setWinStartup(bool value);
#endif

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

    bool mailNotificationSMTPAuth() const;
    void setMailNotificationSMTPAuth(bool value);

    QString mailNotificationSMTPUsername() const;
    void setMailNotificationSMTPUsername(const QString& value);

    QString mailNotificationSMTPPassword() const;
    void setMailNotificationSMTPPassword(const QString& value);

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

    int webUISessionTimeout() const;
    void setWebUISessionTimeout(int value);

    // WebUI security
    bool webUIClickjackingProtectionEnabled() const;
    void setWebUIClickjackingProtectionEnabled(bool value);
    bool webUICSRFProtectionEnabled() const;
    void setWebUICSRFProtectionEnabled(bool value);
    bool webUIHostHeaderValidationEnabled() const;
    void setWebUIHostHeaderValidationEnabled(bool value);

    // HTTPS
    bool webUIHttpsEnabled() const;
    void setWebUIHttpsEnabled(bool value);
    QString webUIHttpsCertificatePath() const;
    void setWebUIHttpsCertificatePath(const QString &value);
    QString webUIHttpsKeyPath() const;
    void setWebUIHttpsKeyPath(const QString &value);
    bool altWebUIEnabled() const;
    void setAltWebUIEnabled(bool value);
    QString webUIRootFolder() const;
    void setWebUIRootFolder(const QString &value);

    // Dynamic DNS
    bool dynDNSEnabled() const;
    void setDynDNSEnabled(bool value);
    DNS::Service dynDNSService() const;
    void setDynDNSService(DNS::Service value);
    QString dynDomainName() const;
    void setDynDomainName(const QString& value);
    QString dynDNSUsername() const;
    void setDynDNSUsername(const QString& value);
    QString dynDNSPassword() const;
    void setDynDNSPassword(const QString& value);

    // Advanced settings
    QByteArray uiLockPassword() const;
    void setUiLockPassword(const QByteArray& value);
    bool uiLocked() const;
    void setUiLocked(bool value);
    bool autoRunEnabled() const;
    void setAutoRunEnabled(bool value);
    QString autoRunProgram() const;
    void setAutoRunProgram(const QString& value);
    bool shutdownWhenDownloadsComplete() const;
    void setShutdownWhenDownloadsComplete(bool value);
    bool suspendWhenDownloadsComplete() const;
    void setSuspendWhenDownloadsComplete(bool value);
    bool hibernateWhenDownloadsComplete() const;
    void setHibernateWhenDownloadsComplete(bool value);
    bool shutdownqBTWhenDownloadsComplete() const;
    void setShutdownqBTWhenDownloadsComplete(bool value);
    bool dontConfirmAutoExit() const;
    void setDontConfirmAutoExit(bool value);
    bool recheckTorrentsOnCompletion() const;
    void setRecheckTorrentsOnCompletion(bool value);
    bool resolvePeerCountries() const;
    void setResolvePeerCountries(bool value);
    bool resolvePeerHostNames() const;
    void setResolvePeerHostNames(bool value);

#if (defined(Q_OS_UNIX) && !defined(Q_OS_MAC))
    bool useSystemIconTheme() const;
    void setUseSystemIconTheme(bool value);
#endif
    bool recursiveDownloadDisabled() const;
    void setRecursiveDownloadDisabled(bool value);

#ifdef Q_OS_WIN
    bool neverCheckFileAssoc() const;
    void setNeverCheckFileAssoc(bool value);

    static bool isTorrentFileAssocSet();
    static bool isMagnetLinkAssocSet();
    static void setTorrentFileAssoc(bool set);
    static void setMagnetLinkAssoc(bool set);
#endif
#ifdef Q_OS_MAC
    static bool isTorrentFileAssocSet();
    static bool isMagnetLinkAssocSet();
    static void setTorrentFileAssoc();
    static void setMagnetLinkAssoc();
#endif
    int trackerPort() const;
    void setTrackerPort(int value);

#if defined(Q_OS_WIN) || defined(Q_OS_MAC)
    bool updateCheckEnabled() const;
    void setUpdateCheckEnabled(bool value);
#endif

    bool confirmTorrentDeletion() const;
    void setConfirmTorrentDeletion(bool value);
    bool confirmTorrentRecheck() const;
    void setConfirmTorrentRecheck(bool value);
    bool confirmRemoveAllTags() const;
    void setConfirmRemoveAllTags(bool value);

#ifndef Q_OS_MAC
    bool systrayIntegration() const;
    void setSystrayIntegration(bool value);

    bool minimizeToTrayNotified() const;
    void setMinimizeToTrayNotified(bool value);

    bool minimizeToTray() const;
    void setMinimizeToTray(bool value);

    bool closeToTray() const;
    void setCloseToTray(bool value);

    bool closeToTrayNotified() const;
    void setCloseToTrayNotified(bool value);

    TrayIcon::Style trayIconStyle() const;
    void setTrayIconStyle(TrayIcon::Style value);

#endif

    // Stuff that don't appear in the Options GUI but are saved
    // in the same file.
    QDateTime dnsLastUpd() const;
    void setDnsLastUpd(const QDateTime& value);
    QString dnsLastIP() const;
    void setDnsLastIP(const QString& value);
    bool acceptedLegal() const;
    void setAcceptedLegal(bool value);
    QByteArray mainGeometry() const;
    void setMainGeometry(const QByteArray& value);
    QByteArray mainVSplitterState() const;
    void setMainVSplitterState(const QByteArray& value);
    QString mainLastDir() const;
    void setMainLastDir(const QString& value);
    QSize prefSize() const;
    void setPrefSize(QSize value);
    QStringList prefHSplitterSizes() const;
    void setPrefHSplitterSizes(const QStringList& value);
    QByteArray peerListState() const;
    void setPeerListState(const QByteArray& value);
    QString propSplitterSizes() const;
    void setPropSplitterSizes(const QString& value);
    QByteArray propFileListState() const;
    void setPropFileListState(const QByteArray& value);
    int propCurTab() const;
    void setPropCurTab(int value);
    bool propVisible() const;
    void setPropVisible(bool value);
    QByteArray propTrackerListState() const;
    void setPropTrackerListState(const QByteArray& value);
    QSize rssGeometrySize() const;
    void setRssGeometrySize(const QSize& value);
    QByteArray rssHSplitterSizes() const;
    void setRssHSplitterSizes(const QByteArray& value);
    QStringList rssOpenFolders() const;
    void setRssOpenFolders(const QStringList& value);
    QByteArray rssSideSplitterState() const;
    void setRssSideSplitterState(const QByteArray& value);
    QByteArray rssMainSplitterState() const;
    void setRssMainSplitterState(const QByteArray& value);
    QByteArray searchTabHeaderState() const;
    void setSearchTabHeaderState(const QByteArray& value);
    bool regexAsFilteringPatternForSearchJob() const;
    void setRegexAsFilteringPatternForSearchJob(bool value);
    QStringList searchEngDisabled() const;
    void setSearchEngDisabled(const QStringList& value);
    QString torImportLastContentDir() const;
    void setTorImportLastContentDir(const QString& value);

    QByteArray torImportGeometry() const;
    void setTorImportGeometry(const QByteArray& value);
    bool statusFilterState() const;
    bool categoryFilterState() const;
    bool tagFilterState() const;
    bool trackerFilterState() const;
    int transSelFilter() const;
    void setTransSelFilter(int value);
    QByteArray transHeaderState() const;
    void setTransHeaderState(const QByteArray& value);
    bool regexAsFilteringPatternForTransferList() const;
    void setRegexAsFilteringPatternForTransferList(bool value);
    int toolbarTextPosition() const;
    void setToolbarTextPosition(int value);

    // From old RssSettings class
    bool rssWidgetEnabled() const;
    void setRssWidgetEnabled(bool value);

    // Network
    QList<QNetworkCookie> networkCookie() const;
    void setNetworkCookie(const QList<QNetworkCookie>& value);

    // SpeedWidget
    bool speedWidgetEnabled() const;
    void setSpeedWidgetEnabled(bool value);
    int speedWidgetPeriod() const;
    void setSpeedWidgetPeriod(int value);
    bool speedWidgetGraphEnable(int id) const;
    void setSpeedWidgetGraphEnable(int id,bool value);

  public slots:
    void setStatusFilterState(bool checked);
    void setCategoryFilterState(bool checked);
    void setTagFilterState(bool checked);
    void setTrackerFilterState(bool checked);

    void apply();
};
