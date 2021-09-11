set(PREFIX google_test)

find_package(GTest CONFIG REQUIRED)
add_single_file_targets(PREFIX ${PREFIX} DIR ${CMAKE_CURRENT_SOURCE_DIR} LIBS GTest::gmock GTest::gtest GTest::gmock_main GTest::gtest_main)
add_test(NAME simple COMMAND google_test_simple)