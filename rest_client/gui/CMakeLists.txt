set(CMAKE_CXX_STANDARD 17)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

find_package(Qt5 COMPONENTS Core Widgets REQUIRED)
add_executable(gui main.cc main_window.cc main_window.h)
target_link_libraries(gui PRIVATE Qt5::Core Qt5::Widgets)

enable_testing()