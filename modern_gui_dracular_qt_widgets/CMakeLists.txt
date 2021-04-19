cmake_minimum_required(VERSION 3.14)
project(modern_gui)
set(CMAKE_CXX_STANDARD 17)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)
list(APPEND CMAKE_PREFIX_PATH ${QT_CMAKE})
find_package(PkgConfig)
find_package(QT NAMES Qt5 Qt6 COMPONENTS Core Widgets Network Quick Concurrent QuickControls2 REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Core Widgets Network Quick Concurrent QuickControls2 REQUIRED)

add_executable(modern_gui_dracular_qt_widgets main.cc resources.qrc mainwindow.ui main_window.cc main_window.h extraleftmenu.cpp extraleftmenu.ui title_right_info.cc title_right_info.h)
target_link_libraries(modern_gui_dracular_qt_widgets PRIVATE Qt${QT_VERSION_MAJOR}::Core Qt${QT_VERSION_MAJOR}::Widgets Qt${QT_VERSION_MAJOR}::WidgetsPrivate Qt${QT_VERSION_MAJOR}::Network Qt${QT_VERSION_MAJOR}::Quick Qt${QT_VERSION_MAJOR}::Concurrent Qt${QT_VERSION_MAJOR}::QuickControls2)
