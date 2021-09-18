#list(APPEND CMAKE_PREFIX_PATH ${QT_CMAKE})

set(PREFIX skia)
set(CMAKE_CXX_STANDARD 17)
if (MSVC)
    add_compile_options("/Zc:__cplusplus" "/utf-8")
#"/fsanitize=address")
endif ()

find_package(PkgConfig)
find_package(skia CONFIG REQUIRED)
find_package(glfw3 CONFIG REQUIRED)
find_package(glad CONFIG REQUIRED)
find_package(fmt CONFIG REQUIRED)

add_library(commons INTERFACE window.h )
add_custom_target(assets
        COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_LIST_DIR}/scenes_example/assets ${CMAKE_CURRENT_BINARY_DIR}/assets
        )

add_single_file_targets(PREFIX ${PREFIX} DIR ${CMAKE_CURRENT_SOURCE_DIR} LIBS glfw skia skia::skia glad::glad fmt::fmt-header-only commons DEPS assets)
add_subdirectory(scenes_example)
