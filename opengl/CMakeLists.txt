if(UNIX AND NOT APPLE)
    set(LINUX TRUE)
endif()

add_subdirectory(source/common)
add_subdirectory(source/opengl)
add_subdirectory(source/glfw)