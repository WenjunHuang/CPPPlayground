//
// Created by rick on 2019/9/11.
//

#pragma once
#include "info_hash.h"
#include "tracker_entry.h"
#include <QRegularExpression>
#include <QString>
#include <QUrl>
#include <QVector>
#include <libtorrent/add_torrent_params.hpp>
#include <libtorrent/bencode.hpp>
#include <libtorrent/magnet_uri.hpp>

class QUrl;
namespace BitTorrent {

    bool isBitTorrentInfoHash(const QString& string);
    class MagnetUri {
      public:
        explicit MagnetUri(const QString& source = {});

        bool isValid() const { return m_valid; }

        InfoHash hash() const { return m_hash; }

        QString name() const { return m_name; }

        QVector<TrackerEntry> trackers() const { return m_trackers; }

        QVector<QUrl> urlSeeds() const { return m_urlSeeds; }

        QString url() const { return m_url; }

        lt::add_torrent_params addTorrentParams() const;

      private:
        bool m_valid;
        QString m_url;
        InfoHash m_hash;
        QString m_name;
        QVector<TrackerEntry> m_trackers;
        QVector<QUrl> m_urlSeeds;
        lt::add_torrent_params m_addTorrentParams;
    };
} // namespace BitTorrent
