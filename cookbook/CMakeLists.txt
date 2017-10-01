cmake_minimum_required(VERSION 3.7)
project(cookbook)
set(CMAKE_CXX_STANDARD 14)
file(GLOB COOKBOOK_SOURCES src/main/*/*.cpp src/main/*/*.h)
file(GLOB COOKBOOK_TEST_SOURCES src/test/*.cpp)

add_executable(${PROJECT_NAME} ${COOKBOOK_SOURCES} ${COOKBOOK_TEST_SOURCES} )
