cmake_minimum_required(VERSION 3.7)
project(qt5)
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)
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
add_custom_target(rust_target DEPENDS "${RUST_LIB}")

find_package(Threads REQUIRED)
find_package(Boost REQUIRED)
find_package(Qt5Widgets REQUIRED)
find_package(Qt5Core REQUIRED)
find_package(Qt5Gui REQUIRED)
find_package(Qt5Quick REQUIRED)
find_package(Qt5PrintSupport REQUIRED)
find_package(Qt5OpenGL REQUIRED)

file(GLOB_RECURSE STYLES src/main/*/qss/*.scss)
set(STYLES_OUTPUT ${PROJECT_SOURCE_DIR}/src/main/360protect/qss/styles.qss)
add_custom_target(COMPILE_SCSS ALL)
add_custom_command(TARGET COMPILE_SCSS COMMAND scss ${STYLES} > ${STYLES_OUTPUT})

file(GLOB_RECURSE ALG_SOURCES src/main/*/*.cpp src/main/*/*.h
        src/main/*/*.ui
        src/main/*/*.ini
        src/main/*/images/*.png
        src/main/*/*.qrc
        src/main/*/*.db)
file(GLOB ALG_TEST_SOURCES src/test/*/*.cpp)

file(GLOB SUB_PROJ_DIRS src/main/*)

include_directories(${Boost_INCLUDE_DIR})
include_directories(src/main/include)
include_directories(${SUB_PROJ_DIRS})
include_directories(${Qt5Widgets_PRIVATE_INCLUDE_DIRS})

add_executable(${PROJECT_NAME} ${ALG_SOURCES} ${ALG_TEST_SOURCES} src/main/main.cpp)
add_dependencies(${PROJECT_NAME} rust_target)

qt5_use_modules(${PROJECT_NAME} Core Widgets Gui Quick)

list(APPEND Libs "${RUST_LIB}")
list(APPEND Libs Qt5::Core Qt5::Widgets Qt5::WidgetsPrivate Qt5::Quick Qt5::PrintSupport Qt5::OpenGL ${CMAKE_DL_LIBS})
list(APPEND Libs "resolv")
target_link_libraries(${PROJECT_NAME} ${Libs})
