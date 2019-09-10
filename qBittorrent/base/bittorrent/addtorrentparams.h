//
// Created by rick on 2019/9/11.
//
#pragma once

#include <QtCore/QSet>
#include <QtCore/QString>
#include <tri_state_bool.h>
namespace BitTorrent {
    enum class DownloadPriority;

    struct AddTorrentParams {
        QString name;
        QString category;
        QSet<QString> tags;
        QString savePath;
        bool disableTempPath        = false;
        bool sequential             = false;
        bool firstLastPiecePriority = false;
        TriStateBool addForced;
        TriStateBool addPaused;
        QVector<DownloadPriority> filePriorities;
        bool ignoreShareLimits = false;
        bool skipChecking      = false;
        TriStateBool createSubfolder;
        TriStateBool useAutoTMM;
        int uploadLimit   = -1;
        int downloadLimit = -1;
    };
} // namespace BitTorrent
