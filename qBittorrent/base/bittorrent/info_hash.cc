//
// Created by rick on 2019/9/11.
//

#include "info_hash.h"
BitTorrent::InfoHash::InfoHash() : m_valid(false) {}

BitTorrent::InfoHash::InfoHash(const lt::sha1_hash& nativeHash)
    : m_valid(true), m_nativeHash(nativeHash) {
    const QByteArray raw = QByteArray::fromRawData(nativeHash.data(), length());
    m_hashString         = QString::fromLatin1(raw.toHex());
}

BitTorrent::InfoHash::InfoHash(const QString& hashString) : m_valid(false) {
    if (hashString.size() != (length() * 2))
        return;

    const QByteArray raw = QByteArray::fromHex(hashString.toLatin1());
    m_valid              = true;
    m_hashString         = hashString;
    m_nativeHash.assign(raw.constData());
}
