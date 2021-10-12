set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)
find_package(PkgConfig)
find_package(freetype CONFIG REQUIRED)
find_package(ICU REQUIRED COMPONENTS uc dt in io)
find_package(harfbuzz CONFIG REQUIRED)
find_package(GTest CONFIG REQUIRED)
find_package(fmt CONFIG REQUIRED)
find_package(Qt5 COMPONENTS Core Widgets REQUIRED)

set(PREFIX text)
add_custom_target(${PREFIX}_fonts
        COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_LIST_DIR}/fonts ${CMAKE_CURRENT_BINARY_DIR}/fonts
        )
add_single_file_targets(PREFIX ${PREFIX} DIR ${CMAKE_CURRENT_SOURCE_DIR} LIBS
        freetype Qt5::Core Qt5::CorePrivate Qt5::Widgets
        ICU::dt ICU::in ICU::io
        harfbuzz
        harfbuzz::harfbuzz
        GTest::gtest
        fmt::fmt
        DEPS ${PREFIX}_fonts assets
        INCLUDES ${Qt5Gui_PRIVATE_INCLUDE_DIRS})
add_subdirectory(icu)