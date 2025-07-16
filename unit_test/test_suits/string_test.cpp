/**
 * @file string_test.cpp
 * @author DuYicheng
 * @date 2025-06-28
 * @brief nva 字符串测试
 */

#include "gtest/gtest.h"

#include "nva/string.h"
#include "negative_zero.h"
#include "gtest_extend_message_types.hpp"

#include <string_view>
#include <array>
#include <climits>
#include <cstring>
#include <iostream>
#include <string>

TEST(StringTest, nva_strlen)
{
    const auto str = "Hello, World!";
    EXPECT_EQ(nva_strlen(str), 13);

    const auto empty_str = "";
    EXPECT_EQ(nva_strlen(empty_str), 0);

    const auto long_str = "This is a long string for testing.";
    EXPECT_EQ(nva_strlen(long_str), 34);
}

TEST(StringTest, nva_strcat)
{
    char dest[50] = "Hello, ";
    const char* src = "World!";
    EXPECT_STREQ(nva_strcat(dest, src), "Hello, World!");

    char empty_dest[50] = "";
    EXPECT_STREQ(nva_strcat(empty_dest, "Test"), "Test");

    char long_dest[50] = "This is a long string. ";
    char empty_src[50] = "";
    EXPECT_STREQ(nva_strcat(long_dest, empty_src), "This is a long string. ");
}

TEST(StringTest, nva_strcpy)
{
    char dest[50];
    const char* src = "Hello, World!";
    EXPECT_STREQ(nva_strcpy(dest, src), "Hello, World!");

    const char* empty_src = "";
    EXPECT_STREQ(nva_strcpy(dest, empty_src), "");

    const char* long_src = "This is a long string for testing.";
    EXPECT_STREQ(nva_strcpy(dest, long_src), "This is a long string for testing.");
}

TEST(StringTest, nva_strcmp)
{
    const char* str1 = "Hello";
    const char* str2 = "Hello";
    EXPECT_EQ(nva_strcmp(str1, str2), 0);

    const char* str3 = "World";
    EXPECT_LT(nva_strcmp(str1, str3), 0);
    EXPECT_GT(nva_strcmp(str3, str1), 0);

    const char* str4 = "Hello, World!";
    EXPECT_LT(nva_strcmp(str1, str4), 0);
    EXPECT_GT(nva_strcmp(str4, str1), 0);

    // referenced from https://cppreference.cn/w/c/string/byte/strcmp
    auto demo = [](const char* lhs, const char* rhs) {
        const int rc = nva_strcmp(lhs, rhs);
        const char* rel = rc < 0 ? "precedes" : rc > 0 ? "follows" : "equals";
        return rel;
    };
    const char* string = "Hello World!";
    EXPECT_STREQ(demo(string, "Hello!"), "precedes");
    EXPECT_STREQ(demo(string, "Hello"), "follows");
    EXPECT_STREQ(demo(string, "Hello there"), "precedes");
    EXPECT_STREQ(demo("Hello, everybody!" + 12, "Hello, somebody!" + 11), "equals");
}

TEST(StringTest, nva_memcpy)
{
    char dest[50];
    const char* src = "Hello, World!";
    EXPECT_EQ(nva_memcpy(dest, src, 14), dest);
    EXPECT_STREQ(dest, "Hello, World!");

    char empty_dest[50] = "";
    EXPECT_EQ(nva_memcpy(empty_dest, "Test", 5), empty_dest);
    EXPECT_STREQ(empty_dest, "Test");

    const char long_src[50] = "This is a long string for testing.";
    char long_dest[50] = "";
    EXPECT_EQ(nva_memcpy(long_dest, long_src, 35), long_dest);
    EXPECT_STREQ(long_dest, "This is a long string for testing.");

    const int arr_src[] = {1};
    int arr_dest[1];
    EXPECT_EQ(nva_memcpy(arr_dest, arr_src, sizeof arr_src), arr_dest);
    EXPECT_EQ(arr_dest[0], 1);

    const short val = 32556;
    short dst;
    EXPECT_EQ(nva_memcpy(&dst, &val, sizeof(val)), &dst);
    EXPECT_EQ(dst, val);

    const signed char uchar_src[] = {1, 35, 31, 127, 0, 83, 11, 42, 42, 2, 7, 21, 53, 12, 0, 0, 0, 0, 0, 0, 12, 56};
    signed char uchar_dest[sizeof(uchar_src)];
    EXPECT_EQ(nva_memcpy(uchar_dest, uchar_src, sizeof(uchar_src)), uchar_dest);
    bool is_equal = true;
    for (size_t i = 0; i < sizeof(uchar_src); ++i) {
        if (uchar_dest[i] != uchar_src[i]) {
            is_equal = false;
            break;
        }
    }
    EXPECT_TRUE(is_equal) << "The uchar arrays are not equal after memcpy.";
}

