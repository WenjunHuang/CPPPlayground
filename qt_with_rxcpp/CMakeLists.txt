set(PREFIX qt_with_rxcpp)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)
list(APPEND CMAKE_PREFIX_PATH ${QT_CMAKE})
find_package(PkgConfig)
#pkg_check_modules(cryptopp REQUIRED libcryptopp)
find_package(Qt5 REQUIRED Core Widgets Network Quick Concurrent QuickControls2)
find_package(rxcpp CONFIG REQUIRED)


add_single_file_targets(PREFIX ${PREFIX} DIR ${CMAKE_CURRENT_SOURCE_DIR} LIBS Qt5::Core Qt5::Widgets Qt5::Network Qt5::Quick Qt5::Concurrent Qt5::QuickControls2 rxcpp)
add_dir_target(PREFIX ${PREFIX} DIR ${CMAKE_CURRENT_SOURCE_DIR} LIBS Qt5::Core Qt5::Widgets Qt5::Network Qt5::Quick Qt5::Concurrent Qt5::QuickControls2 rxcpp)

