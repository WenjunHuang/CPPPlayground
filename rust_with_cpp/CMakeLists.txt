cmake_minimum_required(VERSION 3.7)
project(rust_with_cpp)
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY bin)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_THREAD_PREFER_PTHREAD TRUE)
list(APPEND CMAKE_PREFIX_PATH $ENV{QT_CMAKE})
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")
#set(CMAKE_BUILD_TYPE Debug)

string(TOUPPER "${CMAKE_BUILD_TYPE}" CMAKE_BUILD_TYPE_UPPER)
if(CMAKE_BUILD_TYPE_UPPER STREQUAL DEBUG)
    set(RUST_TARGET_DIR target/debug/)
    set(RUST_BUILD_FLAG)
else()
    set(RUST_TARGET_DIR target/release/)
    set(RUST_BUILD_FLAG --release)
endif()

# Find Cargo and Rust
include(FeatureSummary)
find_package(Cargo REQUIRED)
find_package(Rust REQUIRED)
SET(RUST_DIR "${CMAKE_CURRENT_SOURCE_DIR}/src/rust")
SET(RUST_LIB "${RUST_DIR}/${RUST_TARGET_DIR}/librust.a")
# compile the rust code into a static library
add_custom_command(
        OUTPUT "${RUST_LIB}"
        COMMAND ${Cargo_EXECUTABLE} build --color=never ${RUST_BUILD_FLAG}
        DEPENDS ${RUST_DIR}/src/lib.rs
        WORKING_DIRECTORY "${RUST_DIR}"
)
add_custom_target(cpp_rust_target DEPENDS "${RUST_LIB}")


file(GLOB_RECURSE ALG_SOURCES src/main/*.cpp src/main/*.h)

include_directories(src/main/include)
add_executable(${PROJECT_NAME} ${ALG_SOURCES})
add_dependencies(${PROJECT_NAME} cpp_rust_target)

list(APPEND Libs "${RUST_LIB}")
list(APPEND Libs "resolv")
target_link_libraries(${PROJECT_NAME} ${Libs})
