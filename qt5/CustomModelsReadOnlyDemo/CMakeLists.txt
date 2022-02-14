set(CMAKE_CXX_STANDARD 17)
find_package(Immer CONFIG REQUIRED)
add_executable(CustomModelsReadOnlyDemo main.cc person.cc person.h mainwindow.ui mainwindow.cc mainwindow.h person_model.cc person_model.h)
target_link_libraries(CustomModelsReadOnlyDemo PRIVATE Qt5::Core Qt5::Widgets immer)