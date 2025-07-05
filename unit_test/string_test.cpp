/**
 * @file string_test.cpp
 * @author DuYicheng
 * @date 2025-06-28
 * @brief nva 字符串测试
 */

#include "gtest/gtest.h"

#include "nva/string.h"

#include <climits>

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

TEST(StringTest, nva_atoi)
{
    EXPECT_EQ(nva_atoi("123"), 123);
    EXPECT_EQ(nva_atoi("-123"), -123);
    EXPECT_EQ(nva_atoi("0123"), 123);
    EXPECT_EQ(nva_atoi("-0123"), -123);
    EXPECT_EQ(nva_atoi("0"), 0);
    EXPECT_EQ(nva_atoi("-0"), 0);

    EXPECT_EQ(nva_atoi(""), 0);
    EXPECT_EQ(nva_atoi("1-0"), 1);
    EXPECT_EQ(nva_atoi("65431-0"), 65431);
    EXPECT_EQ(nva_atoi("--0"), 0);
    EXPECT_EQ(nva_atoi("a"), 0);
}

TEST(StringTest, nva_itoa)
{
    char buffer[50];

    for (unsigned char i = 2; i <= 16; i++) {
        EXPECT_STREQ(nva_itoa(0, buffer, i, NVA_FALSE), "0");
        EXPECT_STREQ(nva_itoa(0, buffer, i, NVA_TRUE), "0");
    }

    EXPECT_STREQ(nva_itoa(1, buffer, 10, NVA_FALSE), "1");
    EXPECT_STREQ(nva_itoa(-1, buffer, 10, NVA_FALSE), "-1");
    EXPECT_STREQ(nva_itoa(12345, buffer, 10, NVA_FALSE), "12345");
    EXPECT_STREQ(nva_itoa(-12345, buffer, 10, NVA_FALSE), "-12345");
    EXPECT_STREQ(nva_itoa(2147483647, buffer, 10, NVA_FALSE), "2147483647");
    EXPECT_STREQ(nva_itoa(-2147483648, buffer, 10, NVA_FALSE), "-2147483648");

    EXPECT_STREQ(nva_itoa(0xF2, buffer, 16, NVA_FALSE), "f2");
    EXPECT_STREQ(nva_itoa(-0xdF, buffer, 16, NVA_FALSE), "-df");
    EXPECT_STREQ(nva_itoa(0xF2, buffer, 16, NVA_TRUE), "F2");
    EXPECT_STREQ(nva_itoa(-0xdF, buffer, 16, NVA_TRUE), "-DF");
    EXPECT_STREQ(nva_itoa(2147483647, buffer, 16, NVA_TRUE), "7FFFFFFF");
    EXPECT_STREQ(nva_itoa(-2147483648, buffer, 16, NVA_TRUE), "-80000000");
    EXPECT_STREQ(nva_itoa(2147483647, buffer, 16, NVA_FALSE), "7fffffff");
    EXPECT_STREQ(nva_itoa(-2147483648, buffer, 16, NVA_FALSE), "-80000000");

    EXPECT_STREQ(nva_itoa(1, buffer, 2, NVA_FALSE), "1");
    EXPECT_STREQ(nva_itoa(-1, buffer, 2, NVA_FALSE), "-1");
    EXPECT_STREQ(nva_itoa(12345, buffer, 2, NVA_FALSE), "11000000111001");
    EXPECT_STREQ(nva_itoa(-12345, buffer, 2, NVA_FALSE), "-11000000111001");
    EXPECT_STREQ(nva_itoa(2147483647, buffer, 2, NVA_FALSE), "1111111111111111111111111111111");
    EXPECT_STREQ(nva_itoa(-2147483648, buffer, 2, NVA_FALSE), "-10000000000000000000000000000000");
}

