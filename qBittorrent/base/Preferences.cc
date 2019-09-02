//
// Created by HUANG WEN JUN on 2019/8/29.
//

#include "Preferences.h"
#ifdef Q_OS_MAC
#include <CoreServices/CoreServices.h>
#endif
#ifdef Q_OS_WIN
#include <shlobj.h>
#endif

#include "SettingsStorage.h"
#include "algorithm.h"
#include "global.h"
#include "utils/Fs.h"
#include <QTime>

Preferences* Preferences::m_instance = nullptr;

Preferences::Preferences() = default;

Preferences* Preferences::instance() { return m_instance; }

void Preferences::initInstance() {
    if (!m_instance)
        m_instance = new Preferences;
}

void Preferences::freeInstance() {
    if (m_instance) {
        delete m_instance;
        m_instance = nullptr;
    }
}

QVariant Preferences::value(const QString& key,
                            const QVariant& defaultValue) const {
    return SettingsStorage::instance()->loadValue(key, defaultValue);
}

void Preferences::setValue(const QString& key, const QVariant& value) {
    SettingsStorage::instance()->storeValue(key, value);
}

QString Preferences::getLocale() const {
    auto localeName =
        value(QLatin1String("Preferences/General/Locale")).toString();
    return (localeName.isEmpty() ? QLocale::system().name() : localeName);
}

void Preferences::setLocale(const QString& locale) {
    setValue(QLatin1String("Preferences/General/Locale"), locale);
}

bool Preferences::userCustomUITheme() const {
    return value(QLatin1String("Preferences/General/UseCustomUITheme"), false)
               .toBool() &&
           !customUIThemePath().isEmpty();
}

void Preferences::setUseCustomUITheme(bool use) {
    setValue(QStringLiteral("Preferences/General/UseCustomUITheme"), use);
}

QString Preferences::customUIThemePath() const {
    return value(QStringLiteral("Preferences/General/CustomUIThemePath"))
        .toString();
}

void Preferences::setCustomUIThemePath(const QString& path) {
    setValue(QStringLiteral("Preferences/General/CustomUIThemePath"), path);
}

bool Preferences::deleteTorrentFilesAsDefault() const {
    return value(QStringLiteral(
                     "Preferences/General/DeleteTorrentsFilesAsDefault"),
                 false)
        .toBool();
}

void Preferences::setDeleteTorrentFilesAsDefault(bool value) {
    setValue(QStringLiteral("Preferences/General/DeleteTorrentsFilesAsDefault"),
             value);
}

bool Preferences::confirmOnExit() const {
    return value(QStringLiteral("Preferences/General/ExitConfirm"), true)
        .toBool();
}

void Preferences::setConfirmOnExit(bool value) {
    setValue(QStringLiteral("Preferences/General/ExitConfirm"), value);
}

bool Preferences::speedInTitleBar() const {
    return value(QStringLiteral("Preferences/General/SpeedInTitleBar"), false)
        .toBool();
}

void Preferences::showSpeedInTitleBar(bool show) {
    setValue(QStringLiteral("Preferences/General/SpeedInTitleBar"), show);
}

bool Preferences::useAlternatingRowColors() const {
    return value(QStringLiteral("Preferences/General/AlternatingRowColors"),
                 true)
        .toBool();
}

void Preferences::setUseAlternatingRowColors(bool value) {
    setValue(QStringLiteral("Preferences/General/AlternatingRowColors"), value);
}

bool Preferences::hideZeroValues() const {
    return value(QStringLiteral("Preferences/General/HideZeroValues"), false)
        .toBool();
}

void Preferences::setHideZeroValues(bool value) {
    setValue(QStringLiteral("Preferences/General/HideZeroValues"), value);
}

int Preferences::hideZeroComboValues() const {
    return value(QStringLiteral("Preferences/General/HideZeroComboValues"), 0)
        .toInt();
}

void Preferences::setHideZeroComboValues(int value) {
    setValue(QStringLiteral("Preferences/General/HideZeroComboValues"), value);
}

