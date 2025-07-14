/**
 * @file format_test.cpp
 * @author DuYicheng
 * @date 2025-07-11
 * @brief 格式化测试
 */

#include "gtest/gtest.h"

#include "nva/format.h"

#include <algorithm>

#define NVA_TEST_FMT(dst, format, status, expect)                      \
    do {                                                               \
        EXPECT_EQ(nva_format((dst), (format), (status)), NVA_SUCCESS); \
        EXPECT_STREQ((dst), (expect));                                 \
        std::fill(dst, dst + std::size(dst), 0);                       \
    } while (0)

#define NVA_TEST_FMT_CPP(dst, fmt, status, expect)                   \
    do {                                                             \
        EXPECT_EQ(nva::format((dst), (fmt), (status)), NVA_SUCCESS); \
        EXPECT_STREQ((dst), (expect));                               \
        std::fill(dst, dst + std::size(dst), 0);                     \
    } while (0)

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
