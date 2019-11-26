cmake_minimum_required(VERSION 3.13)

project(folly)
set(LANGUAGE CXX)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(PkgConfig)
pkg_check_modules(folly REQUIRED libfolly IMPORTED_TARGET)
pkg_check_modules(glog REQUIRED libglog IMPORTED_TARGET)
#find_package(folly REQUIRED)
add_subdirectory(futures)

