/**
 * @file stack_test.cpp
 * @author DuYicheng
 * @date 2025-06-29
 * @brief 栈测试
 */

#include <gtest/gtest.h>

#include "nva/stack.h"

class StackTest : public ::testing::Test
{
protected:
    nva_Stack stack{NVA_STACK_INIT_VALUE};

    void SetUp() override {}

    void TearDown() override {}
};

// NVA_IS_SIGNED 宏测试
TEST(StackMacroTest, IsSigned)
{
    EXPECT_FALSE(NVA_IS_SIGNED(NVA_TYPEID_CHAR));
    EXPECT_TRUE(NVA_IS_SIGNED(NVA_TYPEID_SCHAR));
    EXPECT_FALSE(NVA_IS_SIGNED(NVA_TYPEID_UCHAR));
    EXPECT_TRUE(NVA_IS_SIGNED(NVA_TYPEID_SSHORT));
    EXPECT_FALSE(NVA_IS_SIGNED(NVA_TYPEID_USHORT));
    EXPECT_TRUE(NVA_IS_SIGNED(NVA_TYPEID_SINT));
    EXPECT_FALSE(NVA_IS_SIGNED(NVA_TYPEID_UINT));
    EXPECT_TRUE(NVA_IS_SIGNED(NVA_TYPEID_SLONG));
    EXPECT_FALSE(NVA_IS_SIGNED(NVA_TYPEID_ULONG));
    EXPECT_TRUE(NVA_IS_SIGNED(NVA_TYPEID_SLLONG));
    EXPECT_FALSE(NVA_IS_SIGNED(NVA_TYPEID_ULLONG));
    EXPECT_FALSE(NVA_IS_SIGNED(NVA_TYPEID_PTR));
    EXPECT_TRUE(NVA_IS_SIGNED(NVA_TYPEID_FLOAT));
    EXPECT_TRUE(NVA_IS_SIGNED(NVA_TYPEID_DOUBLE));
    for (int i = 0; i < 256; ++i) {
        nva_TypeId tid = i;
        if (tid == NVA_TYPEID_CHAR) continue;
        if ((tid & 0xF0) == 0x10)
            EXPECT_TRUE(NVA_IS_SIGNED(tid));
        else
            EXPECT_FALSE(NVA_IS_SIGNED(tid));
    }
}

// NVA_IS_UNSIGNED 宏测试
TEST(StackMacroTest, IsUnsigned)
{
    EXPECT_FALSE(NVA_IS_UNSIGNED(NVA_TYPEID_CHAR));
    EXPECT_FALSE(NVA_IS_UNSIGNED(NVA_TYPEID_SCHAR));
    EXPECT_TRUE(NVA_IS_UNSIGNED(NVA_TYPEID_UCHAR));
    EXPECT_FALSE(NVA_IS_UNSIGNED(NVA_TYPEID_SSHORT));
    EXPECT_TRUE(NVA_IS_UNSIGNED(NVA_TYPEID_USHORT));
    EXPECT_FALSE(NVA_IS_UNSIGNED(NVA_TYPEID_SINT));
    EXPECT_TRUE(NVA_IS_UNSIGNED(NVA_TYPEID_UINT));
    EXPECT_FALSE(NVA_IS_UNSIGNED(NVA_TYPEID_SLONG));
    EXPECT_TRUE(NVA_IS_UNSIGNED(NVA_TYPEID_ULONG));
    EXPECT_FALSE(NVA_IS_UNSIGNED(NVA_TYPEID_SLLONG));
    EXPECT_TRUE(NVA_IS_UNSIGNED(NVA_TYPEID_ULLONG));
    EXPECT_TRUE(NVA_IS_UNSIGNED(NVA_TYPEID_PTR));
    EXPECT_FALSE(NVA_IS_UNSIGNED(NVA_TYPEID_FLOAT));
    EXPECT_FALSE(NVA_IS_UNSIGNED(NVA_TYPEID_DOUBLE));
    for (int i = 0; i < 256; ++i) {
        nva_TypeId tid = i;
        if (tid == NVA_TYPEID_CHAR) continue;
        if ((tid & 0xF0) == 0x20)
            EXPECT_TRUE(NVA_IS_UNSIGNED(tid));
        else
            EXPECT_FALSE(NVA_IS_UNSIGNED(tid));
    }
}

