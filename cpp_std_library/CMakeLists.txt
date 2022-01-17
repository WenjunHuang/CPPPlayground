set(CMAKE_CXX_STANDARD 17)
set(PREFIX cpp_std)

find_package(fmt CONFIG REQUIRED)
find_package(GTest CONFIG REQUIRED)
add_single_file_targets(PREFIX ${PREFIX} DIR ${CMAKE_CURRENT_SOURCE_DIR} LIBS fmt::fmt GTest::gtest GTest::gmock)

add_subdirectory("iterator-pair algorithms")