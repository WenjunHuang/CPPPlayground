project(ImageProvider)

add_executable(ImageProvider main.cc my_image_provider.cc my_image_provider.h resources.qrc http_worker.cc http_worker.h)
target_link_libraries(ImageProvider PRIVATE Qt5::Core Qt5::Network Qt5::Quick)