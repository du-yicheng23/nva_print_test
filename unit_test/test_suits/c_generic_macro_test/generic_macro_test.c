/**
 * @file generic_macro_test.c
 * @author DuYicheng
 * @date 2025-07-13
 * @brief C 语言的 _Generic 宏测试
 */

#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdint.h>

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

static const char* ptr_to_string(void* ptr, const uint8_t base, const bool prefix, const bool upper_case)
{
    static char str[20] = {0};
    char fmt[10] = "%";

    if (prefix) {
        strcat(fmt, "#");
    }

    strcat(fmt, "z");

    switch (base) {
    case 2:
        goto print_bindary;
    case 8:
        strcat(fmt, "o");
        break;
    case 10:
        strcat(fmt, "u");
        break;
    case 16:
        strcat(fmt, (upper_case ? "X" : "x"));
        break;
    default:
        break;
    }

    sprintf(str, fmt, (NVA_SIZE_T)ptr);

    return str;

print_bindary:
    str[0] = '\0';

    if (prefix) {
        strcat(str, (upper_case ? "0B" : "0b"));
    }

    bool is_1 = false;
    NVA_SIZE_T mask = 0x8000000000000000ULL;
    for (; mask != 0U; mask >>= 1) {
        char buffer[2] = {0};
        sprintf(buffer, "%d", ((NVA_SIZE_T)ptr & mask) ? 1 : 0);  // 按位与运算，逐一输出 n 的每一位
        if (!is_1 && strcmp(buffer, "1") == 0) {
            is_1 = true;
        }
        if (is_1) {
            strcat(str, buffer);
        }
    }

    return str;
}

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

MU_TEST(CharTest)
{
    char dst[100];

    NVA_TEST_FMT(dst, "{}", nva_char('1', NVA_START), "1");
    NVA_TEST_FMT(dst, "{}", nva_char('d', NVA_START), "d");
    NVA_TEST_FMT(dst, "{:+}", nva_char('h', NVA_START), "h");
    NVA_TEST_FMT(dst, "{: c}", nva_char('J', NVA_START), "J");

    NVA_TEST_FMT(dst,
                 "arr = [{}, {}, {}].\n",
                 nva_char('1', nva_char('\n', nva_char('l', NVA_START))),
                 "arr = [1, \n, l].\n");
    NVA_TEST_FMT(dst,
                 "arr = [{2}, {0}, {1}].\n",
                 nva_char('\a', nva_char('P', nva_char('q', NVA_START))),
                 "arr = [q, \a, P].\n");
    NVA_TEST_FMT(dst,
                 "arr = [{2:<d}, {0:<3d}, {1:<#5}].\n",
                 nva_char('\\', nva_char('%', nva_char('\'', NVA_START))),
                 "arr = [\', \\  , %    ].\n");
    NVA_TEST_FMT(dst,
                 "arr = [{2:<.5d}, {0:*<3.3d}, {1:<#5.4}].\n",
                 nva_char('m', nva_char('\t', nva_char('f', NVA_START))),
                 "arr = [f, m**, \t    ].\n");
}

MU_TEST(StringTest)
{
    char dst[500];

    NVA_TEST_FMT(dst, "{}", nva_str("Hello, World!", NVA_START), "Hello, World!");
    NVA_TEST_FMT(dst, "{}{}", nva_str("Hello, World!", nva_str("\nI\'m nva.", NVA_START)), "Hello, World!\nI\'m nva.");

    NVA_TEST_FMT(dst,
                 "12{}{}34",
                 nva_str("Hello, World!", nva_str("\nI\'m nva.", NVA_START)),
                 "12Hello, World!\nI\'m nva.34");
    NVA_TEST_FMT(dst,
                 "12{:s}{:s}34",
                 nva_str("Hello, World!", nva_str("\nI\'m nva.", NVA_START)),
                 "12Hello, World!\nI\'m nva.34");

    NVA_TEST_FMT(dst,
                 "12{:15}{:>12}34",
                 nva_str("Hello, World!", nva_str("\nI\'m nva.", NVA_START)),
                 "12Hello, World!     \nI\'m nva.34");
    NVA_TEST_FMT(dst,
                 "12{:<15}{:>12}34",
                 nva_str("Hello, World!", nva_str("\nI\'m nva.", NVA_START)),
                 "12Hello, World!     \nI\'m nva.34");
}

