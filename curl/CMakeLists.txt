list(APPEND CMAKE_PREFIX_PATH ${QT_CMAKE})

set(CMAKE_CXX_STANDARD 17)
if (MSVC)
    add_compile_options("/Zc:__cplusplus" "/utf-8")
#"/fsanitize=address")
endif ()
find_package(CURL CONFIG REQUIRED)
find_package(Qt5 COMPONENTS Core REQUIRED)

set(PREFIX curl)
add_single_file_targets(PREFIX ${PREFIX} DIR ${CMAKE_CURRENT_SOURCE_DIR} LIBS CURL::libcurl Qt5::Core)
add_dir_target(PREFIX ${PREFIX} DIR ${CMAKE_CURRENT_SOURCE_DIR} LIBS CURL::libcurl Qt5::Core)


