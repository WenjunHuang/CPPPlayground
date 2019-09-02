add_library(qbt_base STATIC net/DownloadManager.cc net/DownloadManager.h Preferences.cc Preferences.h net/ProxyConfigurationManager.cc net/ProxyConfigurationManager.h SettingsStorage.cc SettingsStorage.h Logger.cc Logger.h utils/Fs.cc utils/Fs.h Profile.cc Profile.h private/profile_p.cc private/profile_p.h global.h utils/net.cc utils/net.h algorithm.h)
target_include_directories(qbt_base PUBLIC ${Boost_INCLUDE_DIRS} ".")
target_link_libraries(qbt_base PUBLIC
        Qt5::Core
        Qt5::Network Qt5::Xml)
