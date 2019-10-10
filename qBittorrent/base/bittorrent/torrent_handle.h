//
// Created by HUANG WEN JUN on 2019/9/11.
//

#pragma once
#include <libtorrent/torrent_handle.hpp>
#include <QCoreApplication>
#include <QDateTime>
#include <QObject>
#include <QSet>
#include <QString>

namespace BitTorrent {
    enum class DownloadPriority;
    class PeerInfo;
    class Session;
    class TrackerEntry;
    struct AddTorrentParams;
    struct PeerAddress;

    struct CreateTorrentParams {
        bool restored;
        QString name;
        QString category;
        QSet<QString> tags;
        QString savePath;
        bool disableTempPath;
        bool sequential;
        bool firstLastPiecePriority;
        bool hasSeedStatus;
        bool skipChecking;
        bool hasRootFolder;
        bool forced;
        bool paused;
        int uploadLimit;
        int downloadLimit;

        QVector<DownloadPriority > filePriorities;
        QDateTime addedTime;
        qreal ratioLimit;
        int seedingTimeLimit;

        CreateTorrentParams();
        explicit CreateTorrentParams(const AddTorrentParams&);
    };

    struct TrackerInfo {
        QString lastMessage;
        int numPeers = 0;
    };

    enum class TorrentState {
        Unknown = -1,

        ForcedDownloading,
        Downloading,
        DownloadingMetadata,
        Allocating,
        StalledDownloading,

        ForcedUploading,
        Uploading,
        StalledUploading,

        CheckingResumeData,
        QueuedDownloading,
        QueueUploading,

        CheckingUploading,
        CheckingDownloading,

        PausedDownloading,
        PausedUploading,

        Moving,

        MissingFiles,
        Error
    };

    class TorrentHandle: public QObject  {
        Q_DISABLE_COPY(TorrentHandle)
        Q_DECLARE_TR_FUNCTIONS(BitTorrent::TorrentHandle)

      public:
        static const qreal kUseGlobalRatio;
        static const qreal kNoRatioLimit;

        static const int kUseGlobalSeedingTime;
        static const int kNoSeedingTimeLimit;

        static const qreal kMaxRatio;
        static const int kMaxSeedingTime;

        TorrentHandle(Session* session,
            const lt::torrent_handle& nativeHandle,
            const CreateTorrentParams& params);
        ~TorrentHandle();

        bool isV

    };
}
