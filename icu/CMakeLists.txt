list(APPEND CMAKE_PREFIX_PATH ${QT_CMAKE})

set(PREFIX ICU)
set(CMAKE_CXX_STANDARD 17)
if (MSVC)
    add_compile_options("/Zc:__cplusplus" "/utf-8")
#"/fsanitize=address")
endif ()

find_package(PkgConfig)
find_package(fmt CONFIG REQUIRED)
find_package(ICU REQUIRED COMPONENTS uc dt in io)

add_single_file_targets(PREFIX ${PREFIX} DIR ${CMAKE_CURRENT_SOURCE_DIR} LIBS ICU::uc ICU::dt ICU::in ICU::io)


