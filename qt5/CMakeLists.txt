<<<<<<< HEAD
project(QtWidgetsPlayground)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
=======
cmake_minimum_required(VERSION 3.7)
project(qt5)

set(CMAKE_CXX_STANDARD 14)
>>>>>>> 6b0663b4cf54727e84e00a957f3c1ab056b8174c
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY bin)
<<<<<<< HEAD
set(CMAKE_PREFIX_PATH $ENV{QT_CMAKE})
find_package(PkgConfig)
pkg_check_modules(crypto++ REQUIRED libcrypto++)
find_package(Qt5 REQUIRED Core Widgets Network)

set_target_properties(Qt5::Core PROPERTIES MAP_IMPORTED_CONFIG_COVERAGE "DEBUG")
set_target_properties(Qt5::Widgets PROPERTIES MAP_IMPORTED_CONFIG_COVERAGE "DEBUG")

add_executable(SizePolicy SizePolicy/main.cc)
qt5_use_modules(SizePolicy LINK_PRIVATE Widgets)

add_executable(Pimp Pimp/main.cc Pimp/main.cc Pimp/MyFoo.cc Pimp/MyFoo.h Pimp/MyFoo_p.h Pimp/MyFoo_p.h)
qt5_use_modules(Pimp LINK_PRIVATE Widgets)

qt5_add_resources(AbsolutePositioningSource resources/resource.qrc)
add_executable(AbsolutePositioning AbsolutePositioning/main.cc ${AbsolutePositioningSource})
qt5_use_modules(AbsolutePositioning LINK_PRIVATE Widgets)

add_executable(DraggableButton DraggableButton/main.cc DraggableButton/DraggableButton.cc)
qt5_use_modules(DraggableButton LINK_PRIVATE Widgets)

add_executable(HttpClient HttpClient/main.cc HttpClient/HttpClientWindow.cc HttpClient/HttpClientWindow.h)
qt5_use_modules(HttpClient LINK_PRIVATE Core Widgets Network)

add_executable(Crypto Crypto/main.cc)
target_link_libraries(Crypto PRIVATE crypto++ Qt5::Core )
=======
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
>>>>>>> 6b0663b4cf54727e84e00a957f3c1ab056b8174c
