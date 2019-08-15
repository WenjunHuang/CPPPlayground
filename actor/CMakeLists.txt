cmake_minimum_required(VERSION 3.7)
project(actor)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_MODULE_PATH  "${PROJECT_SOURCE_DIR}/cmake")

find_package(CAF COMPONENTS core REQUIRED)
include_directories(${CAF_INCLUDE_DIRS})

file(GLOB ACTOR_SOURCES src/main/*.cpp src/main/*.h)

add_executable(${PROJECT_NAME} ${ACTOR_SOURCES} )
target_link_libraries(${PROJECT_NAME} ${CAF_LIBRARIES})
