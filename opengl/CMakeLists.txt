cmake_minimum_required(VERSION 3.13)
project(OpenglPlayground)
set(CMAKE_CXX_STANDARD 17)

if(UNIX AND NOT APPLE)
    set(LINUX TRUE)
endif()

add_subdirectory(source/common)
add_subdirectory(source/opengl)
add_subdirectory(source/glfw)