cmake_minimum_required(VERSION 3.8)
project(CPPPlayground)

set(CMAKE_CXX_STANDARD 17)
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake")

add_subdirectory(qt5)
add_subdirectory(cpp_language)
add_subdirectory(boost)
add_subdirectory(opengl)
add_subdirectory(clang)
