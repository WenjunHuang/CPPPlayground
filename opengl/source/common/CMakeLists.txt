add_library(glad STATIC glad/glad.h glad/glad.c)
target_include_directories(glad PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

add_library(stb_image STATIC stb/stb_image.h stb/stb_image.cc)
target_include_directories(stb_image PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
