find_package(Boost REQUIRED)

set(CMAKE_CXX_STANDARD 20)

set(PREFIX boost)
add_single_file_targets(PREFIX ${PREFIX} DIR ${CMAKE_CURRENT_SOURCE_DIR} INCLUDES ${Boost_INCLUDE_DIRS})
