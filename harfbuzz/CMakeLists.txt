
set(PREFIX harfbuzz)
set(CMAKE_CXX_STANDARD 17)
if (MSVC)
    add_compile_options("/Zc:__cplusplus" "/utf-8")
#"/fsanitize=address")
endif ()

find_package(PkgConfig)
find_package(fmt CONFIG REQUIRED)
find_package(harfbuzz CONFIG REQUIRED)

add_single_file_targets(PREFIX ${PREFIX} DIR ${CMAKE_CURRENT_SOURCE_DIR} LIBS harfbuzz)


