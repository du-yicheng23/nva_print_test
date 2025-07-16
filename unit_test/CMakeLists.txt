cmake_minimum_required(VERSION 3.27)

project(unit_test C CXX)

set(CMAKE_C_STANDARD 11)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

message("Running ${PROJECT_NAME}.")

set(BUILD_GMOCK OFF)
add_subdirectory(./googletest)

option(UNIT_TEST_INLINE_MODE "Enable inline mode for unit tests" OFF)
option(UNIT_TEST_SUPPORT_INF_AND_NAN "Enable support for INF and NAN in unit tests" ON)

add_compile_options(-Winline)

add_subdirectory(../nva_print ../nva_print)
target_compile_definitions(nva_print INTERFACE -DNVA_ADD_USER_OPTIONS)
target_include_directories(nva_print INTERFACE ./nva_user_option/)

if (UNIT_TEST_INLINE_MODE)
    message("Unit test inline mode is enabled.")
    target_compile_definitions(nva_print INTERFACE -DNVA_USE_INLINE)
endif ()

add_executable(${PROJECT_NAME}
    test_suits/string_test.cpp
    test_suits/stack_test.cpp
    test_suits/format_test.cpp
    test_suits/nva_memcpy_test.cpp
    test_suits/nva_memmove_test.cpp

    test_suits/c_generic_macro_test/c_generic_macro_test.cpp
    test_suits/c_generic_macro_test/generic_macro_test.c

    test_suits/print_test.cpp
)

if (NOT ${UNIT_TEST_SUPPORT_INF_AND_NAN})
    target_compile_definitions(${PROJECT_NAME} PUBLIC -DNVA_NO_INF_AND_NAN)
endif ()

target_include_directories(${PROJECT_NAME} PRIVATE
    ./minunit/  # add minunit lib

    ./
    ./test_suits/c_generic_macro_test/
)

target_link_libraries(${PROJECT_NAME} PRIVATE
    GTest::gtest_main

    nva_print
)

include(GoogleTest)
gtest_discover_tests(${PROJECT_NAME})
