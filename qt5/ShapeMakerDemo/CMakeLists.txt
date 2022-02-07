set(PREFIX ShapeMakerDemo)
add_executable(ShapeMakerDemo ShapeMakerDemo.cc mainwindow.ui resource.qrc
               mainwindow.cc shape_canvas.cc shape_canvas.h)
target_link_libraries(ShapeMakerDemo PRIVATE Qt5::Core Qt5::Widgets)