#ifndef Q_OS_MAC
bool Preferences::systrayIntegration() const {
    return value(QStringLiteral("Preferences/General/SystrayEnabled"), true)
        .toBool();
}

void Preferences::setSystrayIntegration(bool value) {
    setValue(QStringLiteral("Preferences/General/SystrayEnabled"), value);
}

bool Preferences::minimizeToTray() const {
    return value("Preferences/General/MinimizeToTray", false).toBool();
}

void Preferences::setMinimizeToTray(bool value) {
    setValue(QStringLiteral("Preferences/General/MinimizeToTray"), value);
}

bool Preferences::minimizeToTrayNotified() const {
    return value(QStringLiteral("Preferences/General/MinimizeToTrayNotified"),
                 false)
        .toBool();
}

void Preferences::setMinimizeToTrayNotified(bool value) {
    setValue(QStringLiteral("Preferences/General/MinimizeToTrayNotified"),
             value);
}

bool Preferences::closeToTray() const {
    return value(QStringLiteral("Preferences/General/CloseToTray"), true)
        .toBool();
}

void Preferences::setCloseToTray(bool value) {
    setValue(QStringLiteral("Preferences/General/CloseToTray"), value);
}

bool Preferences::closeToTrayNotified() const {
    return value(QStringLiteral("Preferences/General/CloseToTrayNotified"),
                 false)
        .toBool();
}

void Preferences::setCloseToTrayNotified(bool value) {
    setValue(QStringLiteral("Preferences/General/CloseToTrayNotified"), value);
}
#endif // Q_OS_MAC

bool Preferences::toolbarDisplayed() const {
    return value(QStringLiteral("Preferences/General/ToolbarDisplayed"), true)
        .toBool();
}

void Preferences::setToolbarDisplayed(bool value) {
    setValue(QStringLiteral("Preferences/General/ToolbarDisplayed"), value);
}

bool Preferences::statusbarDisplayed() const {
    return value(QStringLiteral("Preferences/General/StatusbarDisplayed"), true)
        .toBool();
}

bool Preferences::startMinimized() const {
    return value(QStringLiteral("Preferences/General/StartMinimized"), false)
        .toBool();
}

void Preferences::setStartMinimized(bool value) {
    setValue(QStringLiteral("Preferences/General/StartMinimized"), value);
}

bool Preferences::splashScreenDisabled() const {
    return value(QStringLiteral("Preferences/General/NoSplashScreen"), true)
        .toBool();
}

bool Preferences::preventFromSuspendWhenDownloading() const {
    return value(QStringLiteral(
                     "Preferences/General/PreventFromSuspendWhenDownloading"),
                 false)
        .toBool();
}

void Preferences::setPreventFromSuspendWhenDownloading(bool value) {
    setValue(
        QStringLiteral("Preferences/General/PreventFromSuspendWhenDownloading"),
        value);
}

bool Preferences::preventFromSuspendWhenSeeding() const {
    return value(QStringLiteral(
                     "Preferences/General/PreventFromSuspendWhenSeeding"),
                 false)
        .toBool();
}

void Preferences::setPreventFromSuspendWhenSeeding(bool value) {
    setValue(
        QStringLiteral("Preferences/General/PreventFromSuspendWhenSeeding"),
        value);
}

#ifdef Q_OS_WIN
bool Preferences::winStartup() const {
    QSettings settings(
        R"(HKEY_CURRENT_USER\Software\Microsoft\Windows\CurrentVersion\Run)",
        QSettings::NativeFormat);
    return settings.contains("qBittorrent");
}

void Preferences::setWinStartup(bool value) {
    QSettings settings(
        R"(HKEY_CURRENT_USER\Software\Microsoft\Windows\CurrentVersion\Run)",
        QSettings::NativeFormat);
    if (value) {
        auto binPath = QString("\"%1\"").arg(
            Utils::Fs::toNativePath(qApp->applicationFilePath()));
        settings.setValue("qBittorrent", binPath);
    } else {
        settings.remove("qBittorrent");
    }
}
#endif // Q_OS_WIN

