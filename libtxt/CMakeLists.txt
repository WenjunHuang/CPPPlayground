
set(PREFIX harfbuzz)
set(CMAKE_CXX_STANDARD 20)
if (MSVC)
    add_compile_options("/Zc:__cplusplus" "/utf-8")
    #"/fsanitize=address")
endif ()

find_package(PkgConfig)
find_package(fmt CONFIG REQUIRED)
find_package(freetype CONFIG REQUIRED)
find_package(harfbuzz CONFIG REQUIRED)
find_package(ICU REQUIRED COMPONENTS uc dt in io)
find_package(GTest CONFIG REQUIRED)
find_package(spdlog CONFIG REQUIRED)
find_package(skia CONFIG REQUIRED)


set(DEPS harfbuzz freetype ICU::dt ICU::in ICU::io fmt::fmt spdlog::spdlog spdlog::spdlog_header_only skia::skia)
add_library(libtxt STATIC src/minikin/SparseBitSet.cpp
        src/minikin/SparseBitSet.h src/minikin/font_family.cc src/minikin/font_family.h src/minikin/minikin_font.cc src/minikin/minikin_font.h
        src/minikin/Hyphenator.cpp
        src/minikin/Hyphenator.h src/minikin/minikin_internal.h src/minikin/hb_font_cache.cc src/minikin/hb_font_cache.h
        src/log/log.cc
        src/log/log.h src/minikin/minikin_internal.cc src/minikin/font_language_list_cache.cc src/minikin/font_language_list_cache.h src/minikin/font_language.cc src/minikin/font_language.h
        src/utils/JenkinsHash.cpp src/minikin/font_utils.cc src/minikin/font_utils.h src/minikin/cmap_coverage.cc src/minikin/cmap_coverage.h src/minikin/grapheme_break.cc src/minikin/grapheme_break.h src/minikin/font_collection.cc src/minikin/font_collection.h)
target_link_libraries(libtxt PUBLIC ${DEPS})
target_include_directories(libtxt PUBLIC "${CMAKE_CURRENT_LIST_DIR}/src"
        $<TARGET_PROPERTY:GTest::gtest,INCLUDE_DIRECTORIES>)
target_compile_definitions(libtxt PRIVATE $<$<CONFIG:DEBUG>:ENABLE_RACE_DETECTION>)

add_custom_target(libtxt_fonts
        COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_LIST_DIR}/fonts ${CMAKE_CURRENT_BINARY_DIR}/fonts
        )

add_executable(libtxt_test test/main.cc test/HbFontCacheTest.cc
        test/SparseBitSetTest.cpp
        test/minikin_font_for_test.cc test/minikin_font_for_test.h test/FontLanguageListCacheTest.cc test/CmapCoverageTest.cc test/FontCollectionTest.cc)
target_link_libraries(libtxt_test PRIVATE libtxt GTest::gtest)
add_dependencies(libtxt_test libtxt_fonts)


