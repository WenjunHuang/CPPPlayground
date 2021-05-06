set(CMAKE_CXX_STANDARD 17)
set(PREFIX function_object)

find_package(Qt5 COMPONENTS Core REQUIRED)

add_single_file_targets(PREFIX ${PREFIX} DIR ${CMAKE_CURRENT_SOURCE_DIR} LIBS Qt5::Core)