// NVA_TYPE_SIZE 宏测试
TEST(StackMacroTest, TypeSize)
{
    EXPECT_EQ(NVA_TYPE_SIZE(NVA_TYPEID_CHAR), sizeof(char));
    EXPECT_EQ(NVA_TYPE_SIZE(NVA_TYPEID_SCHAR), sizeof(char));
    EXPECT_EQ(NVA_TYPE_SIZE(NVA_TYPEID_UCHAR), sizeof(char));
    EXPECT_EQ(NVA_TYPE_SIZE(NVA_TYPEID_SSHORT), sizeof(short));
    EXPECT_EQ(NVA_TYPE_SIZE(NVA_TYPEID_USHORT), sizeof(short));
    EXPECT_EQ(NVA_TYPE_SIZE(NVA_TYPEID_SINT), sizeof(int));
    EXPECT_EQ(NVA_TYPE_SIZE(NVA_TYPEID_UINT), sizeof(int));
    EXPECT_EQ(NVA_TYPE_SIZE(NVA_TYPEID_SLONG), sizeof(long));
    EXPECT_EQ(NVA_TYPE_SIZE(NVA_TYPEID_ULONG), sizeof(long));
    EXPECT_EQ(NVA_TYPE_SIZE(NVA_TYPEID_SLLONG), sizeof(long long));
    EXPECT_EQ(NVA_TYPE_SIZE(NVA_TYPEID_ULLONG), sizeof(long long));
    EXPECT_EQ(NVA_TYPE_SIZE(NVA_TYPEID_PTR), sizeof(void*));
    EXPECT_EQ(NVA_TYPE_SIZE(NVA_TYPEID_FLOAT), sizeof(float));
    EXPECT_EQ(NVA_TYPE_SIZE(NVA_TYPEID_DOUBLE), sizeof(double));
    ASSERT_EQ(NVA_TYPE_SIZE(0x56), 0U);  // 测试无效类型ID

    for (int i = 0; i < 256; ++i) {
        const auto tid = static_cast<nva_TypeId>(i);
        const size_t sz = NVA_TYPE_SIZE(tid);

        if (tid == NVA_TYPEID_CHAR || tid == NVA_TYPEID_SCHAR || tid == NVA_TYPEID_UCHAR)
            EXPECT_EQ(sz, sizeof(char));
        else if (tid == NVA_TYPEID_SSHORT || tid == NVA_TYPEID_USHORT)
            EXPECT_EQ(sz, sizeof(short));
        else if (tid == NVA_TYPEID_SINT || tid == NVA_TYPEID_UINT)
            EXPECT_EQ(sz, sizeof(int));
        else if (tid == NVA_TYPEID_SLONG || tid == NVA_TYPEID_ULONG)
            EXPECT_EQ(sz, sizeof(long));
        else if (tid == NVA_TYPEID_SLLONG || tid == NVA_TYPEID_ULLONG)
            EXPECT_EQ(sz, sizeof(long long));
        else if (tid == NVA_TYPEID_PTR || tid == NVA_TYPEID_STR)
            EXPECT_EQ(sz, sizeof(void*));
        else if (tid == NVA_TYPEID_FLOAT)
            EXPECT_EQ(sz, sizeof(float));
        else if (tid == NVA_TYPEID_DOUBLE)
            EXPECT_EQ(sz, sizeof(double));
        else
            EXPECT_EQ(sz, 0U) << "i = " << i;
    }
}

// nva_stackInit 测试
TEST_F(StackTest, StackInit)
{
    nva_Stack s;
    nva_StatusCode code = nva_stackInit(&s);
    ASSERT_EQ(code, 0);
    ASSERT_EQ(s.type_top, 0);
    ASSERT_EQ(s.data_top, 0);

    ASSERT_EQ(stack.type_top, 0);
    ASSERT_EQ(stack.data_top, 0);
}

