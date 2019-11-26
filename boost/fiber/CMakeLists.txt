cmake_minimum_required(VERSION 3.13)

project(fiber LANGUAGES CXX)

#find_package(Qt5 COMPONENTS Core Quick REQUIRED)

add_executable(${PROJECT_NAME} "fiber.cc")
target_link_libraries(${PROJECT_NAME} PRIVATE Boost::fiber)

