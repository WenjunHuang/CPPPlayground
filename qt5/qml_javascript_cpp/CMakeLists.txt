cmake_minimum_required(VERSION 3.13)

project(qml_javascript_cpp LANGUAGES CXX)

find_package(Qt5 COMPONENTS Core Quick REQUIRED)

add_executable(${PROJECT_NAME} "main.cc" "resource.qrc" )
target_link_libraries(${PROJECT_NAME} PRIVATE Qt5::Core Qt5::Quick)

