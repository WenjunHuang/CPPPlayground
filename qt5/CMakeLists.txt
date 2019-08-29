cmake_minimum_required(VERSION 3.7)
project(qt5)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY bin)
set(CMAKE_PREFIX_PATH $ENV{QT_CMAKE})
find_package(PkgConfig)
#pkg_check_modules(crypto++ REQUIRED libcrypto++)
find_package(Qt5 REQUIRED Core Widgets Network)

set_target_properties(Qt5::Core PROPERTIES MAP_IMPORTED_CONFIG_COVERAGE "DEBUG")
set_target_properties(Qt5::Widgets PROPERTIES MAP_IMPORTED_CONFIG_COVERAGE "DEBUG")

add_executable(SizePolicy SizePolicy/main.cc)
qt5_use_modules(SizePolicy LINK_PRIVATE Widgets)

add_executable(Pimp Pimp/main.cc Pimp/main.cc Pimp/MyFoo.cc Pimp/MyFoo.h Pimp/MyFoo_p.h Pimp/MyFoo_p.h)
qt5_use_modules(Pimp LINK_PRIVATE Widgets)

qt5_add_resources(AbsolutePositioningSource resources/resource.qrc)
add_executable(AbsolutePositioning AbsolutePositioning/main.cc ${AbsolutePositioningSource})
qt5_use_modules(AbsolutePositioning LINK_PRIVATE Widgets)

add_executable(DraggableButton DraggableButton/main.cc DraggableButton/DraggableButton.cc)
qt5_use_modules(DraggableButton LINK_PRIVATE Widgets)

add_executable(HttpClient HttpClient/main.cc HttpClient/HttpClientWindow.cc HttpClient/HttpClientWindow.h)
qt5_use_modules(HttpClient LINK_PRIVATE Core Widgets Network)

add_executable(Crypto Crypto/main.cc)
target_link_libraries(Crypto PRIVATE crypto++ Qt5::Core )
