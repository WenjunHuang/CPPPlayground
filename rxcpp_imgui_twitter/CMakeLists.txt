set(PREFIX imgui)
set(CMAKE_CXX_STANDARD 17)

find_package(imgui CONFIG REQUIRED)
find_package(glfw3 CONFIG REQUIRED)
find_package(GLEW REQUIRED)
find_package(CURL CONFIG REQUIRED)
find_package(rxcpp CONFIG REQUIRED)
find_package(range-v3 CONFIG REQUIRED)
find_package(cpprestsdk CONFIG REQUIRED)

add_library(rxcpp_imgui_lib STATIC  rxcurl.h model.h tweets.h util.h rximgui.h json.hpp)
target_link_libraries(rxcpp_imgui_lib PUBLIC  rxcpp imgui::imgui glfw GLEW::GLEW CURL::libcurl range-v3 range-v3-meta range-v3::meta range-v3-concepts  cpprestsdk::cpprest)
target_compile_definitions(rxcpp_imgui_lib PUBLIC "NOMINMAX")

add_executable(rxcpp_imgui_twitter main.cc)
target_link_libraries(rxcpp_imgui_twitter PRIVATE rxcpp_imgui_lib)
target_compile_definitions(rxcpp_imgui_twitter PRIVATE "NOMINMAX")

add_subdirectory(test)
