/**
 * @file generic_macro_test.c
 * @author DuYicheng
 * @date 2025-07-13
 * @brief C 语言的 _Generic 宏测试
 */

#include <stdio.h>
#include <string.h>

#include "minunit.h"

#include "nva/format.h"

#define STR_EQ(a, b) (strcmp((a), (b)) == 0)
#define EQU(a, b)    ((a) == (b))

#define NVA_TEST_FMT(dst, format, status, expect)                                       \
    do {                                                                                \
        mu_assert(EQU(nva_format((dst), (format), (status)), NVA_SUCCESS), "not equ!"); \
        mu_assert_string_eq((dst), (expect));                                           \
        memset((dst), 0, NVA_COUNTOF(dst));                                             \
    } while (0)

MU_TEST(mu_test)
{
    mu_check(7 == 7);
}

MU_TEST(NoneFmtTest)
{
    char dst[100];
    mu_assert(nva_format(dst, "No!", NVA_ERROR) == NVA_FAIL, "not equ!");
    mu_assert(EQU(nva_format(NULL, "No!", NVA_START), NVA_PARAM_ERROR), "not equ!");
    mu_assert(EQU(nva_format(dst, NULL, NVA_START), NVA_PARAM_ERROR), "not equ!");
    mu_assert(EQU(nva_format(NULL, NULL, NVA_START), NVA_PARAM_ERROR), "not equ!");

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
}

MU_TEST(IntegerTestBasic)
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
    NVA_TEST_FMT(dst,
                 "arr = [{2:<.5d}, {0:*<3.3d}, {1:<#5.4}].\n",
                 nva_add(123, nva_add(28510, nva_add(32, NVA_START))),
                 "arr = [32, 123, 28510].\n");

    NVA_TEST_FMT(dst,
                 "arr = [{2:d}, {0:3d}, {1:#5}].\n",
                 nva_int(1, nva_int(2, nva_int(3, NVA_START))),
                 "arr = [3,   1,     2].\n");
    NVA_TEST_FMT(dst,
                 "arr = [{2:>.5d}, {0:*>3.3d}, {1::>#5.4}].\n",
                 nva_int(1, nva_int(2, nva_int(3, NVA_START))),
                 "arr = [3, **1, ::::2].\n");
    NVA_TEST_FMT(dst,
                 "arr = [{2:>.5d}, {0:*>3.3d}, {1:>#5.4}].\n",
                 nva_add(123, nva_add(28510, nva_add(32, NVA_START))),
                 "arr = [32, 123, 28510].\n");

    NVA_TEST_FMT(dst,
                 "arr = [{2:^d}, {0:^3d}, {1:^#5}].\n",
                 nva_int(1, nva_int(2, nva_int(378512, NVA_START))),
                 "arr = [378512,  1 ,   2  ].\n");
    NVA_TEST_FMT(dst,
                 "arr = [{2:^.5d}, {0:*^3.3d}, {1::^#5.4}].\n",
                 nva_int(1, nva_int(2, nva_int(3, NVA_START))),
                 "arr = [3, *1*, ::2::].\n");
    NVA_TEST_FMT(dst,
                 "arr = [{2:^.5d}, {0:*^3.3d}, {1::^#5.4}].\n",
                 nva_int(12, nva_int(27, nva_int(3, NVA_START))),
                 "arr = [3, 12*, :27::].\n");
    NVA_TEST_FMT(dst,
                 "arr = [{2:^.5d}, {0:*^3.3d}, {1:^#5.4}].\n",
                 nva_add(123, nva_add(28510, nva_add(32, NVA_START))),
                 "arr = [32, 123, 28510].\n");
}

MU_TEST(IntegerTestPlusChar)
{
    char dst[100] = {0};

    NVA_TEST_FMT(dst,
                 "arr = [{2:<-d}, {0:<+3d}, {1:<#5}].\n",
                 nva_int(1, nva_int(-2, nva_int(3, NVA_START))),
                 "arr = [3, +1 , -2   ].\n");
    NVA_TEST_FMT(dst,
                 "arr = [{2:< .5d}, {0:*<-3.3d}, {1:<#+5.4}].\n",
                 nva_int(1, nva_int(245, nva_int(3, NVA_START))),
                 "arr = [ 3, 1**, +245 ].\n");
    NVA_TEST_FMT(dst,
                 "arr = [{2:<.5d}, {0:*< 3.3d}, {1:<+#5.4}].\n",
                 nva_add(1, nva_add(2, nva_add(3, NVA_START))),
                 "arr = [3,  1*, +2   ].\n");
    NVA_TEST_FMT(dst,
                 "arr = [{2:<.5d}, {0:*<+3.3d}, {1:<#5.4}].\n",
                 nva_add(123, nva_add(28510, nva_add(32, NVA_START))),
                 "arr = [32, +123, 28510].\n");

    NVA_TEST_FMT(dst,
                 "arr = [{2:-d}, {0:+3d}, {1:+#5}].\n",
                 nva_int(1, nva_int(2, nva_int(3, NVA_START))),
                 "arr = [3,  +1,    +2].\n");
    NVA_TEST_FMT(dst,
                 "arr = [{2:>.5d}, {0:*> 3.3d}, {1::> #5.4}].\n",
                 nva_int(1, nva_int(2, nva_int(3, NVA_START))),
                 "arr = [3, * 1, ::: 2].\n");
    NVA_TEST_FMT(dst,
                 "arr = [{2:>-.5d}, {0:*>+3.3d}, {1:> #5.4}].\n",
                 nva_add(123, nva_add(28510, nva_add(32, NVA_START))),
                 "arr = [32, +123,  28510].\n");

    NVA_TEST_FMT(dst,
                 "arr = [{2:^+d}, {0:^+3d}, {1:^+#5}].\n",
                 nva_int(1, nva_int(2, nva_int(378512, NVA_START))),
                 "arr = [+378512, +1 ,  +2  ].\n");
    NVA_TEST_FMT(dst,
                 "arr = [{2:^ .5d}, {0:*^ 3.3d}, {1::^+#5.4}].\n",
                 nva_int(1, nva_int(2, nva_int(3, NVA_START))),
                 "arr = [ 3,  1*, :+2::].\n");
    NVA_TEST_FMT(dst,
                 "arr = [{2:^.5x}, {0:*^#3.3X}, {1::^#5.4x}].\n",
                 nva_int(0x12, nva_int(0x27, nva_int(0x3, NVA_START))),
                 "arr = [3, 0X12, 0x27:].\n");
    NVA_TEST_FMT(dst,
                 "arr = [{2:^.5d}, {0:*^3.3d}, {1:^#5.4}].\n",
                 nva_add(123, nva_add(28510, nva_add(32, NVA_START))),
                 "arr = [32, 123, 28510].\n");
}

MU_TEST_SUITE(SameFromFormatTest)
{
    MU_RUN_TEST(mu_test);
    MU_RUN_TEST(NoneFmtTest);
    MU_RUN_TEST(IntegerTestBasic);
    MU_RUN_TEST(IntegerTestPlusChar);
}

int generic_macro_test_main(void)
{
    printf("munit messages:\n");

    MU_RUN_SUITE(SameFromFormatTest);

    MU_REPORT();

    printf("munit messages end.\n\n");
    return MU_EXIT_CODE;
}
