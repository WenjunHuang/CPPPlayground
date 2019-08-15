cmake_minimum_required(VERSION 3.7)
project(libsdl2_learn)
set(CMAKE_CXX_STANDARD 14)
file(GLOB_RECURSE SOURCES src/main/*.cpp src/main/*.h)
file(GLOB TEST_SOURCES src/test/*.cpp)

include_directories(src/main)

add_executable(${PROJECT_NAME} ${SOURCES} ${TEST_SOURCES})

