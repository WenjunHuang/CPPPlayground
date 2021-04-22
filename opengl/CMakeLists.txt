list(APPEND CMAKE_PREFIX_PATH ${QT_CMAKE})

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

find_package(Qt5 REQUIRED Core Widgets Network Quick QuickControls2 Gui)
find_package(glfw3 CONFIG REQUIRED)
find_package(glad CONFIG REQUIRED)
find_package(range-v3 CONFIG REQUIRED)
find_package(assimp CONFIG REQUIRED)

set(PREFIX opengl)
add_single_file_targets(PREFIX ${PREFIX} DIR ${CMAKE_CURRENT_SOURCE_DIR} LIBS glfw glad::glad Qt5::Core Qt5::Widgets Qt5::GuiPrivate Qt5::Gui Qt5::Quick range-v3 range-v3-meta range-v3::meta range-v3-concepts  assimp::assimp)
add_dir_target(PREFIX ${PREFIX} DIR ${CMAKE_CURRENT_SOURCE_DIR} LIBS glfw glad::glad Qt5::Core Qt5::Widgets Qt5::Gui Qt5::Quick Qt5::GuiPrivate range-v3 range-v3-meta range-v3::meta range-v3-concepts  assimp::assimp)
