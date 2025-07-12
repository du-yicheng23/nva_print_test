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

TEST(FormatTest, IntegerTest)
{
    char dst[100];

    NVA_TEST_FMT(dst, "{}", nva_int(123, NVA_START), "123");
    NVA_TEST_FMT(dst, "{}", nva_int(-123, NVA_START), "-123");

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
}
