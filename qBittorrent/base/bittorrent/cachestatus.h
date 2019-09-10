//
// Created by rick on 2019/9/11.
//
#pragma  once
#include <QtGlobal>

namespace BitTorrent {
    struct CacheStatus {
        quint64 totalUsedBuffers = 0;
        quint64 jobQueueLength = 0;
        quint64 averageJobTime = 0;
        quint64 queueBytes = 0;
        qreal readRatio = 0.0;
    };
}
