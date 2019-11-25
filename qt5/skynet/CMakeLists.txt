project(skynet)

add_executable(skynet main.cc)
target_link_libraries(skynet PRIVATE Qt5::Core Qt5::Concurrent)

