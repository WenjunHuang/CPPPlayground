
if (WIN32)
    find_package(sqlite3 CONFIG REQUIRED)
endif ()
list(APPEND CMAKE_PREFIX_PATH ${QT_CMAKE})
find_package(Qt5 REQUIRED Core)

file(GLOB_RECURSE SRCS *.cc)
set(TARGETS "")
foreach (SRC_FILE IN LISTS SRCS)
    get_filename_component(FILE_NAME ${SRC_FILE} NAME_WLE)
    string(REGEX MATCHALL "([A-Za-z0-9_]+)-?.*" MATCHES "${FILE_NAME}")
    if (MATCHES)
        set(TARGET_NAME ${CMAKE_MATCH_1})
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
    add_executable(${TARGET} ${${TARGET}_files} sqlite3_left_and_inner_join.cc)
    target_link_libraries(${TARGET} PRIVATE sqlite3 Qt5::Core)
#    add_custom_command(
#            TARGET ${TARGET} POST_BUILD
#            COMMAND ${CMAKE_COMMAND} -E copy
#            ${CMAKE_CURRENT_SOURCE_DIR}/${TARGET}.sqlite
#            ${CMAKE_CURRENT_BINARY_DIR}/${TARGET}.sqlite)
endforeach ()
