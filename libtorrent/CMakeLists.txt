set(CMAKE_CXX_STANDARD 17)
find_package(LibtorrentRasterbar CONFIG REQUIRED)

set(PREFIX libtorrent)
add_single_file_targets(PREFIX ${PREFIX} DIR ${CMAKE_CURRENT_SOURCE_DIR} LIBS LibtorrentRasterbar::torrent-rasterbar)
add_dir_target(PREFIX ${PREFIX} DIR ${CMAKE_CURRENT_SOURCE_DIR} LIBS LibtorrentRasterbar::torrent-rasterbar)

