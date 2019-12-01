cmake_minimum_required(VERSION 3.13)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY bin)
list(APPEND CMAKE_PREFIX_PATH ${QT_CMAKE})
find_package(Qt5 REQUIRED Core Widgets Network Quick QuickControls2 Concurrent)

project(popup LANGUAGES CXX)

find_package(Qt5 COMPONENTS Core Quick Network Concurrent REQUIRED)

add_executable(${PROJECT_NAME} "main.cc" "resource.qrc" )
target_link_libraries(${PROJECT_NAME} PRIVATE Qt5::Core Qt5::Quick Qt5::Network Qt5::Concurrent Qt5::QuickControls2)

