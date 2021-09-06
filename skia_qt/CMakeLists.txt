list(APPEND CMAKE_PREFIX_PATH ${QT_CMAKE})

set(PREFIX skia)
set(CMAKE_CXX_STANDARD 17)
if (MSVC)
    add_compile_options("/Zc:__cplusplus" "/utf-8")
#"/fsanitize=address")
endif ()

find_package(PkgConfig)
find_package(QT NAMES Qt5 Qt6 COMPONENTS Core Widgets Network Quick Concurrent QuickControls2 REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Core Widgets Network Quick Concurrent QuickControls2 REQUIRED)
find_package(skia CONFIG REQUIRED)
find_package(range-v3 CONFIG REQUIRED)
add_single_file_targets(PREFIX ${PREFIX} DIR ${CMAKE_CURRENT_SOURCE_DIR} LIBS Qt${QT_VERSION_MAJOR}::Core Qt${QT_VERSION_MAJOR}::CorePrivate Qt${QT_VERSION_MAJOR}::Widgets Qt${QT_VERSION_MAJOR}::Network Qt${QT_VERSION_MAJOR}::Quick Qt${QT_VERSION_MAJOR}::Concurrent Qt${QT_VERSION_MAJOR}::QuickControls2 skia skia::skia  range-v3 range-v3-meta range-v3::meta range-v3-concepts)

add_subdirectory(scenes_example)

