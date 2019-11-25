project(qtquick_popup)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY bin)
find_package(Qt5 REQUIRED Core Quick)

add_executable(qtquick_popup main.cc resource.qrc message.cc message.h)
qt5_use_modules(qtquick_popup LINK_PRIVATE Core Quick)
