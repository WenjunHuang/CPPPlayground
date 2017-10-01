cmake_minimum_required(VERSION 3.7)
project(CPPPlayground)

set(CMAKE_CXX_STANDARD 14)

enable_testing()
include_directories(include)

add_subdirectory(boost)
add_subdirectory(algorithm)
add_subdirectory(cookbook)