// Downloads
QString Preferences::lastLocationPath() const {
    return Utils::Fs::toUniformPath(
        value(QStringLiteral("Preferences/Downloads/LastLocationPath"))
            .toString());
}

void Preferences::setLastLocationPath(const QString& value) {
    setValue(QStringLiteral("Preferences/Downloads/LastLocationPath"),
             Utils::Fs::toUniformPath(value));
}

QVariantHash Preferences::scanDirs() const {
    return value(QStringLiteral("Preferences/Downloads/ScanDirsV2")).toHash();
}

// This must be called somewhere with data from the model
void Preferences::setScanDirs(const QVariantHash& value) {
    setValue(QStringLiteral("Preferences/Downloads/ScanDirsV2"), value);
}

QString Preferences::scanDirsLastPath() const {
    return Utils::Fs::toUniformPath(
        value(QStringLiteral("Preferences/Downloads/ScanDirsLastPath"))
            .toString());
}

void Preferences::setScanDirsLastPath(const QString& value) {
    setValue(QStringLiteral("Preferences/Downloads/ScanDirsLastPath"),
             Utils::Fs::toUniformPath(value));
}

bool Preferences::mailNotificationEnabled() const {
    return value(QStringLiteral("Preferences/MailNotification/enabled"), false)
        .toBool();
}

void Preferences::setMailNotificationEnabled(bool value) {
    setValue(QStringLiteral("Preferences/MailNotification/enabled"), value);
}

QString Preferences::mailNotificationSender() const {
    return value(QStringLiteral("Preferences/MailNotification/sender"),
                 QStringLiteral("qBittorent_notification@example.com"))
        .toString();
}

void Preferences::setMailNotificationSender(const QString& value) {
    setValue(QStringLiteral("Preferences/MailNotification/sender"), value);
}

QString Preferences::mailNotificationEmail() const {
    return value(QStringLiteral("Preferences/MailNotification/email"))
        .toString();
}

void Preferences::setMailNotificationEmail(const QString& value) {
    setValue(QStringLiteral("Preferences/MailNotification/email"), value);
}

QString Preferences::mailNotificationSMTP() const {
    return value(QStringLiteral("Preferences/MailNotification/smtp_server"),
                 QStringLiteral("smtp.changeme.com"))
        .toString();
}

void Preferences::setMailNotificationSMTP(const QString& value) {
    setValue(QStringLiteral("Preferences/MailNotification/smtp_server"), value);
}

bool Preferences::mailNotificationSMTPSSL() const {
    return value(QStringLiteral("Preferences/MailNotification/req_ssl"), false)
        .toBool();
}

bool Preferences::mailNotificationSMTPAuth() const {
    return value(QStringLiteral("Preferences/MailNotification/req_auth"), false)
        .toBool();
}

void Preferences::setMailNotificationSMTPAuth(bool value) {
    setValue(QStringLiteral("Preferences/MailNotification/req_auth"), value);
}

QString Preferences::mailNotificationSMTPUsername() const {
    return value(QStringLiteral("Preferences/MailNotification/username"))
        .toString();
}

void Preferences::setMailNotificationSMTPUsername(const QString& value) {
    setValue(QStringLiteral("Preferences/MailNotification/username"), value);
}

QString Preferences::mailNotificationSMTPPassword() const {
    return value(QStringLiteral("Preferences/MailNotification/password"))
        .toString();
}

void Preferences::setMailNotificationSMTPPassword(const QString& value) {
    setValue(QStringLiteral("Preferences/MailNotification/password"), value);
}

int Preferences::actionOnDblClOnTorrentDl() const {
    return value(QStringLiteral("Preferences/Downloads/DblClOnTorDl"), 0)
        .toInt();
}

void Preferences::setActionOnDblClOnTorrentDl(int value) {
    setValue(QStringLiteral("Preferences/Downloads/DblClOnTorDl"), value);
}

