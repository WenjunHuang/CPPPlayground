project(fontawesome)

find_package(Qt5 REQUIRED Core Quick)

add_executable(fontawesome main.cc resource.qrc)
qt5_use_modules(fontawesome LINK_PRIVATE Core Quick)
