set(PREFIX ListWidget)
add_executable(ListWidget
               main.cc
               resource.qrc
               mainwindow.cc
               mainwindow.ui)
target_link_libraries(ListWidget PRIVATE Qt5::Core Qt5::Widgets)