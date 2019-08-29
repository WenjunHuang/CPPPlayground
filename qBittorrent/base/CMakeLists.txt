add_library(qbt_base STATIC net/DownloadManager.cc net/DownloadManager.h Preferences.cc Preferences.h)
target_link_libraries(qbt_base PUBLIC Qt5::Core Qt5::Network Qt5::Xml)