TEST(StringTest, nva_memmove)
{
    std::array<int, 10> arr{1, 2, 3, 4, 5, 0, 0, 0, 0, 0};
    EXPECT_EQ(nva_memmove(&arr[3], &arr[0], 5 * sizeof(int)), &arr[3]);

    std::array<int, 10> arr_1{1, 2, 3, 4, 5, 0, 0, 0, 0, 0};
    std::memmove(&arr_1[3], &arr_1[0], 5 * sizeof(int));
    EXPECT_TRUE(arr == arr_1) << "arr_1 = " << arr_1 << "; arr = " << arr;

    EXPECT_EQ(nva_memmove(&arr[0], &arr[3], 5 * sizeof(int)), &arr[0]);
    std::memmove(&arr_1[0], &arr_1[3], 5 * sizeof(int));
    EXPECT_TRUE(arr == arr_1) << "arr_1 = " << arr_1 << "; arr = " << arr;

    EXPECT_EQ(nva_memmove(&arr[6], &arr[0], 4 * sizeof(int)), &arr[6]);
    std::memmove(&arr_1[6], &arr_1[0], 4 * sizeof(int));
    EXPECT_TRUE(arr == arr_1) << "arr_1 = " << arr_1 << "; arr = " << arr;
}

TEST(StringTest, nva_atoi)
{
    unsigned int width;

    EXPECT_EQ(nva_atoi("123", &width), 123);
    EXPECT_EQ(width, 3);
    EXPECT_EQ(nva_atoi("-123", &width), -123);
    EXPECT_EQ(width, 4);
    EXPECT_EQ(nva_atoi("0123", &width), 123);
    EXPECT_EQ(width, 4);
    EXPECT_EQ(nva_atoi("-0123", &width), -123);
    EXPECT_EQ(width, 5);
    EXPECT_EQ(nva_atoi("0", &width), 0);
    EXPECT_EQ(width, 1);
    EXPECT_EQ(nva_atoi("-0", &width), 0);
    EXPECT_EQ(width, 2);

    EXPECT_EQ(nva_atoi("", &width), 0);
    EXPECT_EQ(width, 0);
    EXPECT_EQ(nva_atoi("1-0", &width), 1);
    EXPECT_EQ(width, 1);
    EXPECT_EQ(nva_atoi("65431-0", &width), 65431);
    EXPECT_EQ(width, 5);
    EXPECT_EQ(nva_atoi("--0", &width), 0);
    EXPECT_EQ(width, 0);
    EXPECT_EQ(nva_atoi("a", &width), 0);
    EXPECT_EQ(width, 0);
}

