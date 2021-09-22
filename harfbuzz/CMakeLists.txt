
set(PREFIX harfbuzz)
set(CMAKE_CXX_STANDARD 17)
if (MSVC)
    add_compile_options("/Zc:__cplusplus" "/utf-8")
    #"/fsanitize=address")
endif ()

find_package(PkgConfig)
find_package(fmt CONFIG REQUIRED)
find_package(harfbuzz CONFIG REQUIRED)
find_package(glfw3 CONFIG REQUIRED)
find_package(glad CONFIG REQUIRED)
find_package(freetype CONFIG REQUIRED)
find_package(ICU REQUIRED COMPONENTS uc dt in io)

set(DEPS harfbuzz glfw glad::glad freetype ICU::dt ICU::in ICU::io)
add_single_file_targets(PREFIX ${PREFIX} DIR ${CMAKE_CURRENT_SOURCE_DIR} LIBS ${DEPS})
add_dir_target(PREFIX ${PREFIX} DIR ${CMAKE_CURRENT_SOURCE_DIR} LIBS ${DEPS})


