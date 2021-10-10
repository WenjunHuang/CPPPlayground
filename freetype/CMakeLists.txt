set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)
list(APPEND CMAKE_PREFIX_PATH ${QT_CMAKE})
find_package(PkgConfig)
find_package(freetype CONFIG REQUIRED)
find_package(ICU REQUIRED COMPONENTS uc dt in io)

find_package(Qt5 COMPONENTS Core Widgets Network Quick Concurrent QuickControls2 REQUIRED)

set(PREFIX freetype)
add_single_file_targets(PREFIX ${PREFIX} DIR ${CMAKE_CURRENT_SOURCE_DIR} LIBS
        freetype Qt5::Core Qt5::CorePrivate Qt5::Widgets Qt5::Network Qt5::Quick Qt5::Concurrent Qt5::QuickControls2
        ICU::dt ICU::in ICU::io
        DEPS assets
        INCLUDES ${Qt5Gui_PRIVATE_INCLUDE_DIRS})
add_dir_target(PREFIX ${PREFIX} DIR ${CMAKE_CURRENT_SOURCE_DIR} LIBS
        freetype Qt5::Core Qt5::CorePrivate Qt5::Widgets Qt5::Network Qt5::Quick Qt5::Concurrent Qt5::QuickControls2
        DEPS assets)
