set(PREFIX imgui)
set(CMAKE_CXX_STANDARD 17)

find_package(imgui CONFIG REQUIRED)
find_package(glfw3 CONFIG REQUIRED)
find_package(GLEW REQUIRED)
find_package(CURL CONFIG REQUIRED)
find_package(rxcpp CONFIG REQUIRED)
find_package(range-v3 CONFIG REQUIRED)
find_package(cpprestsdk CONFIG REQUIRED)

add_executable(rxcpp_imgui_twitter rxcurl.h model.h tweets.h util.h rximgui.h json.hpp main.cc )
target_link_libraries(rxcpp_imgui_twitter PRIVATE rxcpp imgui::imgui glfw GLEW::GLEW CURL::libcurl range-v3 range-v3-meta range-v3::meta range-v3-concepts  cpprestsdk::cpprest)

add_subdirectory(test)