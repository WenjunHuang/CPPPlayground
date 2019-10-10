include(FindPkgConfig)
if (Linux)
pkg_search_module(torrentrasterbar REQUIRED torrentrasterbar)
endif()

if (Darwin)
pkg_search_module(torrentrasterbar REQUIRED libtorrent-rasterbar)
endif()

add_executable(base_test main.cc test_bittorrent.cc ../base/bittorrent/info_hash.cc ../base/bittorrent/info_hash.h)
target_include_directories(base_test PUBLIC ${Boost_INCLUDE_DIRS} ".")
target_link_libraries(base_test PUBLIC
        Qt5::Core
        Qt5::Network
        Qt5::Xml
        torrentrasterbar)