TEST(StringTest, nva_itoa)
{
#define NVA_TEST_ITOA_CHECK(value, dest, attr, width, expect_str)               \
    do {                                                                        \
        EXPECT_STREQ(nva_itoa((value), (dest), (attr), (width)), (expect_str)); \
        EXPECT_EQ((*(width)), std::string_view{expect_str}.length());           \
    } while (0)

    char buffer[50];

    for (unsigned char i = 2; i <= 16; i++) {
        nva_NumToStringAttr attr{.base = i, .upper_case = NVA_TRUE};
        unsigned int width;

        NVA_TEST_ITOA_CHECK(0, buffer, &attr, &width, "0");

        attr.upper_case = NVA_FALSE;
        NVA_TEST_ITOA_CHECK(0, buffer, &attr, &width, "0");
    }

    nva_NumToStringAttr attr{.base = 10, .upper_case = NVA_TRUE};
    unsigned int width;
    EXPECT_STREQ(nva_itoa(1, buffer, &attr, &width), "1");
    EXPECT_EQ(width, 1);
    EXPECT_STREQ(nva_itoa(-1, buffer, &attr, &width), "-1");
    EXPECT_EQ(width, 2);
    EXPECT_STREQ(nva_itoa(12345, buffer, &attr, &width), "12345");
    EXPECT_EQ(width, 5);
    EXPECT_STREQ(nva_itoa(-12345, buffer, &attr, &width), "-12345");
    EXPECT_EQ(width, 6);
    EXPECT_STREQ(nva_itoa(2147483647, buffer, &attr, &width), "2147483647");
    EXPECT_EQ(width, 10);
    EXPECT_STREQ(nva_itoa(-2147483648, buffer, &attr, &width), "-2147483648");
    EXPECT_EQ(width, 11);

    attr = {.base = 16, .upper_case = NVA_FALSE};
    EXPECT_STREQ(nva_itoa(0xF2, buffer, &attr, &width), "f2");
    EXPECT_EQ(width, 2);
    EXPECT_STREQ(nva_itoa(-0xdF, buffer, &attr, &width), "-df");
    EXPECT_EQ(width, 3);
    attr.upper_case = NVA_TRUE;
    EXPECT_STREQ(nva_itoa(0xF2, buffer, &attr, &width), "F2");
    EXPECT_EQ(width, 2);
    EXPECT_STREQ(nva_itoa(-0xdF, buffer, &attr, &width), "-DF");
    EXPECT_EQ(width, 3);
    EXPECT_STREQ(nva_itoa(2147483647, buffer, &attr, &width), "7FFFFFFF");
    EXPECT_EQ(width, 8);
    EXPECT_STREQ(nva_itoa(-2147483648, buffer, &attr, &width), "-80000000");
    EXPECT_EQ(width, 9);
    attr.upper_case = NVA_FALSE;
    EXPECT_STREQ(nva_itoa(2147483647, buffer, &attr, &width), "7fffffff");
    EXPECT_EQ(width, 8);
    EXPECT_STREQ(nva_itoa(-2147483648, buffer, &attr, &width), "-80000000");
    EXPECT_EQ(width, 9);

    attr = {.base = 2, .upper_case = NVA_FALSE};
    EXPECT_STREQ(nva_itoa(1, buffer, &attr, &width), "1");
    EXPECT_EQ(width, 1);
    EXPECT_STREQ(nva_itoa(-1, buffer, &attr, &width), "-1");
    EXPECT_EQ(width, 2);
    EXPECT_STREQ(nva_itoa(12345, buffer, &attr, &width), "11000000111001");
    EXPECT_EQ(width, 14);
    EXPECT_STREQ(nva_itoa(-12345, buffer, &attr, &width), "-11000000111001");
    EXPECT_EQ(width, 15);
    EXPECT_STREQ(nva_itoa(2147483647, buffer, &attr, &width), "1111111111111111111111111111111");
    EXPECT_EQ(width, 31);
    EXPECT_STREQ(nva_itoa(-2147483648, buffer, &attr, &width), "-10000000000000000000000000000000");
    EXPECT_EQ(width, std::string_view{"-10000000000000000000000000000000"}.length());
}

