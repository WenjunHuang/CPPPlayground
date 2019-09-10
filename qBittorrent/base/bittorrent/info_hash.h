//
// Created by rick on 2019/9/11.
//

#pragma once
#include <QString>
#include <libtorrent/sha1_hash.hpp>
#include <libtorrent/version.hpp>

namespace BitTorrent {
    class InfoHash {
      public:
        InfoHash();
        InfoHash(const lt::sha1_hash& nativeHash);
        InfoHash(const QString& hasString);
        InfoHash(const InfoHash& other) = default;

        static constexpr int length() { return lt::sha1_hash::size; }

        bool isValid() const { return m_valid; }
        operator lt::sha1_hash() const { return m_nativeHash; }
        operator QString() const { return m_hashString; }

      private:
        bool m_valid;
        lt::sha1_hash m_nativeHash;
        QString m_hashString;
    };

    bool operator==(const InfoHash& left, const InfoHash& right) {
        return (static_cast<lt::sha1_hash>(left) ==
                static_cast<lt::sha1_hash>(right));
    }
    bool operator!=(const InfoHash& left, const InfoHash& right) {
        return !(left == right);
    }

    uint qHash(const InfoHash& key, uint seed) {
        return qHash(static_cast<QString>(key), seed);
    }
} // namespace BitTorrent
