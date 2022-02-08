find_package(benchmark CONFIG REQUIRED)
add_executable(shareddatapointer_bm shareddatapointer_bm.cc)
target_link_libraries(shareddatapointer_bm  PRIVATE Qt5::Core benchmark::benchmark)
