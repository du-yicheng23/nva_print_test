cmake_minimum_required(VERSION 3.27)

project(unit_test C CXX)

set(CMAKE_C_STANDARD 11)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

message("Running ${PROJECT_NAME}.")

set(BUILD_GMOCK OFF)
add_subdirectory(./googletest)

option(UNIT_TEST_INLINE_MODE "Enable inline mode for unit tests" OFF)

add_compile_options(-Winline)

set(NVA_INLINE_MODE ${UNIT_TEST_INLINE_MODE})
add_subdirectory(../nva_print ../nva_print)
target_compile_definitions(nva_print INTERFACE -DNVA_ADD_USER_OPTIONS)
target_include_directories(nva_print INTERFACE ./nva_user_option/)

add_executable(${PROJECT_NAME}
    string_test.cpp
    stack_test.cpp
    format_test.cpp
    nva_memcpy_test.cpp
    nva_memmove_test.cpp
)

target_link_libraries(${PROJECT_NAME} PRIVATE
    GTest::gtest_main

    nva_print
)

include(GoogleTest)
gtest_discover_tests(${PROJECT_NAME})
