set(CMAKE_CXX_STANDARD 17)
add_executable(CustomModelsReadOnlyDemo main.cc person.cc person.h mainwindow.ui mainwindow.cc mainwindow.h person_model.cc person_model.h person_delegate.cc person_delegate.h)
target_link_libraries(CustomModelsReadOnlyDemo PRIVATE Qt${QT_VERSION_MAJOR}::Core Qt${QT_VERSION_MAJOR}::Widgets)