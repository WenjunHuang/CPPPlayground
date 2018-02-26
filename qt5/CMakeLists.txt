cmake_minimum_required(VERSION 3.7)
project(qt5)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY bin)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_THREAD_PREFER_PTHREAD TRUE)

list(APPEND CMAKE_PREFIX_PATH $ENV{QT_CMAKE})
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

find_package(Threads REQUIRED)
find_package(Boost REQUIRED)
find_package(Qt5Widgets REQUIRED)
find_package(Qt5Core REQUIRED)
find_package(Qt5Gui REQUIRED)
find_package(Qt5Quick REQUIRED)
find_package(Qt5PrintSupport REQUIRED)
find_package(Qt5OpenGL REQUIRED)
find_package(Qt5Test REQUIRED)

file(GLOB_RECURSE STYLES src/main/*/qss/*.scss)
set(STYLES_OUTPUT ${PROJECT_SOURCE_DIR}/src/main/360protect/qss/styles.qss)
add_custom_target(COMPILE_SCSS ALL)
add_custom_command(TARGET COMPILE_SCSS COMMAND scss ${STYLES} > ${STYLES_OUTPUT})

file(GLOB_RECURSE ALG_SOURCES src/main/*/*.cpp src/main/*/*.h
        src/main/*/*.ui
        src/main/*/*.ini
        src/main/*/images/*.png
        src/main/*/*.qrc
        src/main/*/*.db)
file(GLOB ALG_TEST_SOURCES src/test/*/*.cpp)

file(GLOB SUB_PROJ_DIRS src/main/*)

include_directories(${Boost_INCLUDE_DIR})
include_directories(src/main/include)
include_directories(${SUB_PROJ_DIRS})
include_directories(${Qt5Widgets_PRIVATE_INCLUDE_DIRS})

add_executable(${PROJECT_NAME} ${ALG_SOURCES} ${ALG_TEST_SOURCES} src/main/main.cpp)

qt5_use_modules(${PROJECT_NAME} Core Widgets Gui Quick Test)

list(APPEND Libs Qt5::Core Qt5::Widgets Qt5::WidgetsPrivate Qt5::Quick Qt5::PrintSupport Qt5::OpenGL Qt5::Test ${CMAKE_DL_LIBS})
target_link_libraries(${PROJECT_NAME} ${Libs})
