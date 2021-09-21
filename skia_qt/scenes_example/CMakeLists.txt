set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_CXX_STANDARD 17)

#list(APPEND CMAKE_PREFIX_PATH ${QT_CMAKE})
#find_package(PkgConfig)
#find_package(QT NAMES Qt5  COMPONENTS Core Widgets REQUIRED)
find_package(Qt5 COMPONENTS Core Widgets REQUIRED)
find_package(range-v3 CONFIG REQUIRED)
find_package(ICU REQUIRED COMPONENTS uc dt in io)
find_package(fmt CONFIG REQUIRED)
find_package(harfbuzz CONFIG REQUIRED)
find_package(GTest CONFIG REQUIRED)

set(SRC_SET
        resource.qrc main.cc QSkiaOpenGlWindow.cc QSkiaOpenGlWindow.h Scene.cc Scene.h Scenes.cc Scenes.h Hud.cc Hud.h scenes/BitmapScene.cc scenes/BitmapScene.h glfw_window.cc glfw_window.h scenes/BitmapImageScene.cc scenes/BitmapImageScene.h scenes/blends_scene.cc scenes/blends_scene.h scenes/break_iterator_scene.cc scenes/break_iterator_scene.h shaper.cc shaper.h unicode.cc unicode.h break_iterator_icu.h script_iterator_icu.h)
set(DEP_LIBS
        skia skia::skia range-v3 range-v3-meta range-v3::meta range-v3-concepts ICU::uc commons fmt::fmt harfbuzz
       ICU::dt ICU::in ICU::io)
add_executable(skia_scenes_example ${SRC_SET})
target_link_libraries(skia_scenes_example PRIVATE Qt${QT_VERSION_MAJOR}::Core Qt${QT_VERSION_MAJOR}::CorePrivate Qt${QT_VERSION_MAJOR}::Widgets glfw glad::glad skia skia::skia range-v3 range-v3-meta range-v3::meta range-v3-concepts ICU::uc commons fmt::fmt harfbuzz)

function(create_test)
    include(CMakeParseArguments)
    set(singleValues NAME)
    set(multiValues SRCS LIBS)
    cmake_parse_arguments(ARG
            ""
            "${singleValues}"
            "${multiValues}"
            ${ARGN})
    add_executable(${ARG_NAME} ${ARG_SRCS})
    target_link_libraries(${ARG_NAME} PRIVATE
            "${ARG_LIBS}"
            GTest::gmock GTest::gtest GTest::gtest_main)
endfunction()

create_test(NAME unicode_test SRCS unicode_test.cc unicode.cc LIBS ${DEP_LIBS})
#add_executable(unicode_test unicode_test.cc unicode.cc)
#target_link_libraries(unicode_test PRIVATE GTest::gmock GTest::gtest GTest::gmock_main GTest::gtest_main)