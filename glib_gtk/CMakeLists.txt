set(PREFIX gtklearn)
find_program(PKG_CONFIG_EXECUTABLE NAMES pkgconf.exe pkg-config.exe)
find_package(PkgConfig)
pkg_check_modules(GTKMM REQUIRED IMPORTED_TARGET gtkmm-4.0)
#find_package(gtk4 CONFIG REQUIRED)

add_single_file_targets(PREFIX ${PREFIX} DIR ${CMAKE_CURRENT_SOURCE_DIR} LIBS PkgConfig::GTKMM)
add_dir_target(PREFIX ${PREFIX} DIR ${CMAKE_CURRENT_SOURCE_DIR} LIBS PkgConfig::GTKMM)
