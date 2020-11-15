set(PREFIX imgui)
if (MSVC)
    find_package(imgui CONFIG REQUIRED)
    find_package(glfw3 CONFIG REQUIRED)
    find_package(GLEW REQUIRED)
    find_package(CURL CONFIG REQUIRED)
    find_package(rxcpp CONFIG REQUIRED)
endif ()

add_executable(rxcpp_imgui_twitter rxcurl.h model.h util.h json.hpp main.cc)
target_link_libraries(rxcpp_imgui_twitter PRIVATE rxcpp imgui::imgui glfw GLEW::GLEW CURL::libcurl )

