/**
 * @file format_test.cpp
 * @author DuYicheng
 * @date 2025-07-11
 * @brief 格式化测试
 */

#include "gtest/gtest.h"

#include "nva/format.h"

#include <algorithm>
#include <cstdio>
#include <cstring>

#define NVA_TEST_FMT(dst, format, status, expect)                      \
    do {                                                               \
        EXPECT_EQ(nva_format((dst), (format), (status)), NVA_SUCCESS); \
        EXPECT_STREQ((dst), (expect));                                 \
    } while (0)

#define NVA_TEST_FMT_CPP(dst, fmt, status, expect)                   \
    do {                                                             \
        EXPECT_EQ(nva::format((dst), (fmt), (status)), NVA_SUCCESS); \
        EXPECT_STREQ((dst), (expect));                               \
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

TEST(FormatTest, NoneFmtTest)
{
    char dst[100];

    EXPECT_EQ(nva_format(dst, "No!", NVA_ERROR), NVA_FAIL);
    EXPECT_EQ(nva_format(nullptr, "No!", NVA_START), NVA_PARAM_ERROR);
    EXPECT_EQ(nva_format(dst, nullptr, NVA_START), NVA_PARAM_ERROR);
    EXPECT_EQ(nva_format(nullptr, nullptr, NVA_START), NVA_PARAM_ERROR);

    NVA_TEST_FMT(dst, "Hello, World!", NVA_START, "Hello, World!");
    NVA_TEST_FMT(dst, "", NVA_START, "");

    NVA_TEST_FMT(dst, "{{}}", NVA_START, "{}");
    NVA_TEST_FMT(dst, "std::vector vec{{1, 2, 3, 4}};", NVA_START, "std::vector vec{1, 2, 3, 4};");
    NVA_TEST_FMT_CPP(dst, "std::vector vec{{1, 2, 3, 4}};", NVA_START, "std::vector vec{1, 2, 3, 4};");
    NVA_TEST_FMT(
        dst,
        "std::array<std::array<int, 3>, 3> arr{{{{1, 2, 3}}, {{4, 5, 6}}, {{7, 8, 9}}}}; std::cout << arr[0][0] << "
        "std::endl;",
        NVA_START,
        "std::array<std::array<int, 3>, 3> arr{{1, 2, 3}, {4, 5, 6}, {7, 8, 9}}; std::cout << arr[0][0] << std::endl;");

    NVA_TEST_FMT(dst, "{{:|^+10.3f}}", nva_double(123.456, NVA_START), "{:|^+10.3f}");
    NVA_TEST_FMT(dst, "{{:|.2f}}", nva_double(123.456, NVA_START), "{:|.2f}");
}

TEST(FormatTest, IntegerTestBasic)
{
    char dst[100];

    NVA_TEST_FMT(dst, "{}", nva_int(123, NVA_START), "123");
    NVA_TEST_FMT(dst, "{}", nva_int(-123, NVA_START), "-123");
    NVA_TEST_FMT(dst, "{:+}", nva_int(123, NVA_START), "+123");
    NVA_TEST_FMT(dst, "{: }", nva_int(123, NVA_START), " 123");

    NVA_TEST_FMT(dst, "arr = [{}, {}, {}].\n", nva_int(1, nva_int(2, nva_int(3, NVA_START))), "arr = [1, 2, 3].\n");
    NVA_TEST_FMT(dst, "arr = [{2}, {0}, {1}].\n", nva_int(1, nva_int(2, nva_int(3, NVA_START))), "arr = [3, 1, 2].\n");
    NVA_TEST_FMT(dst,
                 "arr = [{2:<d}, {0:<3d}, {1:<#5}].\n",
                 nva_int(1, nva_int(2, nva_int(3, NVA_START))),
                 "arr = [3, 1  , 2    ].\n");
    NVA_TEST_FMT(dst,
                 "arr = [{2:<.5d}, {0:*<3.3d}, {1:<#5.4}].\n",
                 nva_int(1, nva_int(2, nva_int(3, NVA_START))),
                 "arr = [3, 1**, 2    ].\n");
    NVA_TEST_FMT_CPP(dst,
                     "arr = [{2:<.5d}, {0:*<3.3d}, {1:<#5.4}].\n",
                     nva::add(1, nva::add(2, nva::add(3, NVA_START))),
                     "arr = [3, 1**, 2    ].\n");
    NVA_TEST_FMT_CPP(dst,
                     "arr = [{2:<.5d}, {0:*<3.3d}, {1:<#5.4}].\n",
                     nva::add(123, nva::add(28510, nva::add(32, NVA_START))),
                     "arr = [32, 123, 28510].\n");

    NVA_TEST_FMT(dst,
                 "arr = [{2:d}, {0:3d}, {1:#5}].\n",
                 nva_int(1, nva_int(2, nva_int(3, NVA_START))),
                 "arr = [3,   1,     2].\n");
    NVA_TEST_FMT(dst,
                 "arr = [{2:>.5d}, {0:*>3.3d}, {1::>#5.4}].\n",
                 nva_int(1, nva_int(2, nva_int(3, NVA_START))),
                 "arr = [3, **1, ::::2].\n");
    NVA_TEST_FMT_CPP(dst,
                     "arr = [{2:>.5d}, {0:*>3.3d}, {1:>#5.4}].\n",
                     nva::add(123, nva::add(28510, nva::add(32, NVA_START))),
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
    NVA_TEST_FMT_CPP(dst,
                     "arr = [{2:^.5d}, {0:*^3.3d}, {1:^#5.4}].\n",
                     nva::add(123, nva::add(28510, nva::add(32, NVA_START))),
                     "arr = [32, 123, 28510].\n");

    NVA_TEST_FMT(dst,
                 "arr = [{:5d}, {:08d}, {:#07x}].\n",
                 nva_int(123, nva_int(1456, nva_int(0x653, NVA_START))),
                 "arr = [  123, 00001456, 0x00653].\n");
}

TEST(FormatTest, IntegerTestPlusChar)
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
    NVA_TEST_FMT_CPP(dst,
                     "arr = [{2:<.5d}, {0:*< 3.3d}, {1:<+#5.4}].\n",
                     nva::add(1, nva::add(2, nva::add(3, NVA_START))),
                     "arr = [3,  1*, +2   ].\n");
    NVA_TEST_FMT_CPP(dst,
                     "arr = [{2:<.5d}, {0:*<+3.3d}, {1:<#5.4}].\n",
                     nva::add(123, nva::add(28510, nva::add(32, NVA_START))),
                     "arr = [32, +123, 28510].\n");

    NVA_TEST_FMT(dst,
                 "arr = [{2:-d}, {0:+3d}, {1:+#5}].\n",
                 nva_int(1, nva_int(2, nva_int(3, NVA_START))),
                 "arr = [3,  +1,    +2].\n");
    NVA_TEST_FMT(dst,
                 "arr = [{2:>.5d}, {0:*> 3.3d}, {1::> #5.4}].\n",
                 nva_int(1, nva_int(2, nva_int(3, NVA_START))),
                 "arr = [3, * 1, ::: 2].\n");
    NVA_TEST_FMT_CPP(dst,
                     "arr = [{2:>-.5d}, {0:*>+3.3d}, {1:> #5.4}].\n",
                     nva::add(123, nva::add(28510, nva::add(32, NVA_START))),
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
    NVA_TEST_FMT_CPP(dst,
                     "arr = [{2:^.5d}, {0:*^3.3d}, {1:^#5.4}].\n",
                     nva::add(123, nva::add(28510, nva::add(32, NVA_START))),
                     "arr = [32, 123, 28510].\n");
}

TEST(FormatTest, CharTest)
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
                 nva::add('\\', nva::add('%', nva::add('\'', NVA_START))),
                 "arr = [\', \\  , %    ].\n");
    NVA_TEST_FMT(dst,
                 "arr = [{2:<.5d}, {0:*<3.3d}, {1:<#5.4}].\n",
                 nva::add('m', nva::add('\t', nva::add('f', NVA_START))),
                 "arr = [f, m**, \t    ].\n");
}

