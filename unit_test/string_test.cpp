/**
 * @file string_test.cpp
 * @author DuYicheng
 * @date 2025-06-28
 * @brief nva 字符串测试
 */

#include "gtest/gtest.h"

#include "nva/string.h"

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
