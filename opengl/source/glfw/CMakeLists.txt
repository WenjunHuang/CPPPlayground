find_package(OpenGL REQUIRED)
find_package(PkgConfig REQUIRED)
pkg_search_module(GLFW REQUIRED glfw3)
include_directories(${GLFW_INCLUDE_DIRS})

add_executable(simple_glfw simple_glfw.cc)
target_link_libraries(simple_glfw PRIVATE ${GLFW_LIBRARIES})