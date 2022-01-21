set(CMAKE_CXX_STANDARD 17)
set(PREFIX bm)
find_package(benchmark CONFIG REQUIRED)
add_single_file_targets(PREFIX ${PREFIX} DIR ${CMAKE_CURRENT_SOURCE_DIR} LIBS benchmark::benchmark)