TEST(StringTest, nva_uitoa)
{
    char buffer[50];

    // 2~16 进制下的 0
    for (unsigned char i = 2; i <= 16; i++) {
        nva_NumToStringAttr attr{.base = i, .upper_case = NVA_TRUE};
        unsigned int width;
        EXPECT_STREQ(nva_itoa(0U, buffer, &attr, &width), "0");
        EXPECT_EQ(width, 1);

        attr.upper_case = NVA_FALSE;
        EXPECT_STREQ(nva_itoa(0U, buffer, &attr, &width), "0");
        EXPECT_EQ(width, 1);
    }

    nva_NumToStringAttr attr{.base = 10, .upper_case = NVA_FALSE};
    unsigned int width;

    // 10进制常规数
    EXPECT_STREQ(nva_uitoa(1u, buffer, &attr, &width), "1");
    EXPECT_EQ(width, 1);
    EXPECT_STREQ(nva_uitoa(12345u, buffer, &attr, &width), "12345");
    EXPECT_EQ(width, 5);
    EXPECT_STREQ(nva_uitoa(4294967295u, buffer, &attr, &width), "4294967295");
    EXPECT_EQ(width, 10);

    // 16进制大小写
    attr = {.base = 16, .upper_case = NVA_FALSE};
    EXPECT_STREQ(nva_uitoa(0xF2u, buffer, &attr, &width), "f2");
    EXPECT_EQ(width, 2);
    EXPECT_STREQ(nva_uitoa(0xDFu, buffer, &attr, &width), "df");
    EXPECT_EQ(width, 2);
    attr.upper_case = NVA_TRUE;
    EXPECT_STREQ(nva_uitoa(0xF2u, buffer, &attr, &width), "F2");
    EXPECT_EQ(width, 2);
    EXPECT_STREQ(nva_uitoa(0xDFu, buffer, &attr, &width), "DF");
    EXPECT_EQ(width, 2);
    EXPECT_STREQ(nva_uitoa(4294967295u, buffer, &attr, &width), "FFFFFFFF");
    EXPECT_EQ(width, 8);
    attr.upper_case = NVA_FALSE;
    EXPECT_STREQ(nva_uitoa(4294967295u, buffer, &attr, &width), "ffffffff");
    EXPECT_EQ(width, 8);

    // 2进制
    attr = {.base = 2, .upper_case = NVA_FALSE};
    EXPECT_STREQ(nva_uitoa(1u, buffer, &attr, &width), "1");
    EXPECT_EQ(width, 1);
    EXPECT_STREQ(nva_uitoa(12345u, buffer, &attr, &width), "11000000111001");
    EXPECT_EQ(width, 14);
    constexpr const auto uint32_max_str = "11111111111111111111111111111111";
    EXPECT_STREQ(nva_uitoa(4294967295u, buffer, &attr, &width), uint32_max_str);
    EXPECT_EQ(width, std::string_view{uint32_max_str}.length());

    // 8进制
    attr = {.base = 8, .upper_case = NVA_FALSE};
    EXPECT_STREQ(nva_uitoa(12345u, buffer, &attr, &width), "30071");
    EXPECT_EQ(width, std::string_view{"30071"}.length());
    EXPECT_STREQ(nva_uitoa(4294967295u, buffer, &attr, &width), "37777777777");
    EXPECT_EQ(width, std::string_view{"37777777777"}.length());

    // 边界值
    attr = {.base = 10, .upper_case = false};
    EXPECT_STREQ(nva_uitoa(UINT_MAX, buffer, &attr, &width), "4294967295");
    EXPECT_EQ(width, std::string_view{"4294967295"}.length());
    attr = {.base = 16, .upper_case = NVA_TRUE};
    EXPECT_STREQ(nva_uitoa(UINT_MAX, buffer, &attr, &width), "FFFFFFFF");
    EXPECT_EQ(width, std::string_view{"FFFFFFFF"}.length());
    attr = {.base = 2, .upper_case = false};
    EXPECT_STREQ(nva_uitoa(UINT_MAX, buffer, &attr, &width), "11111111111111111111111111111111");
    EXPECT_EQ(width, std::string_view{"11111111111111111111111111111111"}.length());
}

