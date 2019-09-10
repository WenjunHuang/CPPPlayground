//
// Created by rick on 2019/9/11.
//

#pragma once

namespace BitTorrent {
    enum class DownloadPriority:int {
        Ignored = 0,
        Normal = 1,
        High = 6,
        Maximum = 7,
        Mixed = -1
    };

    bool isValidDownloadPriority(DownloadPriority priority);
}
