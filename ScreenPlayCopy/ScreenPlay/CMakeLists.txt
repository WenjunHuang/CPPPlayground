project(ScreenPlay LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)


find_package(OpenSSL REQUIRED)
find_package(benchmark CONFIG REQUIRED)
find_package(doctest CONFIG REQUIRED)
find_package(OpenSSL REQUIRED)
find_package(benchmark CONFIG REQUIRED)
find_package(doctest CONFIG REQUIRED)
find_package(
        Qt5
        REQUIRED
        Core
        Quick
        QuickControls2
        Gui
        Widgets
        WebSockets
        Svg
        LinguistTools
        WebEngine
        QuickCompiler
        Test)


set(src
        app.cc
        )

set(headers
        app.h
        )

if (CMAKE_BUILD_TYPE STREQUAL "Debug" OR CMAKE_BUILD_TYPE STREQUAL "RelWithDebInfo")
    qt_add_resources(resources ScreenPlayQML.qrc)
else ()
    qtquick_compiler_add_resources(resources ScreenPlayQML.qrc)
endif ()

add_library(ScreenPlayLib ${src} ${headers} ${resources} ${resources} ${fonts})
target_link_libraries(ScreenPlayLib
        PUBLIC
        Qt5::Quick
        Qt5::QuickControls2
        Qt5::Gui
        Qt5::Widgets
        Qt5::Core
        Qt5::WebSockets
        Qt5::Svg
        Qt5::WebEngine)

add_executable(${PROJECT_NAME} main.cc)
target_link_libraries(${PROJECT_NAME} PUBLIC ScreenPlayLib)