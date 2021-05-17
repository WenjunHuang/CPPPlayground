set(PREFIX cpp_clocks_timers)
find_package(fmt CONFIG REQUIRED)
add_single_file_targets(PREFIX ${PREFIX} DIR ${CMAKE_CURRENT_SOURCE_DIR} LIBS fmt::fmt)