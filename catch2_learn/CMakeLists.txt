project(catch2_learn)
set(CMAKE_CXX_STANDARD 14)
find_package(Catch2 CONFIG REQUIRED)
add_single_file_targets(PREFIX ${PREFIX} DIR ${CMAKE_CURRENT_SOURCE_DIR} LIBS Catch2::Catch2)