TEST(FormatTest, StringTest)
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
TEST(FormatTest, PtrTest_ptr_to_string_FuncTest)
{
    EXPECT_STREQ(ptr_to_string((void*)0x12345, 16, true, false), "0x12345");
    EXPECT_STREQ(ptr_to_string((void*)0x13462, 16, true, true), "0X13462");
    EXPECT_STREQ(ptr_to_string((void*)0x13462, 16, false, true), "13462");
    EXPECT_STREQ(ptr_to_string((void*)013462, 8, false, true), "13462");
    EXPECT_STREQ(ptr_to_string((void*)2313462, 10, false, true), "2313462");
    EXPECT_STREQ(ptr_to_string((void*)0x59, 2, true, true), "0B1011001");
    EXPECT_STREQ(ptr_to_string((void*)0x59, 2, true, false), "0b1011001");
    EXPECT_STREQ(ptr_to_string((void*)0x59, 2, false, false), "1011001");
}

TEST(FormatTest, PtrTest)
{
    char dst[100] = {0};

    int a;
    int* p = &a;

    NVA_TEST_FMT(dst, "{}", nva_ptr(p, NVA_START), ptr_to_string(p, 16, true, false));
}

TEST(FormatTest, FloatTest)
{
    char dst[100] = {0};

    NVA_TEST_FMT(dst, "{:.3f}", nva_float(1.456f, NVA_START), "1.456");
    NVA_TEST_FMT(dst, "{:.4f}", nva_float(0.456f, NVA_START), "0.4560");
    NVA_TEST_FMT_CPP(dst, "{:.2f}", nva::add(123.456f, NVA_START), "123.46");
    NVA_TEST_FMT_CPP(dst, "{:.0f}", nva::add(123.456f, NVA_START), "123");
    NVA_TEST_FMT_CPP(dst, "{:#.0f}", nva::add(123.456f, NVA_START), "123.");

    NVA_TEST_FMT(dst, "{:f}", nva_double(123.456, NVA_START), "123.456000");
    NVA_TEST_FMT(dst, "{:.3f}", nva_double(123.456, NVA_START), "123.456");
    NVA_TEST_FMT(dst, "{:.2f}", nva_double(123.456, NVA_START), "123.46");
    NVA_TEST_FMT(dst, "{:#.0f}", nva_double(123.456, NVA_START), "123.");

    NVA_TEST_FMT(dst, "{:9.3f}", nva_double(123.456, NVA_START), "  123.456");
    NVA_TEST_FMT(dst, "{:012f}", nva_double(123.456, NVA_START), "00123.456000");
    NVA_TEST_FMT(dst, "{:012.3f}", nva_double(123.456, NVA_START), "00000123.456");

    NVA_TEST_FMT(dst, "{:+<9.3f}", nva_double(123.456, NVA_START), "123.456++");
    NVA_TEST_FMT(dst, "{: <9.3f}", nva_double(123.456, NVA_START), "123.456  ");
    NVA_TEST_FMT(dst, "{:>09.3f}", nva_double(123.456, NVA_START), "  123.456");
    NVA_TEST_FMT(dst, "{:#>9.3f}", nva_double(123.456, NVA_START), "##123.456");
    NVA_TEST_FMT_CPP(dst, "{::^9.3f}", nva::add(123.456, NVA_START), ":123.456:");
    NVA_TEST_FMT_CPP(dst, "{:|^10.3f}", nva::add(123.456, NVA_START), "|123.456||");
    NVA_TEST_FMT_CPP(dst, "{:|^ 10.3f}", nva::add(123.456, NVA_START), "| 123.456|");
    NVA_TEST_FMT_CPP(dst, "{:|^+10.3f}", nva::add(123.456, NVA_START), "|+123.456|");
    NVA_TEST_FMT_CPP(dst, "{:|^-10.3f}", nva::add(123.456, NVA_START), "|123.456||");
}
