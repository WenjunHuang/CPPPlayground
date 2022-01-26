cmake_minimum_required(VERSION 3.15)
project(CPPPlayground)
enable_testing()
set(CMAKE_CXX_STANDARD 20)
#if (MSVC)
#    add_compile_options("/Zc:__cplusplus" "/utf-8" "/std:c++latest")
#endif ()
if (APPLE)
    list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake")
endif ()

# 单个cc文件
function(add_single_file_targets)
    include(CMakeParseArguments)
    set(options TEST)
    set(singleValues DIR PREFIX)
    set(multiValues LIBS INCLUDES DEPS)
    cmake_parse_arguments(ARG
            "${options}"
            "${singleValues}"
            "${multiValues}"
            ${ARGN})

    if (ARG_DIR)
        set(TARGETS "")
        file(GLOB SRCS ${ARG_DIR}/*.cc)
        foreach (SRC_FILE IN LISTS SRCS)
            get_filename_component(FILE_NAME ${SRC_FILE} NAME_WLE)
            string(REGEX MATCHALL "([A-Za-z0-9_]+)-?.*" MATCHES "${FILE_NAME}")
            if (MATCHES)
                set(TARGET_NAME ${PREFIX}_${CMAKE_MATCH_1})
                list(FIND TARGETS ${TARGET_NAME} IDX)
                if (IDX EQUAL "-1")
                    list(APPEND TARGETS ${TARGET_NAME})
                    set(${TARGET_NAME}_files ${SRC_FILE})
                else ()
                    list(APPEND ${TARGET_NAME}_files ${SRC_FILE})
                endif ()
            endif ()
        endforeach ()

        foreach (TARGET IN LISTS TARGETS)
            add_executable(${TARGET} ${${TARGET}_files})
            if (ARG_LIBS)
                target_link_libraries(${TARGET} PRIVATE ${ARG_LIBS})
            endif ()
            if (ARG_INCLUDES)
                target_include_directories(${TARGET} PRIVATE ${ARG_INCLUDES})
            endif ()
            if (ARG_DEPS)
                add_dependencies(${TARGET} ${ARG_DEPS})
            endif ()
            if (ARG_TEST)
                add_test(NAME ${TARGET} COMMAND ${TARGET})
            endif ()
            #            if (WIN32)
            #                target_compile_options(${TARGET} PRIVATE -fsanitize=address)
            #                target_link_directories(${TARGET} PRIVATE "$ENV{ProgramFiles\(x86\)}/Microsoft Visual Studio/2019/Community/VC/Tools/Llvm/x64/lib/clang/12.0.0/lib/windows")
            #                target_link_libraries(${TARGET} PRIVATE clang_rt.asan_dynamic-x86_64 clang_rt.asan_dynamic_runtime_thunk-x86_64)
            #                target_link_options(${TARGET} PRIVATE /wholearchive:clang_rt.asan_dynamic_runtime_thunk-x86_64.lib)
            #            endif ()
        endforeach ()
    else ()
        message(FATAL_ERROR "please specify DIR")
    endif ()
endfunction()

function(add_dir_target)
    include(CMakeParseArguments)
    set(singleValues DIR PREFIX)
    set(multiValues LIBS INCLUDES DEPS)
    cmake_parse_arguments(ARG
            ""
            "${singleValues}"
            "${multiValues}"
            ${ARGN})
    if (ARG_PREFIX AND ARG_DIR)
        if (IS_DIRECTORY ${ARG_DIR})
            file(GLOB children ${ARG_DIR}/*)
            foreach (child IN LISTS children)
                IF (IS_DIRECTORY ${child})
                    file(GLOB files LIST_DIRECTORIES false ${child}/*)
                    get_filename_component(dir_name ${child} NAME)
                    add_executable(${ARG_PREFIX}_${dir_name} ${files})
                    if (ARG_LIBS)
                        target_link_libraries(${ARG_PREFIX}_${dir_name} PRIVATE ${ARG_LIBS})
                    endif ()
                    if (ARG_INCLUDES)
                        target_include_directories(${ARG_PREFIX}_${dir_name} PRIVATE ${ARG_INCLUDES})
                    endif ()
                    if (ARG_DEPS)
                        add_dependencies(${ARG_PREFIX}_${dir_name} ${ARG_DEPS})
                    endif ()
                ENDIF ()
            endforeach ()
        else ()
            message(SEND_ERROR "${ARG_DIR} is not a directory")
        endif ()
    endif ()
endfunction()

function(create_test)
    include(CMakeParseArguments)
    set(singleValues NAME)
    set(multiValues SRCS LIBS)
    cmake_parse_arguments(ARG
            ""
            "${singleValues}"
            "${multiValues}"
            ${ARGN})
    add_executable(${ARG_NAME} ${ARG_SRCS})
    target_link_libraries(${ARG_NAME} PRIVATE
            "${ARG_LIBS}"
            GTest::gmock GTest::gtest GTest::gtest_main)
endfunction()

# common assets files used in sub projects
add_custom_target(assets
        COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_LIST_DIR}/assets ${CMAKE_CURRENT_BINARY_DIR}/assets
        )
#add_subdirectory(skia)
add_subdirectory(qt5)
#add_subdirectory(imgui_play)
#add_subdirectory(range)
#add_subdirectory(text_unicode_shape_render)
#add_subdirectory(coroutines)
#add_subdirectory(google_test)
#add_subdirectory(libtxt)
## The gtest/gtest_main targets carry header search path
## dependencies automatically when using CMake 2.8.11 or
## later. Otherwise we have to add them here ourselves.
#if (CMAKE_VERSION VERSION_LESS 2.8.11)
#    include_directories("${gtest_SOURCE_DIR}/include")
#endif()
#
#include_directories(~/github/boost)
#set(SOURCE_FILES main.cpp string_algrithm.h boost_xpressive.cpp)
#add_executable(${PROJECT_NAME} ${SOURCE_FILES})
#target_link_libraries(${PROJECT_NAME} gtest_main)
#add_test(NAME ${PROJECT_NAME}_test COMMAND ${PROJECT_NAME})