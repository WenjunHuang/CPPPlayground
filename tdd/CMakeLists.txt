set(CMAKE_CXX_STANDARD 17)
set(PREFIX tdd)

find_package(fmt CONFIG REQUIRED)
find_package(GTest CONFIG REQUIRED)
find_package(Qt5 COMPONENTS Core REQUIRED)
add_single_file_targets(PREFIX ${PREFIX} DIR ${CMAKE_CURRENT_SOURCE_DIR} LIBS fmt::fmt GTest::gtest GTest::gmock Qt5::Core)
