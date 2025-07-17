cmake_minimum_required(VERSION 3.27)

project(nva_print_test)

if (NOT(DEFINED NVA_TEST_ITEM_NAME))
    message(FATAL_ERROR "TEST_ITEM_NAME is not defined. Please set it to run tests!")
else ()
    if (${NVA_TEST_ITEM_NAME} STREQUAL "unit_test")
        enable_testing()
    endif ()

    add_subdirectory(./${NVA_TEST_ITEM_NAME})
endif ()
