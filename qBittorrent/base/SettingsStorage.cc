//
// Created by HUANG WEN JUN on 2019/8/30.
//

#include "SettingsStorage.h"
#include <QFile>
#include <QHash>
#include <memory>

namespace {
    class TransactionalSettings {
      public:
        explicit TransactionalSettings(const QString& name) : m_name(name) {}
        QVariantHash read();
        bool write(const QVariantHash& data);

      private:
        QString deserialize(const QString& name, QVariantHash& data);
        QString serialize(const QString& name, const QVariantHash& data);
        const QString m_name;
    };

    QString mapKey(const QString& key) {
        static const QHash<QString, QString> keyMapping = {
            {"BitTorrent/Session/MaxRatioAction",
             "Preferences/Bittorrent/MaxRatioAction"},
            {"BitTorrent/Session/DefaultSavePath",
             "Preferences/Downloads/SavePath"},
            {"BitTorrent/Session/TempPath", "Preferences/Downloads/TempPath"},
            {"BitTorrent/Session/TempPathEnabled",
             "Preferences/Downloads/TempPathEnabled"},
            {"BitTorrent/Session/AddTorrentPaused",
             "Preferences/Downloads/StartInPause"},
            {"BitTorrent/Session/RefreshInterval",
             "Preferences/General/RefreshInterval"},
            {"BitTorrent/Session/Preallocation",
             "Preferences/Downloads/PreAllocation"},
            {"BitTorrent/Session/AddExtensionToIncompleteFiles",
             "Preferences/Downloads/UseIncompleteExtension"},
            {"BitTorrent/Session/TorrentExportDirectory",
             "Preferences/Downloads/TorrentExportDir"},
            {"BitTorrent/Session/FinishedTorrentExportDirectory",
             "Preferences/Downloads/FinishedTorrentExportDir"},
            {"BitTorrent/Session/GlobalUPSpeedLimit",
             "Preferences/Connection/GlobalUPLimit"},
            {"BitTorrent/Session/GlobalDLSpeedLimit",
             "Preferences/Connection/GlobalDLLimit"},
            {"BitTorrent/Session/AlternativeGlobalUPSpeedLimit",
             "Preferences/Connection/GlobalUPLimitAlt"},
            {"BitTorrent/Session/AlternativeGlobalDLSpeedLimit",
             "Preferences/Connection/GlobalDLLimitAlt"},
            {"BitTorrent/Session/UseAlternativeGlobalSpeedLimit",
             "Preferences/Connection/alt_speeds_on"},
            {"BitTorrent/Session/BandwidthSchedulerEnabled",
             "Preferences/Scheduler/Enabled"},
            {"BitTorrent/Session/Port", "Preferences/Connection/PortRangeMin"},
            {"BitTorrent/Session/UseRandomPort",
             "Preferences/General/UseRandomPort"},
            {"BitTorrent/Session/IPv6Enabled",
             "Preferences/Connection/InterfaceListenIPv6"},
            {"BitTorrent/Session/Interface",
             "Preferences/Connection/Interface"},
            {"BitTorrent/Session/InterfaceName",
             "Preferences/Connection/InterfaceName"},
            {"BitTorrent/Session/InterfaceAddress",
             "Preferences/Connection/InterfaceAddress"},
            {"BitTorrent/Session/SaveResumeDataInterval",
             "Preferences/Downloads/SaveResumeDataInterval"},
            {"BitTorrent/Session/Encryption",
             "Preferences/Bittorrent/Encryption"},
            {"BitTorrent/Session/ForceProxy",
             "Preferences/Connection/ProxyForce"},
            {"BitTorrent/Session/ProxyPeerConnections",
             "Preferences/Connection/ProxyPeerConnections"},
            {"BitTorrent/Session/MaxConnections",
             "Preferences/Bittorrent/MaxConnecs"},
            {"BitTorrent/Session/MaxUploads",
             "Preferences/Bittorrent/MaxUploads"},
            {"BitTorrent/Session/MaxConnectionsPerTorrent",
             "Preferences/Bittorrent/MaxConnecsPerTorrent"},
            {"BitTorrent/Session/MaxUploadsPerTorrent",
             "Preferences/Bittorrent/MaxUploadsPerTorrent"},
            {"BitTorrent/Session/DHTEnabled", "Preferences/Bittorrent/DHT"},
            {"BitTorrent/Session/LSDEnabled", "Preferences/Bittorrent/LSD"},
            {"BitTorrent/Session/PeXEnabled", "Preferences/Bittorrent/PeX"},
            {"BitTorrent/Session/AddTrackersEnabled",
             "Preferences/Bittorrent/AddTrackers"},
            {"BitTorrent/Session/AdditionalTrackers",
             "Preferences/Bittorrent/TrackersList"},
            {"BitTorrent/Session/IPFilteringEnabled",
             "Preferences/IPFilter/Enabled"},
            {"BitTorrent/Session/TrackerFilteringEnabled",
             "Preferences/IPFilter/FilterTracker"},
            {"BitTorrent/Session/IPFilter", "Preferences/IPFilter/File"},
            {"BitTorrent/Session/GlobalMaxRatio",
             "Preferences/Bittorrent/MaxRatio"},
            {"BitTorrent/Session/AnnounceToAllTrackers",
             "Preferences/Advanced/AnnounceToAllTrackers"},
            {"BitTorrent/Session/DiskCacheSize",
             "Preferences/Downloads/DiskWriteCacheSize"},
            {"BitTorrent/Session/DiskCacheTTL",
             "Preferences/Downloads/DiskWriteCacheTTL"},
            {"BitTorrent/Session/UseOSCache", "Preferences/Advanced/osCache"},
            {"BitTorrent/Session/AnonymousModeEnabled",
             "Preferences/Advanced/AnonymousMode"},
            {"BitTorrent/Session/QueueingSystemEnabled",
             "Preferences/Queueing/QueueingEnabled"},
            {"BitTorrent/Session/MaxActiveDownloads",
             "Preferences/Queueing/MaxActiveDownloads"},
            {"BitTorrent/Session/MaxActiveUploads",
             "Preferences/Queueing/MaxActiveUploads"},
            {"BitTorrent/Session/MaxActiveTorrents",
             "Preferences/Queueing/MaxActiveTorrents"},
            {"BitTorrent/Session/IgnoreSlowTorrentsForQueueing",
             "Preferences/Queueing/IgnoreSlowTorrents"},
            {"BitTorrent/Session/OutgoingPortsMin",
             "Preferences/Advanced/OutgoingPortsMin"},
            {"BitTorrent/Session/OutgoingPortsMax",
             "Preferences/Advanced/OutgoingPortsMax"},
            {"BitTorrent/Session/IgnoreLimitsOnLAN",
             "Preferences/Advanced/IgnoreLimitsLAN"},
            {"BitTorrent/Session/IncludeOverheadInLimits",
             "Preferences/Advanced/IncludeOverhead"},
            {"BitTorrent/Session/AnnounceIP",
             "Preferences/Connection/InetAddress"},
            {"BitTorrent/Session/SuperSeedingEnabled",
             "Preferences/Advanced/SuperSeeding"},
            {"BitTorrent/Session/MaxHalfOpenConnections",
             "Preferences/Connection/MaxHalfOpenConnec"},
            {"BitTorrent/Session/uTPEnabled", "Preferences/Bittorrent/uTP"},
            {"BitTorrent/Session/uTPRateLimited",
             "Preferences/Bittorrent/uTP_rate_limited"},
            {"BitTorrent/TrackerEnabled",
             "Preferences/Advanced/trackerEnabled"},
            {"Network/Proxy/OnlyForTorrents",
             "Preferences/Connection/ProxyOnlyForTorrents"},
            {"Network/Proxy/Type", "Preferences/Connection/ProxyType"},
            {"Network/Proxy/Authentication",
             "Preferences/Connection/Proxy/Authentication"},
            {"Network/Proxy/Username", "Preferences/Connection/Proxy/Username"},
            {"Network/Proxy/Password", "Preferences/Connection/Proxy/Password"},
            {"Network/Proxy/IP", "Preferences/Connection/Proxy/IP"},
            {"Network/Proxy/Port", "Preferences/Connection/Proxy/Port"},
            {"Network/PortForwardingEnabled", "Preferences/Connection/UPnP"},
            {"AddNewTorrentDialog/TreeHeaderState",
             "AddNewTorrentDialog/qt5/treeHeaderState"},
            {"AddNewTorrentDialog/Width", "AddNewTorrentDialog/width"},
            {"AddNewTorrentDialog/Position", "AddNewTorrentDialog/y"},
            {"AddNewTorrentDialog/Expanded", "AddNewTorrentDialog/expanded"},
            {"AddNewTorrentDialog/SavePathHistory",
             "TorrentAdditionDlg/save_path_history"},
            {"AddNewTorrentDialog/Enabled",
             "Preferences/Downloads/NewAdditionDialog"},
            {"AddNewTorrentDialog/TopLevel",
             "Preferences/Downloads/NewAdditionDialogFront"},

            {"State/BannedIPs", "Preferences/IPFilter/BannedIPs"}};

        return keyMapping.value(key, key);
    }
} // namespace

