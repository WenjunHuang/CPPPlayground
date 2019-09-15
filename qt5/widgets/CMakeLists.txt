cmake_minimum_required(VERSION 3.7)
project(qt5)

add_executable(simple_widgets simple.cc example.cc example.h)
target_link_libraries(simple_widgets PRIVATE Qt5::Widgets)

add_executable(menu_and_status_bar menu_and_status_bar.cc menu_and_status_bar.h)
target_link_libraries(menu_and_status_bar PRIVATE Qt5::Widgets)
