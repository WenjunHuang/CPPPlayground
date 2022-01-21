set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)
find_package(PkgConfig)
find_package(Qt5 COMPONENTS Core Widgets Network Quick Concurrent QuickControls2 REQUIRED)

set(PREFIX qt)
add_single_file_targets(PREFIX ${PREFIX} DIR ${CMAKE_CURRENT_SOURCE_DIR} LIBS Qt5::Core Qt5::CorePrivate Qt5::Widgets Qt5::Network Qt5::Quick Qt5::Concurrent Qt5::QuickControls2
        INCLUDES ${Qt5Gui_PRIVATE_INCLUDE_DIRS})
add_dir_target(PREFIX ${PREFIX} DIR ${CMAKE_CURRENT_SOURCE_DIR} LIBS Qt5::Core Qt5::CorePrivate Qt5::Widgets Qt5::Network Qt5::Quick Qt5::Concurrent Qt5::QuickControls2)

add_subdirectory(EventSystem)