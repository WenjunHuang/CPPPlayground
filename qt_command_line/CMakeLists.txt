cmake_minimum_required(VERSION 3.7)
project(qt_command_line)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY bin)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_THREAD_PREFER_PTHREAD TRUE)

list(APPEND CMAKE_PREFIX_PATH $ENV{QT_CMAKE})
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

find_package(Qt5Core REQUIRED)
find_package(Qt5Network REQUIRED)

file(GLOB_RECURSE SOURCES src/main/*.cpp src/main/*.h)


add_executable(${PROJECT_NAME} ${SOURCES})

qt5_use_modules(${PROJECT_NAME} Core Network)

list(APPEND Libs Qt5::Core Qt5::Network)
target_link_libraries(${PROJECT_NAME} ${Libs})