TEST(StringTest, nva_uitoa)
{
    char buffer[50];

    // 2~16 进制下的 0
    for (unsigned char base = 2; base <= 16; base++) {
        EXPECT_STREQ(nva_uitoa(0u, buffer, base, NVA_FALSE), "0");
        EXPECT_STREQ(nva_uitoa(0u, buffer, base, NVA_TRUE), "0");
    }

    // 10进制常规数
    EXPECT_STREQ(nva_uitoa(1u, buffer, 10, NVA_FALSE), "1");
    EXPECT_STREQ(nva_uitoa(12345u, buffer, 10, NVA_FALSE), "12345");
    EXPECT_STREQ(nva_uitoa(4294967295u, buffer, 10, NVA_FALSE), "4294967295"); // UINT32_MAX

    // 16进制大小写
    EXPECT_STREQ(nva_uitoa(0xF2u, buffer, 16, NVA_FALSE), "f2");
    EXPECT_STREQ(nva_uitoa(0xDFu, buffer, 16, NVA_FALSE), "df");
    EXPECT_STREQ(nva_uitoa(0xF2u, buffer, 16, NVA_TRUE), "F2");
    EXPECT_STREQ(nva_uitoa(0xDFu, buffer, 16, NVA_TRUE), "DF");
    EXPECT_STREQ(nva_uitoa(4294967295u, buffer, 16, NVA_TRUE), "FFFFFFFF");
    EXPECT_STREQ(nva_uitoa(4294967295u, buffer, 16, NVA_FALSE), "ffffffff");

    // 2进制
    EXPECT_STREQ(nva_uitoa(1u, buffer, 2, NVA_FALSE), "1");
    EXPECT_STREQ(nva_uitoa(12345u, buffer, 2, NVA_FALSE), "11000000111001");
    EXPECT_STREQ(nva_uitoa(4294967295u, buffer, 2, NVA_FALSE), "11111111111111111111111111111111");

    // 8进制
    EXPECT_STREQ(nva_uitoa(12345u, buffer, 8, NVA_FALSE), "30071");
    EXPECT_STREQ(nva_uitoa(4294967295u, buffer, 8, NVA_FALSE), "37777777777");

    // 边界值
    EXPECT_STREQ(nva_uitoa(UINT_MAX, buffer, 10, NVA_FALSE), "4294967295");
    EXPECT_STREQ(nva_uitoa(UINT_MAX, buffer, 16, NVA_TRUE), "FFFFFFFF");
    EXPECT_STREQ(nva_uitoa(UINT_MAX, buffer, 2, NVA_FALSE), "11111111111111111111111111111111");
}

