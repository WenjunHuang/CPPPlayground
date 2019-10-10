//
// Created by rick on 2019/9/11.
//

#include "tracker_entry.h"
BitTorrent::TrackerEntry::TrackerEntry(const QString& url)
    : m_nativeEntry(url.toStdString()) {}
BitTorrent::TrackerEntry::TrackerEntry(const lt::announce_entry& nativeEntry)
    : m_nativeEntry(nativeEntry) {}
