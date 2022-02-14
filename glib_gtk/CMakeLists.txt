
set(PREFIX glib_gtk)
find_program(PKG_CONFIG_EXECUTABLE NAMES pkgconf.exe pkg-config.exe)
find_package(PkgConfig REQUIRED)
pkg_check_modules(GTKMM REQUIRED IMPORTED_TARGET gtkmm-4.0)
add_single_file_targets(PREFIX ${PREFIX} DIR ${CMAKE_CURRENT_SOURCE_DIR} LIBS PkgConfig::GTKMM)
add_dir_target(PREFIX ${PREFIX} DIR ${CMAKE_CURRENT_SOURCE_DIR} LIBS PkgConfig::GTKMM)