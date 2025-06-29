cmake_minimum_required(VERSION 3.27)

project(build_test C)

set(CMAKE_C_STANDARD 90)

message("Running ${PROJECT_NAME}.")

add_executable(${PROJECT_NAME})

target_sources(${PROJECT_NAME} PRIVATE main.c)

add_subdirectory(../nva_print ../nva_print)
target_compile_definitions(nva_print INTERFACE -DNVA_ADD_USER_OPTIONS -DNVA_NO_RESTRICT)
target_include_directories(nva_print INTERFACE ./nva_user_option/)

target_link_libraries(${PROJECT_NAME} PRIVATE nva_print)
