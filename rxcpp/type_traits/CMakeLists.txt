list(APPEND CMAKE_PREFIX_PATH ${QT_CMAKE})

set(CMAKE_CXX_STANDARD 17)
if (MSVC)
    add_compile_options("/Zc:__cplusplus" "/utf-8")

endif ()
find_package(Qt5 COMPONENTS Core  REQUIRED)
find_package(rxcpp CONFIG REQUIRED)
set(PREFIX rxcpp_tt)
add_single_file_targets(PREFIX ${PREFIX} DIR ${CMAKE_CURRENT_SOURCE_DIR} LIBS rxcpp Qt5::Core )


