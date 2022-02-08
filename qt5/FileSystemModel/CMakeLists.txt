add_executable(FileSystemModel main.cc mainwindow.ui mainwindow.cpp mainwindow.h)
target_link_libraries(FileSystemModel PRIVATE Qt5::Core Qt5::Widgets)