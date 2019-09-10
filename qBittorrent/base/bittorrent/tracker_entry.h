//
// Created by rick on 2019/9/11.
//

#pragma once

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
        TrackerEntry(const QString& url) : m_nativeEntry(url.toStdString()) {}
        TrackerEntry(const lt::announce_entry& nativeEntry)
            : m_nativeEntry(nativeEntry) {}

        QString url() const {
            return QString::fromStdString(nativeEntry().url);
        }

        bool isWorking() const {
            return nativeEntry().is_working();
        }

        lt::announce_entry nativeEntry() const {
            return m_nativeEntry;
        }

        int tier() const {
            return nativeEntry().tier;

        }

        Status status() const {
            if (nativeEntry().verified && isWorking())
                return Working;

            if ((nativeEntry().fails == 0) && nativeEntry().updating)
                return Updating;
            if  (nativeEntry().fails == 0)
                return NotContacted;
        }

      private:
        lt::announce_entry m_nativeEntry;
    };
} // namespace BitTorrent