TEST(StringTest, nva_fptoa_type_f)
{
    char buffer[50];

#define NVA_FPTOA_TYPE_F_TEST(func, expect_str)                   \
    do {                                                          \
        EXPECT_EQ((func), std::string_view(expect_str).length()); \
        EXPECT_STREQ(buffer, (expect_str));                       \
    } while (0)

    nva_FloatPointToStrAttr attr{.base = 10,
                                 .precision = 6,
                                 .flag = {.keep_decimal_point = 0, .upper_case = 0, .type = NVA_FP_TO_STR_TYPE_F}};

    NVA_FPTOA_TYPE_F_TEST(nva_fptoa(0.0, buffer, &attr), "0.000000");
    NVA_FPTOA_TYPE_F_TEST(nva_fptoa(-0.0, buffer, &attr), NEGATIVE_ZERO_PREFIX "0.000000");
    NVA_FPTOA_TYPE_F_TEST(nva_fptoa(123.456, buffer, &attr), "123.456000");
    NVA_FPTOA_TYPE_F_TEST(nva_fptoa(-123.456, buffer, &attr), "-123.456000");
    NVA_FPTOA_TYPE_F_TEST(nva_fptoa(1.23456e10, buffer, &attr), "12345600000.000000");
    NVA_FPTOA_TYPE_F_TEST(nva_fptoa(-1.23456e10, buffer, &attr), "-12345600000.000000");
    NVA_FPTOA_TYPE_F_TEST(nva_fptoa(3.14159265358979323846, buffer, &attr), "3.141593");
    NVA_FPTOA_TYPE_F_TEST(nva_fptoa(-3.14159265358979323846, buffer, &attr), "-3.141593");
    attr.precision = 3;
    NVA_FPTOA_TYPE_F_TEST(nva_fptoa(765432123.456, buffer, &attr), "765432123.456");
    NVA_FPTOA_TYPE_F_TEST(nva_fptoa(-765432123.456, buffer, &attr), "-765432123.456");
    attr.precision = 2;
    NVA_FPTOA_TYPE_F_TEST(nva_fptoa(2.61734536, buffer, &attr), "2.62");
    NVA_FPTOA_TYPE_F_TEST(nva_fptoa(-2.61734536, buffer, &attr), "-2.62");
    attr.precision = 5;
    NVA_FPTOA_TYPE_F_TEST(nva_fptoa(2.61734536, buffer, &attr), "2.61734");
    NVA_FPTOA_TYPE_F_TEST(nva_fptoa(-2.61734536, buffer, &attr), "-2.61734");
    NVA_FPTOA_TYPE_F_TEST(nva_fptoa(2.61737536, buffer, &attr), "2.61738");
    NVA_FPTOA_TYPE_F_TEST(nva_fptoa(-2.61737536, buffer, &attr), "-2.61738");

    attr.precision = 6;
    NVA_FPTOA_TYPE_F_TEST(nva_fptoa(0.0f, buffer, &attr), "0.000000");
    NVA_FPTOA_TYPE_F_TEST(nva_fptoa(-0.0f, buffer, &attr), NEGATIVE_ZERO_PREFIX "0.000000");
    attr.precision = 4;
    NVA_FPTOA_TYPE_F_TEST(nva_fptoa(123.456f, buffer, &attr), "123.4560");
    NVA_FPTOA_TYPE_F_TEST(nva_fptoa(-123.456f, buffer, &attr), "-123.4560");
    attr.precision = 6;
    NVA_FPTOA_TYPE_F_TEST(nva_fptoa(1.23456e10f, buffer, &attr), "12345600000.000000");
    NVA_FPTOA_TYPE_F_TEST(nva_fptoa(-1.23456e10f, buffer, &attr), "-12345600000.000000");
    NVA_FPTOA_TYPE_F_TEST(nva_fptoa(3.14159265358979323846f, buffer, &attr), "3.141593");
    NVA_FPTOA_TYPE_F_TEST(nva_fptoa(-3.14159265358979323846f, buffer, &attr), "-3.141593");
    attr.precision = 3;
    NVA_FPTOA_TYPE_F_TEST(nva_fptoa(2123.456f, buffer, &attr), "2123.456");
    NVA_FPTOA_TYPE_F_TEST(nva_fptoa(-2123.456f, buffer, &attr), "-2123.456");
    attr.precision = 2;
    NVA_FPTOA_TYPE_F_TEST(nva_fptoa(2.61734536f, buffer, &attr), "2.62");
    NVA_FPTOA_TYPE_F_TEST(nva_fptoa(-2.61734536f, buffer, &attr), "-2.62");
    attr.precision = 5;
    NVA_FPTOA_TYPE_F_TEST(nva_fptoa(2.61734536f, buffer, &attr), "2.61734");
    NVA_FPTOA_TYPE_F_TEST(nva_fptoa(-2.61734536f, buffer, &attr), "-2.61734");
    NVA_FPTOA_TYPE_F_TEST(nva_fptoa(2.61737536f, buffer, &attr), "2.61738");
    NVA_FPTOA_TYPE_F_TEST(nva_fptoa(-2.61737536f, buffer, &attr), "-2.61738");

    // 测试精度为0的情况
    attr.precision = 0;
    NVA_FPTOA_TYPE_F_TEST(nva_fptoa(123.456, buffer, &attr), "123");
    NVA_FPTOA_TYPE_F_TEST(nva_fptoa(-123.456, buffer, &attr), "-123");
    NVA_FPTOA_TYPE_F_TEST(nva_fptoa(0.0, buffer, &attr), "0");
    NVA_FPTOA_TYPE_F_TEST(nva_fptoa(-0.0, buffer, &attr), NEGATIVE_ZERO_PREFIX "0");
    NVA_FPTOA_TYPE_F_TEST(nva_fptoa(123.656, buffer, &attr), "124");
    NVA_FPTOA_TYPE_F_TEST(nva_fptoa(123.556, buffer, &attr), "124");
    NVA_FPTOA_TYPE_F_TEST(nva_fptoa(124.556, buffer, &attr), "124");
    NVA_FPTOA_TYPE_F_TEST(nva_fptoa(-124.556, buffer, &attr), "-124");
    NVA_FPTOA_TYPE_F_TEST(nva_fptoa(-123.656, buffer, &attr), "-124");
    NVA_FPTOA_TYPE_F_TEST(nva_fptoa(-123.556, buffer, &attr), "-124");

    NVA_FPTOA_TYPE_F_TEST(nva_fptoa(123.456f, buffer, &attr), "123");
    NVA_FPTOA_TYPE_F_TEST(nva_fptoa(-123.456f, buffer, &attr), "-123");
    NVA_FPTOA_TYPE_F_TEST(nva_fptoa(0.0f, buffer, &attr), "0");
    NVA_FPTOA_TYPE_F_TEST(nva_fptoa(-0.0f, buffer, &attr), NEGATIVE_ZERO_PREFIX "0");
    NVA_FPTOA_TYPE_F_TEST(nva_fptoa(123.656f, buffer, &attr), "124");
    NVA_FPTOA_TYPE_F_TEST(nva_fptoa(123.556f, buffer, &attr), "124");
    NVA_FPTOA_TYPE_F_TEST(nva_fptoa(124.556f, buffer, &attr), "124");
    NVA_FPTOA_TYPE_F_TEST(nva_fptoa(-124.556f, buffer, &attr), "-124");
    NVA_FPTOA_TYPE_F_TEST(nva_fptoa(-123.656f, buffer, &attr), "-124");
    NVA_FPTOA_TYPE_F_TEST(nva_fptoa(-123.556f, buffer, &attr), "-124");

    nva_FloatPointToStrAttr attr2{.base = 10,
                                  .precision = 0,
                                  .flag = {.keep_decimal_point = 1U, .upper_case = 0, .type = NVA_FP_TO_STR_TYPE_F}};
    NVA_FPTOA_TYPE_F_TEST(nva_fptoa(0.0f, buffer, &attr2), "0.");
    NVA_FPTOA_TYPE_F_TEST(nva_fptoa(-0.0f, buffer, &attr2), NEGATIVE_ZERO_PREFIX "0.");
    NVA_FPTOA_TYPE_F_TEST(nva_fptoa(123.456, buffer, &attr2), "123.");
    NVA_FPTOA_TYPE_F_TEST(nva_fptoa(-123.456, buffer, &attr2), "-123.");
    NVA_FPTOA_TYPE_F_TEST(nva_fptoa(123.656, buffer, &attr2), "124.");
    NVA_FPTOA_TYPE_F_TEST(nva_fptoa(-123.656, buffer, &attr2), "-124.");
    NVA_FPTOA_TYPE_F_TEST(nva_fptoa(123.556, buffer, &attr2), "124.");
    NVA_FPTOA_TYPE_F_TEST(nva_fptoa(-123.556, buffer, &attr2), "-124.");
    NVA_FPTOA_TYPE_F_TEST(nva_fptoa(124.556, buffer, &attr2), "124.");
    NVA_FPTOA_TYPE_F_TEST(nva_fptoa(-124.556, buffer, &attr2), "-124.");

    // 测试精度为1的情况
    attr.precision = 1;
    NVA_FPTOA_TYPE_F_TEST(nva_fptoa(123.456, buffer, &attr), "123.4");
    NVA_FPTOA_TYPE_F_TEST(nva_fptoa(-123.456, buffer, &attr), "-123.4");
    NVA_FPTOA_TYPE_F_TEST(nva_fptoa(123.556, buffer, &attr), "123.6");
    NVA_FPTOA_TYPE_F_TEST(nva_fptoa(-123.556, buffer, &attr), "-123.6");
    NVA_FPTOA_TYPE_F_TEST(nva_fptoa(0.0, buffer, &attr), "0.0");
    NVA_FPTOA_TYPE_F_TEST(nva_fptoa(-0.0, buffer, &attr), NEGATIVE_ZERO_PREFIX "0.0");

    NVA_FPTOA_TYPE_F_TEST(nva_fptoa(123.456f, buffer, &attr), "123.4");
    NVA_FPTOA_TYPE_F_TEST(nva_fptoa(-123.456f, buffer, &attr), "-123.4");
    NVA_FPTOA_TYPE_F_TEST(nva_fptoa(123.556f, buffer, &attr), "123.6");
    NVA_FPTOA_TYPE_F_TEST(nva_fptoa(-123.556f, buffer, &attr), "-123.6");
    NVA_FPTOA_TYPE_F_TEST(nva_fptoa(0.0f, buffer, &attr), "0.0");
    NVA_FPTOA_TYPE_F_TEST(nva_fptoa(-0.0f, buffer, &attr), NEGATIVE_ZERO_PREFIX "0.0");

    // 测试 inf 与 nan
    constexpr auto inf_v = std::numeric_limits<double>::infinity();
    constexpr auto nan_v = std::numeric_limits<double>::quiet_NaN();

    attr.flag.upper_case = 0;
    NVA_FPTOA_TYPE_F_TEST(nva_fptoa(inf_v, buffer, &attr), "inf");
    NVA_FPTOA_TYPE_F_TEST(nva_fptoa(-inf_v, buffer, &attr), "-inf");
    NVA_FPTOA_TYPE_F_TEST(nva_fptoa(nan_v, buffer, &attr), "nan");
    NVA_FPTOA_TYPE_F_TEST(nva_fptoa(-nan_v, buffer, &attr), "-nan");

    attr.flag.upper_case = 1;
    NVA_FPTOA_TYPE_F_TEST(nva_fptoa(inf_v, buffer, &attr), "INF");
    NVA_FPTOA_TYPE_F_TEST(nva_fptoa(-inf_v, buffer, &attr), "-INF");
    NVA_FPTOA_TYPE_F_TEST(nva_fptoa(nan_v, buffer, &attr), "NAN");
    NVA_FPTOA_TYPE_F_TEST(nva_fptoa(-nan_v, buffer, &attr), "-NAN");
}

