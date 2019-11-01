project(attached_property)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY bin)
set(CMAKE_PREFIX_PATH $ENV{QT_CMAKE})
find_package(Qt5 REQUIRED Core Quick)

add_executable(attached_property main.cc resource.qrc message.cc message.h)
qt5_use_modules(attached_property LINK_PRIVATE Core Quick)
