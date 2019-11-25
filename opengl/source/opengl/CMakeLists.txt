set(CMAKE_AUTOMOC TRUE)
set(CMAKE_AUTOUIC TRUE)
set(CMAKE_PREFIX_PATH ${QT_CMAKE})

find_package(OpenGL REQUIRED)
find_package(Qt5 REQUIRED COMPONENTS Core Gui OpenGL Widgets)

if (LINUX)
    find_package(OpenGL REQUIRED)
    find_package(PkgConfig REQUIRED)
    pkg_search_module(glfw REQUIRED IMPORTED_TARGET GLOBAL glfw3)
endif ()

add_executable(hello_window hello_window_clear.cc)
target_link_libraries(hello_window PRIVATE OpenGL::GL glfw glad)
if (LINUX)
    target_link_libraries(hello_window PRIVATE dl)
endif ()

add_executable(hello_triangle hello_triangle_rectangle.cc shader.h)
target_link_libraries(hello_triangle PRIVATE
        OpenGL::GL
        glfw
        glad
        stb_image)
file(COPY
        "${CMAKE_CURRENT_SOURCE_DIR}/vertex_hello_triangle_rectangle.glsl"
        "${CMAKE_CURRENT_SOURCE_DIR}/fragment_hello_triangle_rectangle.glsl"
        DESTINATION "${CMAKE_CURRENT_BINARY_DIR}")
if (LINUX)
    target_link_libraries(hello_triangle PRIVATE dl)
endif ()

add_executable(texture_combine
        textures_combined.cc
        OpenGLWindow.cc)
qt5_use_modules(texture_combine Core OpenGL Widgets Gui)

add_executable(transformations
        transformations.cc
        OpenGLWindow.cc)
qt5_use_modules(transformations Core OpenGL Widgets Gui)

add_executable(coordinate_systems_multiple
        coordinate_systems_multiple.cc
        OpenGLWindow.cc)
qt5_use_modules(coordinate_systems_multiple Core OpenGL Widgets Gui)







