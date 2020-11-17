
find_package(rxcpp CONFIG REQUIRED)
set(PREFIX rxcpp)
add_single_file_targets(PREFIX ${PREFIX} DIR ${CMAKE_CURRENT_SOURCE_DIR} LIBS rxcpp)
add_dir_target(PREFIX ${PREFIX} DIR ${CMAKE_CURRENT_SOURCE_DIR} LIBS rxcpp)


