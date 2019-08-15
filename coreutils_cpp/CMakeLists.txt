cmake_minimum_required(VERSION 3.7)
project(coreutils_cpp)
set(CMAKE_CXX_STANDARD 14)
file(GLOB CUC_SOURCES src/main/*.cpp src/main/*.h)
file(GLOB CUC_TEST_SOURCES src/test/*.cpp)

include_directories(src/main)

add_executable(${PROJECT_NAME} ${CUC_SOURCES} ${CUC_TEST_SOURCES} src/main.cpp)

