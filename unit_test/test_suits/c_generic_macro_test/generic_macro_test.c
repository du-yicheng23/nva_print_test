/**
 * @file generic_macro_test.c
 * @author DuYicheng
 * @date 2025-07-13
 * @brief C 语言的 _Generic 宏测试
 */

#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#include "minunit.h"

#include "nva/format.h"

int tests_run = 0;

unsigned int assert_fail_line = 0;
volatile const char* assert_fail_file = NULL;

static char msg_buffer[200] = {0};

#define STR_EQ(a, b) (strcmp(a, b) == 0)
#define EQU(a, b)    (a == b)

#define NVA_TEST_FMT(dst, format, status, expect)                                       \
    do {                                                                                \
        MU_ASSERT("not equ!", EQU(nva_format((dst), (format), (status)), NVA_SUCCESS)); \
        MU_ASSERT("str not equ!", STR_EQ((dst), (expect)));                             \
        memset((dst), 0, NVA_COUNTOF(dst));                                             \
    } while (0)

MU_TEST_SUIT(mu_test)
{
    MU_ASSERT("true is not true!", true);
    MU_ASSERT("0 != 0", 0 == 0);

    MU_SUCCEED();
}

MU_TEST_SUIT(NoneFmtTest)
{
    char dst[100];
    MU_ASSERT("not equ!", EQU(nva_format(dst, "No!", NVA_ERROR), NVA_FAIL));
    MU_ASSERT("not equ!", EQU(nva_format(NULL, "No!", NVA_START), NVA_PARAM_ERROR));
    MU_ASSERT("not equ!", EQU(nva_format(dst, NULL, NVA_START), NVA_PARAM_ERROR));
    MU_ASSERT("not equ!", EQU(nva_format(NULL, NULL, NVA_START), NVA_PARAM_ERROR));

    NVA_TEST_FMT(dst, "Hello, World!", NVA_START, "Hello, World!");
    NVA_TEST_FMT(dst, "", NVA_START, "");

    NVA_TEST_FMT(dst, "{{}}", NVA_START, "{}");
    NVA_TEST_FMT(dst, "std::vector vec{{1, 2, 3, 4}};", NVA_START, "std::vector vec{1, 2, 3, 4};");
    NVA_TEST_FMT(
        dst,
        "std::array<std::array<int, 3>, 3> arr{{{{1, 2, 3}}, {{4, 5, 6}}, {{7, 8, 9}}}}; std::cout << arr[0][0] << "
        "std::endl;",
        NVA_START,
        "std::array<std::array<int, 3>, 3> arr{{1, 2, 3}, {4, 5, 6}, {7, 8, 9}}; std::cout << arr[0][0] << std::endl;");

    MU_SUCCEED();
}

MU_TEST_SUIT(IntegerTest)
{
    char dst[100];

    NVA_TEST_FMT(dst, "{}", nva_add(123, NVA_START), "123");
    NVA_TEST_FMT(dst, "{}", nva_add(-123, NVA_START), "-123");

    NVA_TEST_FMT(dst, "arr = [{}, {}, {}].\n", nva_add(1, nva_add(2, nva_add(3, NVA_START))), "arr = [1, 2, 3].\n");
    NVA_TEST_FMT(dst, "arr = [{2}, {0}, {1}].\n", nva_add(1, nva_add(2, nva_add(3, NVA_START))), "arr = [3, 1, 2].\n");
    NVA_TEST_FMT(dst,
                 "arr = [{2:<d}, {0:<3d}, {1:<#5}].\n",
                 nva_add(1, nva_add(2, nva_add(3, NVA_START))),
                 "arr = [3, 1  , 2    ].\n");
    NVA_TEST_FMT(dst,
                 "arr = [{2:<.5d}, {0:*<3.3d}, {1:<#5.4}].\n",
                 nva_add(1, nva_add(2, nva_add(3, NVA_START))),
                 "arr = [3, 1**, 2    ].\n");
}

static const char* mu__all_test_list(void)
{
    MU_RUN_TEST_SUIT(mu_test);
    MU_RUN_TEST_SUIT(NoneFmtTest);
    MU_RUN_TEST_SUIT(IntegerTest);

    MU_SUCCEED();
}

const char* generic_macro_test_main(void)
{
    const char* const msg = mu__all_test_list();

    if (msg != NULL) {
        sprintf(msg_buffer, "MU: Assert Failed at %s:%u:\n    %s", assert_fail_file, assert_fail_line, msg);
        fprintf(stderr, "%s", msg_buffer);
        return msg_buffer;
    }

    return NULL;
}
