//
// Created by HUANG WEN JUN on 2019/8/30.
//

#include "ProxyConfigurationManager.h"
#include <SettingsStorage.h>

#define SETTINGS_KEY(name) QStringLiteral("Network/Proxy/" name)
const QString KEY_ONLY_FOR_TORRENTS = SETTINGS_KEY("OnlyForTorrents");
const QString KEY_TYPE              = SETTINGS_KEY("Type");
const QString KEY_IP                = SETTINGS_KEY("IP");
const QString KEY_PORT              = SETTINGS_KEY("Port");
const QString KEY_USERNAME          = SETTINGS_KEY("Username");
const QString KEY_PASSWORD          = SETTINGS_KEY("Password");

namespace {
    inline SettingsStorage* settings() { return SettingsStorage::instance(); }
} // namespace

bool Net::operator==(const Net::ProxyConfiguration& left,
                     const Net::ProxyConfiguration& right) {
    return (left.type == right.type) && (left.ip == right.ip) &&
           (left.port == right.port) && (left.username == right.username) &&
           (left.password == right.password);
}

bool Net::operator!=(const Net::ProxyConfiguration& left,
                     const Net::ProxyConfiguration& right) {
    return !(left == right);
}

using namespace Net;

ProxyConfigurationManager* ProxyConfigurationManager::m_instance = nullptr;

ProxyConfigurationManager::ProxyConfigurationManager(QObject* parent)
    : QObject(parent) {
    m_isProxyOnlyForTorrents =
        settings()->loadValue(KEY_ONLY_FOR_TORRENTS, false).toBool();
    m_config.type = static_cast<ProxyType>(
        settings()
            ->loadValue(KEY_TYPE, static_cast<int>(ProxyType::None))
            .toInt());

    if ((m_config.type < ProxyType::None) ||
        (m_config.type > ProxyType::SOCKS4))
        m_config.type = ProxyType ::None;

    m_config.ip = settings()->loadValue(KEY_IP, "0.0.0.0").toString();
    m_config.port =
        static_cast<ushort>(settings()->loadValue(KEY_PORT, 8080).toUInt());
    m_config.username = settings()->loadValue(KEY_USERNAME).toString();
    m_config.password = settings()->loadValue(KEY_PASSWORD).toString();
    configureProxy();
}

void ProxyConfigurationManager::initInstance() {
    if (!m_instance)
        m_instance = new ProxyConfigurationManager;
}

void ProxyConfigurationManager::freeInstance() {
    if (m_instance) {
        delete m_instance;
        m_instance = nullptr;
    }
}

ProxyConfigurationManager* ProxyConfigurationManager::instance() {
    return m_instance;
}

ProxyConfiguration ProxyConfigurationManager::proxyConfiguration() const {
    return m_config;
}

void ProxyConfigurationManager::setProxyConfiguration(
    const Net::ProxyConfiguration& config) {
    if (config != m_config) {
        m_config = config;
        settings()->storeValue(KEY_TYPE, static_cast<int>(config.type));
        settings()->storeValue(KEY_IP, config.ip);
        settings()->storeValue(KEY_USERNAME, config.username);
        settings()->storeValue(KEY_PASSWORD, config.password);
        configureProxy();

        emit proxyConfigurationChanged();
    }
}

bool ProxyConfigurationManager::proxyOnlyForTorrents() const {
    return m_isProxyOnlyForTorrents || (m_config.type == ProxyType::SOCKS4);
}

void ProxyConfigurationManager::setProxyOnlyForTorrents(bool value) {
    if (m_isProxyOnlyForTorrents != value) {
        settings()->storeValue(KEY_ONLY_FOR_TORRENTS, value);
        m_isProxyOnlyForTorrents = value;
    }
}

bool ProxyConfigurationManager::authenticationRequired() const {
    return m_config.type == ProxyType ::SOCKS5_PW ||
           m_config.type == ProxyType ::HTTP_PW;
}

void ProxyConfigurationManager::configureProxy() {
    QString proxyStrHTTP, proxyStrSOCK;
    if (!m_isProxyOnlyForTorrents) {
        switch (m_config.type) {
        case ProxyType ::HTTP_PW:
            proxyStrHTTP =
                QString(QLatin1String("http://%1:%2@%3:%4"))
                    .arg(m_config.username, m_config.password, m_config.ip,
                         QString::number(m_config.port));
            break;
        case ProxyType::HTTP:
            proxyStrHTTP =
                QString(QLatin1String("http://%1:%2"))
                    .arg(m_config.ip, QString::number(m_config.port));
            break;
        case ProxyType ::SOCKS5:
            proxyStrSOCK =
                QString(QLatin1String("%1:%2"))
                    .arg(m_config.ip, QString::number(m_config.port));
            break;
        case ProxyType::SOCKS5_PW:
            proxyStrSOCK =
                QString(QLatin1String("%1:%2@%3:%4"))
                    .arg(m_config.username, m_config.password, m_config.ip,
                         QString::number(m_config.port));
            break;
        default:
            qDebug("Disabling HTTP communications proxy");
        }

        qDebug("HTTP communications proxy string: %s",
            qUtf8Printable((m_config.type == ProxyType::SOCKS5) || (m_config.type==ProxyType::SOCKS5_PW)?proxyStrSOCK:proxyStrHTTP));
    }

    qputenv("http_proxy",proxyStrHTTP.toLocal8Bit());
    qputenv("https_proxy",proxyStrHTTP.toLocal8Bit());
    qputenv("sock_proxy",proxyStrSOCK.toLocal8Bit());
}