// 先测试辅助函数 ptr_to_string
MU_TEST(PtrTest_ptr_to_string_FuncTest)
{
    mu_assert_string_eq(ptr_to_string((void*)0x12345, 16, true, false), "0x12345");
    mu_assert_string_eq(ptr_to_string((void*)0x13462, 16, true, true), "0X13462");
    mu_assert_string_eq(ptr_to_string((void*)0x13462, 16, false, true), "13462");
    mu_assert_string_eq(ptr_to_string((void*)013462, 8, false, true), "13462");
    mu_assert_string_eq(ptr_to_string((void*)2313462, 10, false, true), "2313462");
    mu_assert_string_eq(ptr_to_string((void*)0x59, 2, true, true), "0B1011001");
    mu_assert_string_eq(ptr_to_string((void*)0x59, 2, true, false), "0b1011001");
    mu_assert_string_eq(ptr_to_string((void*)0x59, 2, false, false), "1011001");
}

MU_TEST(PtrTest)
{
    char dst[100] = {0};

    int a;
    int* p = &a;

    NVA_TEST_FMT(dst, "{}", nva_ptr(p, NVA_START), ptr_to_string(p, 16, true, false));
}

MU_TEST(FloatTest)
{
    char dst[100] = {0};

    NVA_TEST_FMT(dst, "{:.3f}", nva_add(1.456f, NVA_START), "1.456");
    NVA_TEST_FMT(dst, "{:.4f}", nva_add(0.456f, NVA_START), "0.4560");
    NVA_TEST_FMT(dst, "{:.2f}", nva_add(123.456f, NVA_START), "123.46");
    NVA_TEST_FMT(dst, "{:.0f}", nva_add(123.456f, NVA_START), "123");
    NVA_TEST_FMT(dst, "{:#.0f}", nva_add(123.456f, NVA_START), "123.");

    NVA_TEST_FMT(dst, "{:f}", nva_add(123.456, NVA_START), "123.456000");
    NVA_TEST_FMT(dst, "{:.3f}", nva_add(123.456, NVA_START), "123.456");
    NVA_TEST_FMT(dst, "{:.2f}", nva_add(123.456, NVA_START), "123.46");
    NVA_TEST_FMT(dst, "{:#.0f}", nva_add(123.456, NVA_START), "123.");

    NVA_TEST_FMT(dst, "{:9.3f}", nva_add(123.456, NVA_START), "  123.456");
    NVA_TEST_FMT(dst, "{:012f}", nva_add(123.456, NVA_START), "00123.456000");
    NVA_TEST_FMT(dst, "{:012.3f}", nva_add(123.456, NVA_START), "00000123.456");

    NVA_TEST_FMT(dst, "{:+<9.3f}", nva_add(123.456, NVA_START), "123.456++");
    NVA_TEST_FMT(dst, "{: <9.3f}", nva_add(123.456, NVA_START), "123.456  ");
    NVA_TEST_FMT(dst, "{:>09.3f}", nva_add(123.456, NVA_START), "  123.456");
    NVA_TEST_FMT(dst, "{:#>9.3f}", nva_add(123.456, NVA_START), "##123.456");
    NVA_TEST_FMT(dst, "{::^9.3f}", nva_add(123.456, NVA_START), ":123.456:");
    NVA_TEST_FMT(dst, "{:|^10.3f}", nva_add(123.456, NVA_START), "|123.456||");
    NVA_TEST_FMT(dst, "{:|^ 10.3f}", nva_add(123.456, NVA_START), "| 123.456|");
    NVA_TEST_FMT(dst, "{:|^+10.3f}", nva_add(123.456, NVA_START), "|+123.456|");
    NVA_TEST_FMT(dst, "{:|^-10.3f}", nva_add(123.456, NVA_START), "|123.456||");
}

MU_TEST_SUITE(SameFromFormatTest)
{
    MU_RUN_TEST(mu_test);
    MU_RUN_TEST(NoneFmtTest);
    MU_RUN_TEST(IntegerTestBasic);
    MU_RUN_TEST(IntegerTestPlusChar);
    MU_RUN_TEST(CharTest);
    MU_RUN_TEST(StringTest);
    MU_RUN_TEST(PtrTest_ptr_to_string_FuncTest);
    MU_RUN_TEST(PtrTest);
    MU_RUN_TEST(FloatTest);
}

int generic_macro_test_main(void)
{
    printf("munit messages:\n");

    MU_RUN_SUITE(SameFromFormatTest);

    MU_REPORT();

    printf("munit messages end.\n\n");
    return MU_EXIT_CODE;
}
