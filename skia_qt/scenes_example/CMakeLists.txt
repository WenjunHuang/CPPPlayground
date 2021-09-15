set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_CXX_STANDARD 20)

#list(APPEND CMAKE_PREFIX_PATH ${QT_CMAKE})
#find_package(PkgConfig)
find_package(QT NAMES Qt5 Qt6 COMPONENTS Core Widgets REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Core Widgets REQUIRED)
find_package(range-v3 CONFIG REQUIRED)
find_package(ICU REQUIRED COMPONENTS uc dt in io)
find_package(fmt CONFIG REQUIRED)
find_package(harfbuzz CONFIG REQUIRED)


set(SRC_SET
        resource.qrc main.cc QSkiaOpenGlWindow.cc QSkiaOpenGlWindow.h Scene.cc Scene.h Scenes.cc Scenes.h Hud.cc Hud.h scenes/BitmapScene.cc scenes/BitmapScene.h glfw_window.cc glfw_window.h scenes/BitmapImageScene.cc scenes/BitmapImageScene.h scenes/blends_scene.cc scenes/blends_scene.h scenes/break_iterator_scene.cc scenes/break_iterator_scene.h shaper.cc shaper.h)
add_executable(skia_scenes_example ${SRC_SET})
target_link_libraries(skia_scenes_example PRIVATE Qt${QT_VERSION_MAJOR}::Core Qt${QT_VERSION_MAJOR}::CorePrivate Qt${QT_VERSION_MAJOR}::Widgets glfw glad::glad skia skia::skia range-v3 range-v3-meta range-v3::meta range-v3-concepts ICU::uc commons fmt::fmt harfbuzz)
