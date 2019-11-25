project(attached_property)

find_package(Qt5 REQUIRED Core Quick)

add_executable(attached_property main.cc resource.qrc message.cc message.h)
qt5_use_modules(attached_property LINK_PRIVATE Core Quick)
