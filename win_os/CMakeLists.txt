set(PREFIX win_os)
if (MSVC)
    add_compile_options("/Zc:__cplusplus" "/utf-8")
    #"/fsanitize=address")
endif ()
find_package(fmt CONFIG REQUIRED)

add_single_file_targets(PREFIX ${PREFIX} DIR ${CMAKE_CURRENT_SOURCE_DIR} LIBS fmt::fmt)
