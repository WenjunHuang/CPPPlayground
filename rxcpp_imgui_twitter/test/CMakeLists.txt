set(CMAKE_CXX_STANDARD 17)
find_package(doctest CONFIG REQUIRED)

enable_testing(true)
set(PREFIX rit_test)
add_compile_definitions("NOMINMAX")
add_single_file_targets(PREFIX ${PREFIX} TEST DIR ${CMAKE_CURRENT_SOURCE_DIR} LIBS doctest::doctest rxcpp imgui::imgui glfw GLEW::GLEW CURL::libcurl range-v3 range-v3-meta range-v3::meta range-v3-concepts  cpprestsdk::cpprest INCLUDES "..")