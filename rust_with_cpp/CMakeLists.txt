cmake_minimum_required(VERSION 3.7)
project(rust_with_cpp)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_THREAD_PREFER_PTHREAD TRUE)

list(APPEND CMAKE_PREFIX_PATH ${QT_CMAKE})
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

# 配置QT
set(QT_MIN_VERSION "5.10.0")
find_package(Qt5 ${QT_MIN_VERSION} CONFIG REQUIRED COMPONENTS Widgets)

string(TOUPPER "${CMAKE_BUILD_TYPE}" CMAKE_BUILD_TYPE_UPPER)
if(CMAKE_BUILD_TYPE_UPPER STREQUAL DEBUG)
    set(RUST_TARGET_DIR target/debug/)
    set(RUST_BUILD_FLAG)
else()
    set(RUST_TARGET_DIR target/release/)
    set(RUST_BUILD_FLAG --release)
endif()

### find dependencies ###

include(FeatureSummary)
find_package(Cargo REQUIRED)
find_package(Rust REQUIRED)

### build commands ###
SET(RUST_DIR "${CMAKE_CURRENT_SOURCE_DIR}/rust")
SET(RUST_LIB "${RUST_DIR}/${RUST_TARGET_DIR}/librust.a")

# compile the rust code into a static library
add_custom_command(
        OUTPUT "${RUST_LIB}"
        COMMAND ${Cargo_EXECUTABLE} build ${RUST_BUILD_FLAG}
        DEPENDS rust/src/main.rs
        WORKING_DIRECTORY "${RUST_DIR}")


list(APPEND Libs Qt5::Widgets ${CMAKE_DL_LIBS})
list(APPEND Libs "${RUST_LIB}")
list(APPEND Libs "resolv")
set(SRCS src/main/cpp_to_rust.cpp)

add_library(MyLib ${SRCS})
add_custom_target(MyRust DEPENDS "${RUST_LIB}")
add_custom_target(my_rust_target DEPENDS MyLib MyRust)

set_target_properties(MyLib PROPERTIES
        CXX_STANDARD 11
        CXX_STANDARD_REQUIRED ON)