int Preferences::actionOnDblClOnTorrentFn() const {
    return value(QStringLiteral("Preferences/Download/DblClOnTorFn"), 1)
        .toInt();
}

QTime Preferences::schedulerStartTime() const {
    return value(QStringLiteral("Preferences/Scheduler/start_time"),
                 QTime(8, 0))
        .toTime();
}

void Preferences::setSchedulerStartTime(const QTime& value) {
    setValue(QStringLiteral("Preferences/Scheduler/start_time"), value);
}

QTime Preferences::schedulerEndTime() const {
    return value(QStringLiteral("Preferences/Scheduler/end_time"), QTime(20, 0))
        .toTime();
}

void Preferences::setSchedulerEndTime(const QTime& value) {
    setValue(QStringLiteral("Preferences/Scheduler/end_time"), value);
}

SchedulerDays Preferences::schedulerDays() const {
    return static_cast<SchedulerDays>(
        value(QStringLiteral("Preferences/Scheduler/days"), EVERY_DAY).toInt());
}

void Preferences::setSchedulerDays(SchedulerDays value) {
    setValue(QStringLiteral("Preferences/Scheduler/days"),
             static_cast<int>(value));
}

// Search
bool Preferences::searchEnabled() const {
    return value(QStringLiteral("Preferences/Search/SearchEnabled"), false)
        .toBool();
}

void Preferences::setSearchEnabled(bool value) {
    setValue(QStringLiteral("Preferences/Search/SearchEnabled"), value);
}

bool Preferences::webUiEnabled() const {
#ifdef DISABLE_GUI
    return true;
#else
    return value(QStringLiteral("Preferences/WebUI/Enabled"), false).toBool();

#endif
}

void Preferences::setWebUiEnabled(bool value) {
    setValue(QStringLiteral("Preferences/WebUI/Enabled"), value);
}

bool Preferences::webUiLocalAuthEnabled() const {
    return value(QStringLiteral("Preferences/WebUI/LocalHostAuth"), true)
        .toBool();
}

void Preferences::setWebUiLocalAuthEnabled(bool value) {
    setValue(QStringLiteral("Preferences/WebUI/LocalHostAuth"), value);
}

bool Preferences::webUiAuthSubnetWhitelistEnabled() const {
    return value(QStringLiteral("Preferences/WebUI/AuthSubnetWhitelistEnabled"),
                 false)
        .toBool();
}

void Preferences::setWebUiAuthSubnetWhitelistEnabled(bool value) {
    setValue(QStringLiteral("Preferences/WebUI/AuthSubnetWhitelistEnabled"),
             value);
}

QVector<Utils::Net::Subnet> Preferences::webUiAuthSubnetWhitelist() const {
    const auto subnets =
        value(QStringLiteral("Preferences/WebUI/AuthSubnetWhiteList"))
            .toStringList();
    QVector<Utils::Net::Subnet> ret;
    ret.reserve(subnets.size());

    for (const auto& rawSubnet : subnets) {
        auto result = Utils::Net::parseSubnet(rawSubnet.trimmed());
        if (result.has_value())
            ret.append(std::move(result).value());
    }

    return ret;
}

void Preferences::setWebUiAuthSubnetWhitelist(QStringList subnets) {
    Algorithm::removeIf(subnets, [](const QString& subnet) {
        return !Utils::Net::parseSubnet(subnet.trimmed()).has_value();
    });

    setValue(QStringLiteral("Preferences/WebUI/AuthSubnetWhitelist"), subnets);
}

QString Preferences::serverDomains() const {
    return value(QStringLiteral("Preferences/WebUI/ServerDomains"), QChar('*'))
        .toString();
}

void Preferences::setServerDomains(const QString& value) {
    setValue(QStringLiteral("Preferences/WebUI/ServerDomains"), value);
}

QString Preferences::webUiAddress() const {
    return value(QStringLiteral("Preferences/WebUI/Address"), QChar('*'))
        .toString()
        .trimmed();
}

