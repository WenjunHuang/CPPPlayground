cmake_minimum_required(VERSION 3.7)
project(qt5)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY bin)
set(CMAKE_PREFIX_PATH $ENV{QT_CMAKE})
find_package(PkgConfig)
find_package(Qt5 REQUIRED Core Concurrent)

add_executable(skynet skynet.cc)
qt5_use_modules(skynet LINK_PRIVATE Core Concurrent)