TEST(StringTest, nva_gcvt)
{
    char buffer[50];

    // 测试 gcvt 函数
    EXPECT_STREQ(nva_gcvt(0.0, 6, buffer), "0.000000");
    EXPECT_STREQ(nva_gcvt(-0.0, 6, buffer), "0.000000");
    EXPECT_STREQ(nva_gcvt(123.456, 6, buffer), "123.456000");
    EXPECT_STREQ(nva_gcvt(-123.456, 6, buffer), "-123.456000");
    EXPECT_STREQ(nva_gcvt(1.23456e10, 6, buffer), "12345600000.000000");
    EXPECT_STREQ(nva_gcvt(-1.23456e10, 6, buffer), "-12345600000.000000");
    EXPECT_STREQ(nva_gcvt(3.14159265358979323846, 6, buffer), "3.141593");
    EXPECT_STREQ(nva_gcvt(-3.14159265358979323846, 6, buffer), "-3.141593");
    EXPECT_STREQ(nva_gcvt(765432123.456, 3, buffer), "765432123.456");
    EXPECT_STREQ(nva_gcvt(-765432123.456, 3, buffer), "-765432123.456");
    EXPECT_STREQ(nva_gcvt(2.61734536, 2, buffer), "2.62");
    EXPECT_STREQ(nva_gcvt(-2.61734536, 2, buffer), "-2.62");
    EXPECT_STREQ(nva_gcvt(2.61734536, 5, buffer), "2.61734");
    EXPECT_STREQ(nva_gcvt(-2.61734536, 5, buffer), "-2.61734");
    EXPECT_STREQ(nva_gcvt(2.61737536, 5, buffer), "2.61738");
    EXPECT_STREQ(nva_gcvt(-2.61737536, 5, buffer), "-2.61738");

    EXPECT_STREQ(nva_gcvt(0.0f, 6, buffer), "0.000000");
    EXPECT_STREQ(nva_gcvt(-0.0f, 6, buffer), "0.000000");
    EXPECT_STREQ(nva_gcvt(123.456f, 4, buffer), "123.4560");
    EXPECT_STREQ(nva_gcvt(-123.456f, 4, buffer), "-123.4560");
    EXPECT_STREQ(nva_gcvt(1.23456e10f, 6, buffer), "12345600000.000000");
    EXPECT_STREQ(nva_gcvt(-1.23456e10f, 6, buffer), "-12345600000.000000");
    EXPECT_STREQ(nva_gcvt(3.14159265358979323846f, 6, buffer), "3.141593");
    EXPECT_STREQ(nva_gcvt(-3.14159265358979323846f, 6, buffer), "-3.141593");
    EXPECT_STREQ(nva_gcvt(2123.456f, 3, buffer), "2123.456");
    EXPECT_STREQ(nva_gcvt(-2123.456f, 3, buffer), "-2123.456");
    EXPECT_STREQ(nva_gcvt(2.61734536f, 2, buffer), "2.62");
    EXPECT_STREQ(nva_gcvt(-2.61734536f, 2, buffer), "-2.62");
    EXPECT_STREQ(nva_gcvt(2.61734536f, 5, buffer), "2.61734");
    EXPECT_STREQ(nva_gcvt(-2.61734536f, 5, buffer), "-2.61734");
    EXPECT_STREQ(nva_gcvt(2.61737536f, 5, buffer), "2.61738");
    EXPECT_STREQ(nva_gcvt(-2.61737536f, 5, buffer), "-2.61738");

    // 测试精度为0的情况
    EXPECT_STREQ(nva_gcvt(123.456, 0, buffer), "123");
    EXPECT_STREQ(nva_gcvt(-123.456, 0, buffer), "-123");
    EXPECT_STREQ(nva_gcvt(0.0, 0, buffer), "0");
    EXPECT_STREQ(nva_gcvt(-0.0, 0, buffer), "0");
    EXPECT_STREQ(nva_gcvt(123.656, 0, buffer), "124");
    EXPECT_STREQ(nva_gcvt(123.556, 0, buffer), "124");
    EXPECT_STREQ(nva_gcvt(124.556, 0, buffer), "124");
    EXPECT_STREQ(nva_gcvt(-124.556, 0, buffer), "-124");
    EXPECT_STREQ(nva_gcvt(-123.656, 0, buffer), "-124");
    EXPECT_STREQ(nva_gcvt(-123.556, 0, buffer), "-124");

    EXPECT_STREQ(nva_gcvt(123.456f, 0, buffer), "123");
    EXPECT_STREQ(nva_gcvt(-123.456f, 0, buffer), "-123");
    EXPECT_STREQ(nva_gcvt(0.0f, 0, buffer), "0");
    EXPECT_STREQ(nva_gcvt(-0.0f, 0, buffer), "0");
    EXPECT_STREQ(nva_gcvt(123.656f, 0, buffer), "124");
    EXPECT_STREQ(nva_gcvt(123.556f, 0, buffer), "124");
    EXPECT_STREQ(nva_gcvt(124.556f, 0, buffer), "124");
    EXPECT_STREQ(nva_gcvt(-124.556f, 0, buffer), "-124");
    EXPECT_STREQ(nva_gcvt(-123.656f, 0, buffer), "-124");
    EXPECT_STREQ(nva_gcvt(-123.556f, 0, buffer), "-124");

    // 测试精度为1的情况
    EXPECT_STREQ(nva_gcvt(123.456, 1, buffer), "123.4");
    EXPECT_STREQ(nva_gcvt(-123.456, 1, buffer), "-123.4");
    EXPECT_STREQ(nva_gcvt(123.556, 1, buffer), "123.6");
    EXPECT_STREQ(nva_gcvt(-123.556, 1, buffer), "-123.6");
    EXPECT_STREQ(nva_gcvt(0.0, 1, buffer), "0.0");
    EXPECT_STREQ(nva_gcvt(-0.0, 1, buffer), "0.0");

    EXPECT_STREQ(nva_gcvt(123.456f, 1, buffer), "123.4");
    EXPECT_STREQ(nva_gcvt(-123.456f, 1, buffer), "-123.4");
    EXPECT_STREQ(nva_gcvt(123.556f, 1, buffer), "123.6");
    EXPECT_STREQ(nva_gcvt(-123.556f, 1, buffer), "-123.6");
    EXPECT_STREQ(nva_gcvt(0.0f, 1, buffer), "0.0");
    EXPECT_STREQ(nva_gcvt(-0.0f, 1, buffer), "0.0");
}
