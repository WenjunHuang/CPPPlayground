set(PREFIX cpp17_stl)
set(CMAKE_CXX_STANDARD 20)

find_package(Boost REQUIRED)
find_package(fmt CONFIG REQUIRED)
add_single_file_targets(PREFIX ${PREFIX} DIR ${CMAKE_CURRENT_SOURCE_DIR} INCLUDES ${Boost_INCLUDE_DIRS} LIBS fmt::fmt-header-only)