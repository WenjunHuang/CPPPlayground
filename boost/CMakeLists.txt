cmake_minimum_required(VERSION 3.7)
project(BoostPlayground)

#find_package(GTest REQUIRED)
#include_directories(${GTEST_INCLUDE_DIRS})

#set(BOOST_ROOT $ENV{BOOST_ROOT})
find_package(Boost REQUIRED
        COMPONENTS filesystem system thread coroutine fiber)

add_subdirectory(fiber)

