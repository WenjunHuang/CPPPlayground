cmake_minimum_required(VERSION 3.7)
project(BoostPlayground)

set(CMAKE_CXX_STANDARD 14)
configure_file(CMakeLists.txt.in googletest-download/CMakeLists.txt)
execute_process(COMMAND ${CMAKE_COMMAND} -G "${CMAKE_GENERATOR}" .
        RESULT_VARIABLE result
        WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/googletest-download )
if(result)
    message(FATAL_ERROR "CMake step for googletest failed: ${result}")
endif()
execute_process(COMMAND ${CMAKE_COMMAND} --build .
        RESULT_VARIABLE result
        WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/googletest-download )
if(result)
    message(FATAL_ERROR "Build step for googletest failed: ${result}")
endif()

# Prevent overriding the parent project's compiler/linker
# settings on Windows
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

# Add googletest directly to our build. This defines
# the gtest and gtest_main targets.
add_subdirectory(${CMAKE_BINARY_DIR}/googletest-src
        ${CMAKE_BINARY_DIR}/googletest-build)

set(BOOST_ROOT $ENV{BOOST_ROOT})
find_package(Boost REQUIRED
        COMPONENTS filesystem system)
include_directories(${Boost_INCLUDE_DIRS})

set(SOURCE_FILES main.cpp string_algrithm.h boost_xpressive.cpp tokenizer.cpp spirit.cpp multi_index.cpp)

add_executable(${PROJECT_NAME} ${SOURCE_FILES})
target_link_libraries(${PROJECT_NAME} gtest_main)
#target_link_libraries(${PROJECT_NAME} ${Boost_LIBRARIES})
add_test(NAME ${PROJECT_NAME}_test COMMAND ${PROJECT_NAME})