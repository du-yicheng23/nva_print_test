cmake_minimum_required(VERSION 3.27)

project(build_test C)

set(CMAKE_C_STANDARD 90)

add_executable(${PROJECT_NAME})

target_sources(${PROJECT_NAME} PRIVATE main.c)

add_subdirectory(../nva_print ../nva_print)

target_link_libraries(${PROJECT_NAME} PRIVATE nva_print)