SettingsStorage* SettingsStorage::m_instance = nullptr;

SettingsStorage::SettingsStorage()
    : m_data{TransactionalSettings(QLatin1String("qBittorrent")).read()},
      m_dirty(false), m_lock(QReadWriteLock::Recursive) {
    m_timer.setSingleShot(true);
    m_timer.setInterval(5 * 1000);
    connect(&m_timer, &QTimer::timeout, this, &SettingsStorage::save);
}

SettingsStorage::~SettingsStorage() {
    save();
}

void SettingsStorage::initInstance() {
    if (!m_instance)
        m_instance = new SettingsStorage;
}

void SettingsStorage::freeInstance() {
    delete m_instance;
    m_instance = nullptr;
}

SettingsStorage* SettingsStorage::instance() {
    return m_instance;
}

bool SettingsStorage::save() {
    if (!m_dirty) return false;
    QWriteLocker locker(&m_lock);
    if (!m_dirty) return false;

    TransactionalSettings settings(QLatin1String("qBittorrent"));
    if (settings.write(m_data)) {
        m_dirty = false;
        return true;
    }

    m_timer.start();
    return false;
}

QVariant SettingsStorage::loadValue(const QString& key,const QVariant& defaultValue) {
    QReadLocker locker(&m_lock);
    return m_data.value(mapKey(key),defaultValue);
}

void SettingsStorage::storeValue(const QString& key, const QVariant& value) {
    const QString realKey = mapKey(key);
    QWriteLocker locker(&m_lock);
    if (m_data.value(realKey) != value) {
        m_dirty = true;
        m_data.insert(realKey,value);
        m_timer.start();
    }
}

void SettingsStorage::removeValue(const QString& key) {
    auto realKey = mapKey(key);
    QWriteLocker locker(&m_lock);
    if (m_data.contains(realKey)) {
        m_dirty = true;
        m_data.remove(realKey);
        m_timer.start();
    }
}

QVariantHash TransactionalSettings::read() {
    QVariantHash res;

    const QString newPath = deserialize(m_name+QLatin1String("_new"),res);
    if (!newPath.isEmpty()) {
        Logger::instance()->addMessage(QObject::tr("Detected unclean program exit. Using fallback file to restore settings: %1")
        .arg(Utils::Fs::toNativePath(newPath)),Log::WARNING);
    }
}