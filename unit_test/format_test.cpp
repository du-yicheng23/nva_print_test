/**
 * @file format_test.cpp
 * @author DuYicheng
 * @date 2025-07-11
 * @brief 格式化测试
 */

#include "gtest/gtest.h"

#include "nva/format.h"

#define NVA_TEST_FMT(dst, format, status, expect)                      \
    do {                                                               \
        EXPECT_EQ(nva_format((dst), (format), (status)), NVA_SUCCESS); \
        EXPECT_STREQ((dst), (expect));                                 \
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
}

TEST(FormatTest, IntegerTest)
{
    char dst[100];

    NVA_TEST_FMT(dst, "{}", nva_int(123, NVA_START), "123");
    NVA_TEST_FMT(dst, "{}", nva_int(-123, NVA_START), "-123");
}
