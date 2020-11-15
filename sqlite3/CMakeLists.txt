
if (WIN32)
    find_package(unofficial-sqlite3 CONFIG REQUIRED)
    list(APPEND LIBS unofficial::sqlite3::sqlite3 )
endif ()

list(APPEND CMAKE_PREFIX_PATH ${QT_CMAKE})
find_package(Qt5 REQUIRED Core)
list(APPEND LIBS Qt5::Core)

set(PREFIX sqlite3)

file(GLOB_RECURSE SRCS *.cc)
set(TARGETS "")
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
    target_link_libraries(${TARGET} PRIVATE ${LIBS})
endforeach ()