void Preferences::setWebUiAddress(const QString& value) {
    setValue(QStringLiteral("Preferences/WebUI/Address"), value.trimmed());
}

quint16 Preferences::webUiPort() const {
    return value(QStringLiteral("Preferences/WebUI/Port"), 8080).toInt();
}

void Preferences::setWebUiPort(quint16 value) {
    setValue(QStringLiteral("Preferences/WebUI/Port"), value);
}

bool Preferences::useUPnPForWebUiPort() const {
#ifdef DISABLE_GUI
    return value(QStringLiteral("Preferences/WebUI/UseUPnP"), true).toBool();
#else
    return value(QStringLiteral("Preferences/WebUI/UseUPnP"), false).toBool();
#endif
}

void Preferences::setUseUPnPForWebUiPort(bool value) {
    setValue(QStringLiteral("Preferences/WebUI/UseUPnP"), value);
}

QString Preferences::webUiUsername() const {
    return value(QStringLiteral("Preferences/WebUI/Username"),
                 QStringLiteral("admin"))
        .toString();
}

void Preferences::setWebUiUsername(const QString& value) {
    setValue(QStringLiteral("Preferences/WebUI/Username"), value);
}

QByteArray Preferences::webUiPassword() const {
    // default: adminadmin
    const QByteArray defaultValue =
        "ARQ77eY1NUZaQsuDHbIMCA==:"
        "0WMRkYTUWVT9wVvdDtHAjU9b3b7uB8NR1Gur2hmQCvCDpm39Q+PsJRJPaCU51dEiz+"
        "dTzh8qbPsL8WkFljQYFQ==";
    return value(QStringLiteral("Preferences/WebUI/Password_PBKDF2"),
                 defaultValue)
        .toByteArray();
}

void Preferences::setWebUiPassword(const QByteArray& value) {
    setValue(QStringLiteral("Preferences/WebUI/Password_PBKDF2"), value);
}

int Preferences::webUISessionTimeout() const {
    return value(QStringLiteral("Preferences/WebUI/SessionTimeout"), 3600)
        .toInt();
}

void Preferences::setWebUISessionTimeout(int value) {
    setValue(QStringLiteral("Preferences/WebUI/SessionTimeout"), value);
}

bool Preferences::webUIClickjackingProtectionEnabled() const {
    return value(QStringLiteral("Preferences/WebUI/ClickjackingProtection"),
                 true)
        .toBool();
}

void Preferences::setWebUIClickjackingProtectionEnabled(bool value) {
    setValue(QStringLiteral("Preferences/WebUI/ClickjackingProtection"), value);
}

bool Preferences::webUICSRFProtectionEnabled() const {
    return value(QStringLiteral("Preferences/WebUI/CSRFProtection"), true)
        .toBool();
}

void Preferences::setWebUICSRFProtectionEnabled(bool value) {
    setValue(QStringLiteral("Preferences/WebUI/CSRFProtection"), value);
}

bool Preferences::webUIHostHeaderValidationEnabled() const {
    return value(QStringLiteral("Preferences/WebUI/HostHeaderValidation"), true)
        .toBool();
}

void Preferences::setWebUIHostHeaderValidationEnabled(bool value) {
    setValue(QStringLiteral("Preferences/WebUI/HostHeaderValidation"), value);
}

bool Preferences::webUIHttpsEnabled() const {
    return value(QStringLiteral("Preferences/WebUI/HTTPS/Enabled"), false)
        .toBool();
}

void Preferences::setWebUIHttpsEnabled(bool value) {
    setValue(QStringLiteral("Preferences/WebUI/HTTPS/Enabled"), value);
}

QString Preferences::webUIHttpsCertificatePath() const {
    return value(QStringLiteral("Preferences/WebUI/HTTPS/CertificatePath"))
        .toString();
}

void Preferences::setWebUIHttpsCertificatePath(const QString& value) {
    setValue(QStringLiteral("Preferences/WebUI/HTTPS/CertificatePath"), value);
}

