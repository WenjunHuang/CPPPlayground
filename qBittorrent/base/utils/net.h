//
// Created by rick on 2019/8/30.
//

#pragma once
#include <QHostAddress>
#include <QPair>

class QSslCertificate;
class QSslKey;
class QString;

namespace Utils::Net {
    using Subnet = QPair<QHostAddress, int>;

    bool isValidIP(const QString& ip);
    std::optional<Subnet> parseSubnet(const QString& subnetStr);
    bool canParseSubnet(const QString& subnetStr);
    bool isLoopbackAddress(const QHostAddress& addr);
    bool isIPInRange(const QHostAddress& addr, const QVector<Subnet>& subnets);
    QString subnetToString(const Subnet& subnet);

    const int kMaxSSLFileSize = 1024 * 1024;
    QList<QSslCertificate> loadSSLCertificate(const QByteArray& data);
    bool isSSLCertificatesValid(const QByteArray& data);
    QSslKey loadSSLKey(const QByteArray& data);
    bool isSSLKeyValid(const QByteArray& data);
} // namespace Utils::Net

class net {};
