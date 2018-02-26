cmake_minimum_required(VERSION 3.7)
project(llvm)
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake")

# llvm
find_package(LLVM 5.0 REQUIRED bitreader core support)
include_directories(${LLVM_INCLUDE_DIR})
link_directories(${LLVM_LIB_DIR})

# clang
find_package(CLANG)
include_directories(${CLANG_INCLUDE_DIR})
link_directories(${CLANG_LIB_DIR})

file(GLOB_RECURSE ALG_SOURCES src/main/*.cpp src/main/*.h)
include_directories(src/main)

message("${CLANG_LIBS}")
list(APPEND Libs ${LLVM_LIBS_CORE})
list(APPEND Libs ${CLANG_LIBS})
list(APPEND Libs "curses")
list(APPEND Libs "clang")

add_executable(${PROJECT_NAME} ${ALG_SOURCES} ${ALG_TEST_SOURCES})
target_link_libraries(${PROJECT_NAME} ${Libs})
set_target_properties(${PROJECT_NAME} PROPERTIES
        COMPILE_FLAGS "${LLVM_COMPILE_FLAGS}")

