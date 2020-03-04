set(PREFIX cpp20)
set(CMAKE_CXX_STANDARD 20)

if (APPLE)
    add_compile_options("-fcoroutines-ts")
endif ()
add_single_file_targets(PREFIX ${PREFIX} DIR ${CMAKE_CURRENT_SOURCE_DIR})
add_dir_target(PREFIX ${PREFIX} DIR ${CMAKE_CURRENT_SOURCE_DIR})

