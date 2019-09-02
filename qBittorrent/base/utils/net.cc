//
// Created by rick on 2019/8/30.
//

#include "net.h"
#include <QSslCertificate>
#include <QSslKey>
#include <QString>

bool Utils::Net::isValidIP(const QString& ip) {
    return !QHostAddress(ip).isNull();
}

std::optional<Utils::Net::Subnet>
Utils::Net::parseSubnet(const QString& subnetStr) {
    const Subnet invalid = qMakePair(QHostAddress(), -1);
    const Subnet subnet  = QHostAddress::parseSubnet(subnetStr);

    if (subnet == invalid)
        return {};
    else
        return subnet;
}

bool Utils::Net::canParseSubnet(const QString& subnetStr) {
    return parseSubnet(subnetStr).has_value();
}

bool Utils::Net::isLoopbackAddress(const QHostAddress& addr) {
    return (addr == QHostAddress::LocalHost) ||
           (addr == QHostAddress::LocalHostIPv6) ||
           (addr == QHostAddress(QLatin1String("::ffff:127.0.0.1")));
}

bool Utils::Net::isIPInRange(const QHostAddress& addr,
                             const QVector<Subnet>& subnets) {
    QHostAddress protocolEquaivalentAddress;
    bool addrConversionOk = false;
    if (addr.protocol() == QAbstractSocket::IPv4Protocol) {
        // always succeeds
        protocolEquaivalentAddress = QHostAddress(addr.toIPv6Address());
        addrConversionOk           = true;
    } else {
        protocolEquaivalentAddress =
            QHostAddress(addr.toIPv4Address(&addrConversionOk));
    }

    for (auto& subnet : subnets) {
        if (addr.isInSubnet(subnet) ||
            (addrConversionOk && protocolEquaivalentAddress.isInSubnet(subnet)))
            return true;
    }

    return false;
}

QString Utils::Net::subnetToString(const Utils::Net::Subnet& subnet) {
    return subnet.first.toString() + '/' + QString::number(subnet.second);
}

QList<QSslCertificate> Utils::Net::loadSSLCertificate(const QByteArray& data) {
    const QList<QSslCertificate> certs{QSslCertificate::fromData(data)};
    if (std::any_of(certs.cbegin(), certs.cend(),
                    [](const QSslCertificate& c) { return c.isNull(); }))
        return {};
    return certs;
}

bool Utils::Net::isSSLCertificatesValid(const QByteArray& data) {
    return !loadSSLCertificate(data).isEmpty();
}

QSslKey Utils::Net::loadSSLKey(const QByteArray& data) {
    QSslKey key {data,QSsl::Rsa};
    if (!key.isNull())
        return key;
    return {data,QSsl::Ec};
}

bool Utils::Net::isSSLKeyValid(const QByteArray& data) {
    return !loadSSLKey(data).isNull();
}