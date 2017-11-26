cmake_minimum_required(VERSION 3.7)
project(cpp_template)
set(CMAKE_CXX_STANDARD 17)
file(GLOB_RECURSE ALG_SOURCES src/main/*/*.cpp src/main/*/*.h)
file(GLOB ALG_TEST_SOURCES src/test/basic/*.cpp)

include_directories(src/main)

add_executable(${PROJECT_NAME} ${ALG_SOURCES} ${ALG_TEST_SOURCES} src/main/main.cpp)