TEST(StringTest, nva_fptoa_type_g)
{
    auto fptoa = [](const double value, char* const dest, const nva_FloatPointToStrAttr& attr) {
        return nva_fptoa(value, dest, &attr);
    };

#define NVA_TEST_FPTOA_CHECK(value, dest, attr, expect_str)                               \
    do {                                                                                  \
        EXPECT_EQ(fptoa((value), (dest), (attr)), std::string_view{expect_str}.length()); \
        EXPECT_STREQ((dest), (expect_str));                                               \
    } while (0)

    char buffer[50];
    nva_FloatPointToStrAttr attr{.base = 10,
                                 .precision = 6,
                                 .flag = {.keep_decimal_point = 0, .upper_case = 0, .type = NVA_FP_TO_STR_TYPE_G}};

    NVA_TEST_FPTOA_CHECK(0.0, buffer, attr, "0");
    NVA_TEST_FPTOA_CHECK(123.456789, buffer, attr, "123.456789");
    NVA_TEST_FPTOA_CHECK(-123.456789, buffer, attr, "-123.456789");
    NVA_TEST_FPTOA_CHECK(1.23456789e10, buffer, attr, "12345678900");
    NVA_TEST_FPTOA_CHECK(-1.23456789e10, buffer, attr, "-12345678900");
    NVA_TEST_FPTOA_CHECK(123.456000, buffer, attr, "123.456");
    NVA_TEST_FPTOA_CHECK(-123.456, buffer, attr, "-123.456");

    attr.flag.upper_case = NVA_TRUE;
    NVA_TEST_FPTOA_CHECK(1.23456789e10, buffer, attr, "12345678900");
    NVA_TEST_FPTOA_CHECK(-1.23456789e10, buffer, attr, "-12345678900");

    attr.precision = 2;
    NVA_TEST_FPTOA_CHECK(123.456789, buffer, attr, "123.46");
    NVA_TEST_FPTOA_CHECK(-123.456789, buffer, attr, "-123.46");

    nva_FloatPointToStrAttr attr2{.base = 10,
                                  .precision = 0,
                                  .flag = {.keep_decimal_point = 1U, .upper_case = 0, .type = NVA_FP_TO_STR_TYPE_G}};
    NVA_FPTOA_TYPE_F_TEST(nva_fptoa(0.0f, buffer, &attr2), "0.");
    NVA_FPTOA_TYPE_F_TEST(nva_fptoa(-0.0f, buffer, &attr2), NEGATIVE_ZERO_PREFIX "0.");
    NVA_FPTOA_TYPE_F_TEST(nva_fptoa(123.456, buffer, &attr2), "123.");
    NVA_FPTOA_TYPE_F_TEST(nva_fptoa(-123.456, buffer, &attr2), "-123.");
    NVA_FPTOA_TYPE_F_TEST(nva_fptoa(123.656, buffer, &attr2), "124.");
    NVA_FPTOA_TYPE_F_TEST(nva_fptoa(-123.656, buffer, &attr2), "-124.");
    NVA_FPTOA_TYPE_F_TEST(nva_fptoa(123.556, buffer, &attr2), "124.");
    NVA_FPTOA_TYPE_F_TEST(nva_fptoa(-123.556, buffer, &attr2), "-124.");
    NVA_FPTOA_TYPE_F_TEST(nva_fptoa(124.556, buffer, &attr2), "124.");
    NVA_FPTOA_TYPE_F_TEST(nva_fptoa(-124.556, buffer, &attr2), "-124.");

    // 测试 inf 与 nan
    constexpr auto inf_v = std::numeric_limits<double>::infinity();
    constexpr auto nan_v = std::numeric_limits<double>::quiet_NaN();

    attr.flag.upper_case = 0;
    NVA_FPTOA_TYPE_F_TEST(nva_fptoa(inf_v, buffer, &attr), "inf");
    NVA_FPTOA_TYPE_F_TEST(nva_fptoa(-inf_v, buffer, &attr), "-inf");
    NVA_FPTOA_TYPE_F_TEST(nva_fptoa(nan_v, buffer, &attr), "nan");
    NVA_FPTOA_TYPE_F_TEST(nva_fptoa(-nan_v, buffer, &attr), "-nan");

    attr.flag.upper_case = 1;
    NVA_FPTOA_TYPE_F_TEST(nva_fptoa(inf_v, buffer, &attr), "INF");
    NVA_FPTOA_TYPE_F_TEST(nva_fptoa(-inf_v, buffer, &attr), "-INF");
    NVA_FPTOA_TYPE_F_TEST(nva_fptoa(nan_v, buffer, &attr), "NAN");
    NVA_FPTOA_TYPE_F_TEST(nva_fptoa(-nan_v, buffer, &attr), "-NAN");
}
