cmake_minimum_required(VERSION 3.7)
project(qBittorrent)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY bin)
set(CMAKE_PREFIX_PATH ${QT_CMAKE})
find_package(Boost REQUIRED COMPONENTS filesystem system thread coroutine)
find_package(Qt5 REQUIRED Core Widgets Network Xml)

add_subdirectory(base)
add_subdirectory(test)

add_executable(qBittorrent main.cc)
qt5_use_modules(qBittorrent LINK_PRIVATE Widgets Network)
target_link_libraries(qBittorrent PRIVATE qbt_base)

