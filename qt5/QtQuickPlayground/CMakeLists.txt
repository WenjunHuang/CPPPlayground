cmake_minimum_required(VERSION 3.1)

project(QtQuickPlayground LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt5 COMPONENTS Core Quick REQUIRED)

add_executable(${PROJECT_NAME} "main.cc" "qml.qrc" "enhancedqmlapplicationengine.cc")
target_compile_definitions(${PROJECT_NAME} PRIVATE $<$<OR:$<CONFIG:Debug>,$<CONFIG:RelWithDebInfo>>:QT_QML_DEBUG>)
target_link_libraries(${PROJECT_NAME} PRIVATE Qt5::Core Qt5::Quick)

add_executable(dynamic_scene "dynamic_scene/main.qml" "dynamic_scene/create-object.js" "dynamic_scene/ufo.qml" "dynamic_scene/rocket.qml")
target_link_libraries(dynamic_scene PRIVATE Qt5::Core Qt5::Quick)
