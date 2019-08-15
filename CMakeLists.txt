cmake_minimum_required(VERSION 3.8)
project(CPPPlayground)

enable_testing()
include_directories(include)
set(CMAKE_CXX_STANDARD 17)
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake")

add_subdirectory(boost)
add_subdirectory(algorithm)
add_subdirectory(cookbook)
add_subdirectory(actor)
add_subdirectory(cpp_template)
add_subdirectory(coreutils_cpp)
add_subdirectory(qt5)
add_subdirectory(lldb)
add_subdirectory(rust_with_cpp)
add_subdirectory(libsdl2)
add_subdirectory(gtk)
add_subdirectory(qt_command_line)
add_subdirectory(kqueue)
add_subdirectory(llvm)

# The gtest/gtest_main targets carry header search path
# dependencies automatically when using CMake 2.8.11 or
# later. Otherwise we have to add them here ourselves.
if (CMAKE_VERSION VERSION_LESS 2.8.11)
    include_directories("${gtest_SOURCE_DIR}/include")
endif()

include_directories(~/github/boost)
set(SOURCE_FILES main.cpp string_algrithm.h boost_xpressive.cpp)
add_executable(${PROJECT_NAME} ${SOURCE_FILES})
target_link_libraries(${PROJECT_NAME} gtest_main)
add_test(NAME ${PROJECT_NAME}_test COMMAND ${PROJECT_NAME})