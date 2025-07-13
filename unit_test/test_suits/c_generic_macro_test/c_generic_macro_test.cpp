/**
 * @file c_generic_macro_test.cpp
 * @author DuYicheng
 * @date 2025-07-13
 * @brief C 语言的 _Generic 宏测试 - gtest 调用处
 */

#include "gtest/gtest.h"

extern "C" const char* generic_macro_test_main(void);

TEST(CGenericMacroTest, test)
{
    const auto msg = generic_macro_test_main();

    EXPECT_EQ(msg, nullptr) << msg;
}
