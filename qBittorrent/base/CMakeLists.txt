add_library(qbt_base STATIC net/DownloadManager.cc net/DownloadManager.h Preferences.cc Preferences.h net/ProxyConfigurationManager.cc net/ProxyConfigurationManager.h SettingsStorage.cc SettingsStorage.h Logger.cc Logger.h utils/Fs.cc utils/Fs.h Profile.cc Profile.h private/profile_p.cc private/profile_p.h global.h utils/net.cc utils/net.h algorithm.h async_file_storage.cc async_file_storage.h bittorrent/addtorrentparams.h tri_state_bool.cc tri_state_bool.h bittorrent/cachestatus.h bittorrent/download_priority.cc bittorrent/download_priority.h bittorrent/info_hash.cc bittorrent/info_hash.h bittorrent/magnet_uri.cc bittorrent/magnet_uri.h bittorrent/tracker_entry.cc bittorrent/tracker_entry.h)
target_include_directories(qbt_base PUBLIC ${Boost_INCLUDE_DIRS} ".")
target_link_libraries(qbt_base PUBLIC
        Qt5::Core
        Qt5::Network Qt5::Xml)
