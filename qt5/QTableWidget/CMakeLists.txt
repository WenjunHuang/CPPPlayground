set(PREFIX TableWidget)
add_executable(TableWidget
               main.cc
               resource.qrc
               mainwindow.cc
               mainwindow.ui)
target_link_libraries(TableWidget PRIVATE Qt5::Core Qt5::Widgets)