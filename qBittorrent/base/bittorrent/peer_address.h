//
// Created by HUANG WEN JUN on 2019/9/11.
//

#pragma once
#include <QHostAddress>
#include <QStringRef>
#include <QVector>

namespace BitTorrent {
    struct PeerAddress {
        QHostAddress ip;
        ushort port = 0;

        static PeerAddress parse(const QString& address);
    };
} // namespace BitTorrent