QString Preferences::webUIHttpsKeyPath() const {
    return value(QStringLiteral("Preferences/WebUI/HTTPS/KeyPath")).toString();
}

void Preferences::setWebUIHttpsKeyPath(const QString& value) {
    setValue(QStringLiteral("Preferences/WebUI/HTTPS/KeyPath"), value);
}

bool Preferences::altWebUIEnabled() const {
    return value(QStringLiteral("Preferences/WebUI/AlternativeUIEnabled"),false).toBool();
}

void Preferences::setAltWebUIEnabled(bool value) {
    setValue(QStringLiteral("Preferences/WebUI/AlternativeUIEnabled"),value);
}

QString Preferences::webUIRootFolder() const {
    return value(QStringLiteral("Preferences/WebUI/RootFolder")).toString();
}

void Preferences::setWebUIRootFolder(const QString& value) {
    setValue(QStringLiteral("Preferences/WebUI/RootFolder"),value);
}

bool Preferences::dynDNSEnabled() const {
    return value(QStringLiteral("Preferences/DynDNS/Enabled"),false).toBool();
}

void Preferences::setDynDNSEnabled(bool value) {
    setValue(QStringLiteral("Preferences/DynDNS/Enabled"),value);
}

DNS::Service Preferences::dynDNSService() const {
    return DNS::Service(value(QStringLiteral("Preferences/DynDNS/Service"),DNS::DYNDNS).toInt());
}

void Preferences::setDynDNSService(DNS::Service value) {
    setValue(QStringLiteral("Preferences/DynDNS/Service"),static_cast<int>(value));
}

QString Preferences::dynDomainName() const {
    return value(QStringLiteral("Preferences/DynDNS/DomainName"),QStringLiteral("changeme.dyndns.org")).toString();
}

void Preferences::setDynDomainName(const QString& value) {
    setValue(QStringLiteral("Preferences/DynDNS/DomainName"),value);
}

QString Preferences::dynDNSUsername() const {
    return value(QStringLiteral("Preferences/DynDNS/Username")).toString();
}

void Preferences::setDynDNSUsername(const QString& value) {
    setValue(QStringLiteral("Preferences/DynDNS/Username"),value);
}

QString Preferences::dynDNSPassword() const {
    return value(QStringLiteral("Preferences/DynDNS/Password")).toString();
}

void Preferences::setDynDNSPassword(const QString& value) {
    setValue(QStringLiteral("Preferences/DynDNS/Password"),value);
}

// Advanced settings
QByteArray Preferences::uiLockPassword() const {
    return value(QStringLiteral("Locking/password_PBKDF2")).toByteArray();
}

void Preferences::setUiLockPassword(const QByteArray& value) {
    setValue(QStringLiteral("Locking/password_PBKDF2"),value);
}

bool Preferences::uiLocked() const {
    return value(QStringLiteral("Locking/locked"),false).toBool();
}

void Preferences::setUiLocked(bool value) {
    setValue(QStringLiteral("Locking/locked"),value);
}

bool Preferences::autoRunEnabled() const {
    return value(QStringLiteral("AutoRun/enabled"),false).toBool();
}

void Preferences::setAutoRunEnabled(bool value) {
    setValue(QStringLiteral("AutoRun/enabled"),value);
}

QString Preferences::autoRunProgram() const {
    return value(QStringLiteral("AutoRun/program")).toString();
}

void Preferences::setAutoRunProgram(const QString& value) {
    setValue(QStringLiteral("AutoRun/program"),value);
}

bool Preferences::shutdownWhenDownloadsComplete() const {
    return value(QStringLiteral("Preferences/Downloads/AutoShutDownOnCompletion"),false).toBool();
}

void Preferences::setShutdownWhenDownloadsComplete(bool value) {
    setValue(QStringLiteral("Preferences/Downloads/AutoShutDownOnCompletion"),value);
}

bool Preferences::suspendWhenDownloadsComplete() const {
    return value(QStringLiteral("Preferences/Downloads/AutoSuspendOnCompletion"),false).toBool();
}

