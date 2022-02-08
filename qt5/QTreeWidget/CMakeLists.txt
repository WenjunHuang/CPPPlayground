set(PREFIX TreeWidget)
add_executable(TreeWidget
               main.cc
               resource.qrc
               mainwindow.cc
               mainwindow.ui)
target_link_libraries(TreeWidget PRIVATE Qt5::Core Qt5::Widgets)