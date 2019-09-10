//
// Created by rick on 2019/9/11.
//

#include "download_priority.h"
bool BitTorrent::isValidDownloadPriority(
    BitTorrent::DownloadPriority priority) {
    switch (priority) {
    case BitTorrent::DownloadPriority::Ignored:
    case BitTorrent::DownloadPriority::Normal:
    case BitTorrent::DownloadPriority::High:
    case BitTorrent::DownloadPriority::Maximum:
    case BitTorrent::DownloadPriority::Mixed: return true;
    default: return false;
    }
}
