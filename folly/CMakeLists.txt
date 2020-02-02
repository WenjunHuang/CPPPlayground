set(CMAKE_CXX_STANDARD 17)

if (MSVC)
    find_package(folly CONFIG REQUIRED)
    find_package(glog CONFIG REQUIRED)
    find_package(ZLIB REQUIRED)
    set(FOLLY Folly::folly Folly::folly_deps)
    set(GLOG glog::glog)
    add_compile_options("/Zc:__cplusplus")
endif ()

if (APPLE)
    find_package(PkgConfig)
    pkg_check_modules(folly REQUIRED libfolly IMPORTED_TARGET)
    pkg_check_modules(glog REQUIRED libglog IMPORTED_TARGET)
    set(FOLLY PkgConfig::folly)
    set(GLOG PkgConfig::glog)
endif ()

add_subdirectory(futures)


