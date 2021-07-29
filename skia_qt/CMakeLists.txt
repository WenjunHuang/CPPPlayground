set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_CXX_STANDARD 17)

list(APPEND CMAKE_PREFIX_PATH ${QT_CMAKE})
find_package(PkgConfig)
find_package(QT NAMES Qt5 Qt6 COMPONENTS Core Widgets Network Quick Concurrent QuickControls2 REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Core Widgets Network Quick Concurrent QuickControls2 REQUIRED)
find_package(skia CONFIG REQUIRED)

add_executable(skia_qt resource.qrc main.cc QSkiaOpenGlWindow.cc QSkiaOpenGlWindow.h Scene.cc Scene.h)
target_link_libraries(skia_qt PRIVATE Qt${QT_VERSION_MAJOR}::Core Qt${QT_VERSION_MAJOR}::CorePrivate Qt${QT_VERSION_MAJOR}::Widgets Qt${QT_VERSION_MAJOR}::Network Qt${QT_VERSION_MAJOR}::Quick Qt${QT_VERSION_MAJOR}::Concurrent Qt${QT_VERSION_MAJOR}::QuickControls2 skia skia::skia)
