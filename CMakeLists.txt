cmake_minimum_required(VERSION 3.27)

project(nva_print_test)

if (NOT(DEFINED NVA_TEST_ITEM_NAME))
    message(FATAL_ERROR "TEST_ITEM_NAME is not defined. Please set it to run tests!")
else ()
    if (${NVA_TEST_ITEM_NAME} STREQUAL "unit_test")
        enable_testing()
    elseif (${NVA_TEST_ITEM_NAME} STREQUAL "mcu_test")
        message(FATAL_ERROR "MCU Test is not supported to add subdirectory directly. Please read the documentation in mcu_test for more details.")
    endif ()

    add_subdirectory(./${NVA_TEST_ITEM_NAME})
endif ()
