project(thread_event)

add_executable(thread_event thread_event.cc)
target_link_libraries(thread_event PRIVATE Qt5::Core Qt5::Widgets)

