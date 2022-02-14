set(PREFIX sanitize)
set(CMAKE_VERBOSE_MAKEFILE ON)
if(MSVC)
  add_compile_options("/utf-8" "/fsanitize=address")
endif()

add_single_file_targets(PREFIX ${PREFIX} DIR ${CMAKE_CURRENT_SOURCE_DIR})