void Preferences::setSuspendWhenDownloadsComplete(bool value) {
    setValue(QStringLiteral("Preferences/Downloads/AutoSuspendOnCompletion"),value);
}

bool Preferences::hibernateWhenDownloadsComplete() const {
    return value(QStringLiteral("Preferences/Downloads/AutoHibernateOnCompletion"),false).toBool();
}

void Preferences::setHibernateWhenDownloadsComplete(bool value) {
    setValue(QStringLiteral("Preferences/Downloads/AutoHibernateOnCompletion"),value);
}

bool Preferences::shutdownqBTWhenDownloadsComplete() const {
    return value(QStringLiteral("Preferences/Downloads/AutoShutDownqBTOnCompletion"),false).toBool();
}

void Preferences::setShutdownqBTWhenDownloadsComplete(bool value) {
    setValue(QStringLiteral("Preferences/Downloads/AutoShutDownqBTOnCompletion"),value);
}

bool Preferences::dontConfirmAutoExit() const {
    return value(QStringLiteral("ShutdownConfirmDlg/DontConfirmAutoExit"),false).toBool();
}

void Preferences::setDontConfirmAutoExit(bool value) {
    setValue(QStringLiteral("ShutdownConfirmDlg/DontConfirmAutoExit"),value);
}

bool Preferences::recheckTorrentsOnCompletion() const {
    return value(QStringLiteral("Preferences/Advanced/RecheckOnCompletion"),false).toBool();
}

void Preferences::setRecheckTorrentsOnCompletion(bool value) {
    setValue(QStringLiteral("Preferences/Advanced/RecheckOnCompletion"),value);
}

bool Preferences::resolvePeerCountries() const {
    return value(QStringLiteral("Preferences/Connection/ResolvePeerCountries"),true).toBool();
}

void Preferences::setResolvePeerCountries(bool value) {
    setValue(QStringLiteral("Preferences/Connection/ResolvePeerCountries"),value);
}

bool Preferences::resolvePeerHostNames() const {
    return value(QStringLiteral("Preferences/Connection/ResolvePeerHostNames"),false).toBool();
}

void Preferences::setResolvePeerHostNames(bool value) {
    setValue(QStringLiteral("Preferences/Connection/ResolvePeerHostNames"),value);
}

#if (defined(Q_OS_UNIX)&& !defined(Q_OS_MAC))
bool Preferences::useSystemIconTheme() const {
    return value(QStringLiteral("Preferences/Advanced/useSystemIconTheme"),true).toBool();
}

void Preferences::setUseSystemIconTheme(bool value) {
    setValue(QStringLiteral("Preferences/Advanced/useSystemIconTheme"),value);
}
#endif

bool Preferences::recursiveDownloadDisabled() const {
    return value(QStringLiteral("Preferences/Advanced/DisableRecursiveDownload"),false).toBool();
}

void Preferences::setRecursiveDownloadDisabled(bool value) {
    setValue(QStringLiteral("Preferences/Advanced/DisableRecursiveDownload"),value);
}

#ifdef Q_OS_WIN
bool Preferences::neverCheckFileAssoc() const {
    return value(QStringLiteral("Preferences/Win32/NeverCheckFileAssocation"),false).toBool();
}

void Preferences::setNeverCheckFileAssoc(bool value) {
    setValue(QStringLiteral("Preferences/Win32/NeverCheckFileAssocation"),value);
}

bool Preferences::isTorrentFileAssocSet() {
    const QSettings settings("HKEY_CURRENT_USER\\Software\\Classes",QSettings::NativeFormat);
    if (settings.value(QStringLiteral(".torrent/Default")).toString() != QStringLiteral("qBittorrent")) {
        qDebug(QStringLiteral(".torrent != qBittorrent"));
        return false;
    }
    return true;
}

bool Preferences::isMagnetLinkAssocSet() {
    const QSettings settings("HKEY_CURRENT_USER\\Software\\Classes",QSettings::NativeFormat);

    // Check magnet link assoc
    const QString shellCommand =
}
#endif