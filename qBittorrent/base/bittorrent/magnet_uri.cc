//
// Created by rick on 2019/9/11.
//

#include "magnet_uri.h"
lt::add_torrent_params BitTorrent::MagnetUri::addTorrentParams() const {
    return m_addTorrentParams;
}
BitTorrent::MagnetUri::MagnetUri(const QString& source)
    : m_valid(false), m_url(source) {
    if (source.isEmpty())
        return;

    if (isBitTorrentInfoHash(source))
        m_url = QLatin1String("magnet:?xt=urn:btih:") + source;

    lt::error_code ec;
    lt::parse_magnet_uri(m_url.toStdString(), m_addTorrentParams, ec);
    if (ec)
        return;

    m_valid = true;
    m_hash  = m_addTorrentParams.info_hash;

    m_valid = true;
    m_hash  = m_addTorrentParams.info_hash;
    m_name  = QString::fromStdString(m_addTorrentParams.name);

    m_trackers.reserve(m_addTorrentParams.trackers.size());
    for (auto& tracker : m_addTorrentParams.trackers)
        m_trackers.append(lt::announce_entry{tracker});

    m_urlSeeds.reserve(m_addTorrentParams.url_seeds.size());
    for (auto& urlSeed : m_addTorrentParams.url_seeds)
        m_urlSeeds.append(QUrl(QString::fromStdString(urlSeed)));
}
bool BitTorrent::isBitTorrentInfoHash(const QString& string) {
    const int SHA1_HEX_SIZE    = InfoHash::length() * 2;
    const int SHA1_BASE32_SIZE = InfoHash::length() * 1.6;

    return ((string.size() == SHA1_HEX_SIZE) &&
            !string.contains(QRegularExpression("[^0-9A-Fa-f]"))) ||
           ((string.size() == SHA1_BASE32_SIZE) &&
            !string.contains(
                QRegularExpression(QLatin1String("[^2-7A-Za-z]"))))
}