// 多类型混合测试
TEST_F(StackTest, StackMultiType)
{
    const char c = 'A';
    const short s = 1234;
    const int i = -5678;
    const long l = 987654;
    const float f = 3.14f;
    const double d = 2.71828;
    const void* const p = &i;
    const char* const str = "Hello, NVA!";
    ASSERT_EQ(nva_stackPush(&stack, &c, NVA_TYPEID_CHAR), NVA_SUCCESS);
    ASSERT_EQ(nva_stackPush(&stack, &s, NVA_TYPEID_SSHORT), NVA_SUCCESS);
    ASSERT_EQ(nva_stackPush(&stack, &i, NVA_TYPEID_SINT), NVA_SUCCESS);
    ASSERT_EQ(nva_stackPush(&stack, &l, NVA_TYPEID_SLONG), NVA_SUCCESS);
    ASSERT_EQ(nva_stackPush(&stack, &f, NVA_TYPEID_FLOAT), NVA_SUCCESS);
    ASSERT_EQ(nva_stackPush(&stack, &d, NVA_TYPEID_DOUBLE), NVA_SUCCESS);
    ASSERT_EQ(nva_stackPush(&stack, &p, NVA_TYPEID_PTR), NVA_SUCCESS);

    // 注意字符串实际上是被当作指针处理的，因此要加 & 符号
    ASSERT_EQ(nva_stackPush(&stack, &str, NVA_TYPEID_STR), NVA_SUCCESS);

    nva_TypeId tid = 0;
    const char* str_out = nullptr;
    ASSERT_EQ(nva_stackPop(&stack, &str_out, &tid), NVA_SUCCESS);
    EXPECT_EQ(tid, NVA_TYPEID_STR);
    EXPECT_STREQ(str_out, str);

    void* out = nullptr;
    double d_out = 0;
    ASSERT_EQ(nva_stackPop(&stack, &out, &tid), NVA_SUCCESS);
    EXPECT_EQ(tid, NVA_TYPEID_PTR);
    EXPECT_EQ(out, p);
    EXPECT_EQ(*static_cast<int*>(out), *static_cast<const int*>(p));

    float f_out = 0;
    ASSERT_EQ(nva_stackPop(&stack, &d_out, &tid), NVA_SUCCESS);
    EXPECT_EQ(tid, NVA_TYPEID_DOUBLE);
    EXPECT_DOUBLE_EQ(d_out, d);

    ASSERT_EQ(nva_stackPop(&stack, &f_out, &tid), NVA_SUCCESS);
    EXPECT_EQ(tid, NVA_TYPEID_FLOAT);
    EXPECT_FLOAT_EQ(f_out, f);

    long l_out = 0;
    ASSERT_EQ(nva_stackPop(&stack, &l_out, &tid), NVA_SUCCESS);
    EXPECT_EQ(tid, NVA_TYPEID_SLONG);
    EXPECT_EQ(l_out, l);

    int i_out = 0;
    ASSERT_EQ(nva_stackPop(&stack, &i_out, &tid), NVA_SUCCESS);
    EXPECT_EQ(tid, NVA_TYPEID_SINT);
    EXPECT_EQ(i_out, i);

    short s_out = 0;
    ASSERT_EQ(nva_stackPop(&stack, &s_out, &tid), NVA_SUCCESS);
    EXPECT_EQ(tid, NVA_TYPEID_SSHORT);
    EXPECT_EQ(s_out, s);

    char c_out = 0;
    ASSERT_EQ(nva_stackPop(&stack, &c_out, &tid), NVA_SUCCESS);
    EXPECT_EQ(tid, NVA_TYPEID_CHAR);
    EXPECT_EQ(c_out, c);
}

// NVA_STACK_INIT_VALUE 测试
TEST(StackMacroTest, StackInitValue)
{
    nva_Stack s = NVA_STACK_INIT_VALUE;
    EXPECT_EQ(s.type_top, 0);
    EXPECT_EQ(s.data_top, 0);
    for (int i = 0; i < NVA_STACK_DEFAULT_SIZE; ++i) {
        EXPECT_EQ(s.type[i], 0);
        EXPECT_EQ(s.data_store[i], 0);
    }
}
