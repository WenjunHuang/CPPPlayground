cmake_minimum_required(VERSION 3.7)
project(BoostPlayground)

set(CMAKE_CXX_STANDARD 14)

enable_testing()
find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})

set(BOOST_ROOT $ENV{BOOST_ROOT})
find_package(Boost REQUIRED
        COMPONENTS filesystem system thread coroutine)
include_directories(${Boost_INCLUDE_DIRS})
include_directories(include)

file(GLOB BOOST_SOURCES src/main/*.cpp src/main/*.h)

add_executable(${PROJECT_NAME} ${BOOST_SOURCES} ../actor/src/main/main.cpp)
target_link_libraries(${PROJECT_NAME} ${Boost_LIBRARIES} ${GTEST_BOTH_LIBRARIES})
add_test(NAME ${PROJECT_NAME}_test COMMAND ${PROJECT_NAME})