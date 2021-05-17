set(PREFIX mp)
set(CMAKE_CXX_STANDARD 17)
find_package(doctest CONFIG REQUIRED)
add_single_file_targets(PREFIX ${PREFIX} DIR ${CMAKE_CURRENT_SOURCE_DIR} LIBS doctest::doctest)
add_dir_target(PREFIX ${PREFIX} DIR ${CMAKE_CURRENT_SOURCE_DIR} LIBS doctest::doctest)


