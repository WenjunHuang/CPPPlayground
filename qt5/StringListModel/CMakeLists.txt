add_executable(StringListModel main.cc
               mainwindow.h
               mainwindow.cc
               mainwindow.ui)
target_link_libraries(StringListModel PRIVATE Qt5::Core Qt5::Widgets)