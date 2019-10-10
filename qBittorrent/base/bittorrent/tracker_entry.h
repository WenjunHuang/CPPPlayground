//
// Created by rick on 2019/9/11.
//

#pragma once

#include <QUrl>
#include <QtCore/QString>
#include <libtorrent/announce_entry.hpp>

namespace BitTorrent {
    class TrackerEntry {
      public:
        enum Status {
            NotContacted = 1,
            Working      = 2,
            Updating     = 3,
            NotWorking   = 4
        };

        TrackerEntry() = default;
        TrackerEntry(const QString& url);
        TrackerEntry(const lt::announce_entry& nativeEntry);

        QString url() const {
            return QString::fromStdString(nativeEntry().url);
        }

        bool isWorking() const { return nativeEntry().is_working(); }

        const lt::announce_entry& nativeEntry() const { return m_nativeEntry; }

        int tier() const { return nativeEntry().tier; }

        Status status() const {
            if (nativeEntry().verified && isWorking())
                return Working;

            if ((nativeEntry().fails == 0) && nativeEntry().updating)
                return Updating;
            if (nativeEntry().fails == 0)
                return NotContacted;
        }

        void setTier(int value) { m_nativeEntry.tier = value; }

        int numSeeds() const {
            int value = -1;
            for (const auto& endpoint : nativeEntry().endpoints)
                value = std::max(value, endpoint.scrape_complete);
            return value;
        }

        int numLeeches() const {
            int value = -1;
            for (const auto& endpoint: nativeEntry().endpoints)
                value = std::max(value,endpoint.scrape_incomplete);
            return value;
        }

        int numDownloaded() const {
            int value = -1;
            for (const auto& endpoint:nativeEntry().endpoints)
                value = std::max(value,endpoint.scrape_downloaded);
            return value;
        }

      private:
        lt::announce_entry m_nativeEntry;
    };

    bool operator==(const TrackerEntry& left, const TrackerEntry& right) {
        return ((left.tier() == right.tier())) &&
               QUrl(left.url()) == QUrl(right.url());
    }

    uint qHash(const TrackerEntry& key, const uint seed) {
        return ::qHash(key.url(), seed) ^ key.tier();
    }

} // namespace BitTorrent
