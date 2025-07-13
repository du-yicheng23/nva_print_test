/**
 * @file minunit.h
 * @author DuYicheng
 * @date 2025-07-13
 * @brief A minimal unit testing framework for C
 * (copy and edited from https://jera.com/techinfo/jtns/jtn002)
 */

#pragma once
#ifndef MINUNIT_H
#define MINUNIT_H

#include <stddef.h>

extern int tests_run;
extern unsigned int assert_fail_line;
extern volatile const char* assert_fail_file;

#define MU_ASSERT(message, test)         \
    do {                                 \
        if (!(test)) {                   \
            assert_fail_file = __FILE__; \
            assert_fail_line = __LINE__; \
            return message;              \
        }                                \
    } while (0)

#define MU_RUN_TEST_SUIT(test)                   \
    do {                                         \
        const char* message = mu__test_##test(); \
        tests_run++;                             \
        if (message != NULL) {                   \
            return message;                      \
        }                                        \
    } while (0)

#define MU_TEST_SUIT(suit_name) static const char* mu__test_##suit_name(void)

#define MU_SUCCEED(void)        return NULL

#endif /* !MINUNIT_H */
