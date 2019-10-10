//
// Created by HUANG WEN JUN on 2019/9/11.
//

#include "peer_address.h"
BitTorrent::PeerAddress BitTorrent::PeerAddress::parse(const QString& address) {
    QVector<QStringRef> ipPort;

    if (address.startsWith('[') && address.contains("]:")) {
        ipPort = address.splitRef("]:");
        ipPort[0] = ipPort[0].mid(1);
    } else if (address.contains(':')) {
        ipPort = address.splitRef(':');
    } else
        return {};

    const QHostAddress ip{ipPort[0].toString()};
    if (ip.isNull())
        return {};

    const ushort port{ipPort[1].toUShort()};
    if (port == 0)
        return {};

    return {ip,port};
}
