/**
 * @file print_test.cpp
 * @author DuYicheng
 * @date 2025-07-16
 * @brief 打印测试
 */

#include "gtest/gtest.h"

#include <algorithm>
#include <array>

#include "nva/print.h"

static struct PrintTargetBuffer {
    std::array<char, 128> buffer;
    std::size_t index;
} print_target_buffer{.buffer{}, .index = 0U};

extern "C" int nva_putchar(const char c)
{
    if (print_target_buffer.index >= print_target_buffer.buffer.size()) {
        return -1;
    }

    print_target_buffer.buffer[print_target_buffer.index++] = c;

    return 1;
}

#define NVA_PRINT_EQ(format, status, expect)                         \
    do {                                                             \
        EXPECT_EQ(nva_print((format), (status)), NVA_SUCCESS);       \
        EXPECT_STREQ((print_target_buffer.buffer.data()), (expect)); \
        print_target_buffer.buffer.fill('\0');                       \
        print_target_buffer.index = 0;                               \
    } while (0)

TEST(PrintTest, NoneTest)
{
    NVA_PRINT_EQ("||{{Hello, World!}}__", NVA_START, "||{Hello, World!}__");
}

TEST(PrintTest, IntegerTest)
{
    NVA_PRINT_EQ("int a = {}", nva::add(26471, NVA_START), "int a = 26471");
}
