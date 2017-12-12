cmake_minimum_required(VERSION 3.7)
project(CPPPlayground)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_BUILD_TYPE Debug)

enable_testing()
include_directories(include)

add_subdirectory(boost)
add_subdirectory(algorithm)
add_subdirectory(cookbook)
add_subdirectory(actor)
add_subdirectory(cpp_template)
add_subdirectory(coreutils_cpp)
add_subdirectory(qt5)
add_subdirectory(lldb)
add_subdirectory(rust_with_cpp)
