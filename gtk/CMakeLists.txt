cmake_minimum_required(VERSION 3.7)
project(gtk)
set(CMAKE_CXX_STANDARD 14)
find_package(PkgConfig REQUIRED)
pkg_check_modules(GTK3 REQUIRED gtk+-3.0)
link_directories(${GTK3_LIBRARY_DIRS})

pkg_check_modules(GTKMM3 REQUIRED gtkmm-3.0)


file(GLOB SOURCES src/main/*.cpp src/main/*.h src/main/*.png)

include_directories(src/main)
include_directories(${GTKMM3_INCLUDE_DIRS})

add_executable(${PROJECT_NAME} ${SOURCES})

list(APPEND Libs ${GTKMM3_LIBRARIES})
target_link_libraries(${PROJECT_NAME} ${Libs})
