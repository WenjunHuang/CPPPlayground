set(PREFIX imgui)
find_package(imgui CONFIG REQUIRED)
find_package(glfw3 CONFIG REQUIRED)
find_package(GLEW REQUIRED)
find_package(glad CONFIG REQUIRED)

add_single_file_targets(PREFIX ${PREFIX} DIR ${CMAKE_CURRENT_SOURCE_DIR} LIBS imgui::imgui glfw GLEW::GLEW glad::glad)
add_dir_target(PREFIX ${PREFIX} DIR ${CMAKE_CURRENT_SOURCE_DIR} LIBS imgui::imgui glfw GLEW::GLEW glad::glad)


