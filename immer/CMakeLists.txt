set(PREFIX immer)
set(CMAKE_CXX_STANDARD 14)

find_package(fmt CONFIG REQUIRED)
find_package(Immer CONFIG REQUIRED)
add_single_file_targets(PREFIX ${PREFIX} DIR ${CMAKE_CURRENT_SOURCE_DIR} LIBS fmt::fmt immer)