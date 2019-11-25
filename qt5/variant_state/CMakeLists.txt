cmake_minimum_required(VERSION 3.13)

project(variant_state LANGUAGES CXX)

find_package(Qt5 COMPONENTS Core Quick REQUIRED)

add_executable(${PROJECT_NAME} "variant_state.cc" "resource.qrc" )
target_link_libraries(${PROJECT_NAME} PRIVATE Qt5::Core Qt5::Quick)

