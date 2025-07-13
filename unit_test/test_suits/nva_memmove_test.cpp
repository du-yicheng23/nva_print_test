/**
 * @file nva_memmove_test.cpp
 * @author DuYicheng
 * @date 2025-07-12
 * @brief 对 nva_memmove 更详细的测试
 */

#include "gtest/gtest.h"

#include <cstring>
#include <cstdint>
#include <vector>

#include "nva/string.h"

#define MEMMOVE nva_memmove

static void fill_seq(uint8_t* buf, const size_t len)
{
    for (size_t i = 0; i < len; ++i) {
        buf[i] = static_cast<uint8_t>(i & 0xFF);
    }
}
static bool eq(const uint8_t* a, const uint8_t* b, const size_t n)
{
    return std::memcmp(a, b, n) == 0;
}

class MemmoveTest : public ::testing::Test
{
protected:
    static constexpr size_t kMax = 64 * 1024 * 1024 + 64;
    std::vector<uint8_t> buffer;

    void SetUp() override
    {
        buffer.resize(kMax);
    }

    uint8_t* data()
    {
        return buffer.data();
    }
};

/*----------------------------------------------------------*/
/* 01~10：零/极小长度                                         */
TEST_F(MemmoveTest, ZeroLength)
{
    MEMMOVE(data(), data(), 0);
    SUCCEED();
}
TEST_F(MemmoveTest, OneByte)
{
    fill_seq(data(), 1);
    MEMMOVE(data() + 1, data(), 1);
    EXPECT_EQ(data()[1], 0);
}
TEST_F(MemmoveTest, TwoBytes)
{
    fill_seq(data(), 2);
    MEMMOVE(data() + 2, data(), 2);
    EXPECT_TRUE(eq(data() + 2, data(), 2));
}
TEST_F(MemmoveTest, ThreeBytes)
{
    fill_seq(data(), 3);
    MEMMOVE(data() + 3, data(), 3);
    EXPECT_TRUE(eq(data() + 3, data(), 3));
}
TEST_F(MemmoveTest, FourBytes)
{
    fill_seq(data(), 4);
    MEMMOVE(data() + 4, data(), 4);
    EXPECT_TRUE(eq(data() + 4, data(), 4));
}
TEST_F(MemmoveTest, FiveBytes)
{
    fill_seq(data(), 5);
    MEMMOVE(data() + 5, data(), 5);
    EXPECT_TRUE(eq(data() + 5, data(), 5));
}
TEST_F(MemmoveTest, SixBytes)
{
    fill_seq(data(), 6);
    MEMMOVE(data() + 6, data(), 6);
    EXPECT_TRUE(eq(data() + 6, data(), 6));
}
TEST_F(MemmoveTest, SevenBytes)
{
    fill_seq(data(), 7);
    MEMMOVE(data() + 7, data(), 7);
    EXPECT_TRUE(eq(data() + 7, data(), 7));
}
TEST_F(MemmoveTest, EightBytes)
{
    fill_seq(data(), 8);
    MEMMOVE(data() + 8, data(), 8);
    EXPECT_TRUE(eq(data() + 8, data(), 8));
}
TEST_F(MemmoveTest, NineToSixteen)
{
    fill_seq(data(), 16);
    MEMMOVE(data() + 16, data(), 16);
    EXPECT_TRUE(eq(data() + 16, data(), 16));
}

/* 11~25：小/中/大长度                                        */
TEST_F(MemmoveTest, ThirtyTwo)
{
    fill_seq(data(), 32);
    MEMMOVE(data() + 32, data(), 32);
    EXPECT_TRUE(eq(data() + 32, data(), 32));
}
TEST_F(MemmoveTest, SixtyFour)
{
    fill_seq(data(), 64);
    MEMMOVE(data() + 64, data(), 64);
    EXPECT_TRUE(eq(data() + 64, data(), 64));
}
TEST_F(MemmoveTest, OneK)
{
    fill_seq(data(), 1024);
    MEMMOVE(data() + 1024, data(), 1024);
    EXPECT_TRUE(eq(data() + 1024, data(), 1024));
}
TEST_F(MemmoveTest, FourK)
{
    fill_seq(data(), 4096);
    MEMMOVE(data() + 4096, data(), 4096);
    EXPECT_TRUE(eq(data() + 4096, data(), 4096));
}
TEST_F(MemmoveTest, SixtyFourK)
{
    fill_seq(data(), 65536);
    MEMMOVE(data() + 65536, data(), 65536);
    EXPECT_TRUE(eq(data() + 65536, data(), 65536));
}
TEST_F(MemmoveTest, OneMeg)
{
    fill_seq(data(), 1 << 20);
    MEMMOVE(data() + (1 << 20), data(), 1 << 20);
    EXPECT_TRUE(eq(data() + (1 << 20), data(), 1 << 20));
}
TEST_F(MemmoveTest, TwoMeg)
{
    fill_seq(data(), 2 << 20);
    MEMMOVE(data() + (2 << 20), data(), 2 << 20);
    EXPECT_TRUE(eq(data() + (2 << 20), data(), 2 << 20));
}
TEST_F(MemmoveTest, FourMeg)
{
    fill_seq(data(), 4 << 20);
    MEMMOVE(data() + (4 << 20), data(), 4 << 20);
    EXPECT_TRUE(eq(data() + (4 << 20), data(), 4 << 20));
}
TEST_F(MemmoveTest, EightMeg)
{
    fill_seq(data(), 8 << 20);
    MEMMOVE(data() + (8 << 20), data(), 8 << 20);
    EXPECT_TRUE(eq(data() + (8 << 20), data(), 8 << 20));
}
TEST_F(MemmoveTest, SixteenMeg)
{
    fill_seq(data(), 16 << 20);
    MEMMOVE(data() + (16 << 20), data(), 16 << 20);
    EXPECT_TRUE(eq(data() + (16 << 20), data(), 16 << 20));
}
TEST_F(MemmoveTest, ThirtyTwoMeg)
{
    fill_seq(data(), 32 << 20);
    MEMMOVE(data() + (32 << 20), data(), 32 << 20);
    EXPECT_TRUE(eq(data() + (32 << 20), data(), 32 << 20));
}

/* 26~40：对齐/非对齐偏移                                     */
#define GEN_ALIGNED(off)                                   \
    TEST_F(MemmoveTest, Offset##off)                       \
    {                                                      \
        const size_t len = 1024;                           \
        fill_seq(data(), len);                             \
        std::vector<uint8_t> golden(len);                  \
        std::memcpy(golden.data(), data(), len);           \
        MEMMOVE(data() + off, data(), len);                \
        EXPECT_TRUE(eq(data() + off, golden.data(), len)); \
    }

GEN_ALIGNED(1);
GEN_ALIGNED(2);
GEN_ALIGNED(3);
GEN_ALIGNED(4);
GEN_ALIGNED(5);
GEN_ALIGNED(6);
GEN_ALIGNED(7);
GEN_ALIGNED(8);
GEN_ALIGNED(9);
GEN_ALIGNED(10);
GEN_ALIGNED(11);
GEN_ALIGNED(12);
GEN_ALIGNED(13);
GEN_ALIGNED(14);
GEN_ALIGNED(15);

/* 41~65：重叠场景 —— 前向（dest < src）                       */
#define GEN_FWD(sz)                                 \
    TEST_F(MemmoveTest, Forward##sz)                \
    {                                               \
        fill_seq(data(), sz + 1);                   \
        std::vector<uint8_t> golden(sz);            \
        std::memcpy(golden.data(), data() + 1, sz); \
        MEMMOVE(data(), data() + 1, sz);            \
        EXPECT_TRUE(eq(data(), golden.data(), sz)); \
    }

GEN_FWD(1);
GEN_FWD(2);
GEN_FWD(3);
GEN_FWD(4);
GEN_FWD(5);
GEN_FWD(6);
GEN_FWD(7);
GEN_FWD(8);
GEN_FWD(15);
GEN_FWD(16);
GEN_FWD(31);
GEN_FWD(32);
GEN_FWD(63);
GEN_FWD(64);
GEN_FWD(127);
GEN_FWD(128);
GEN_FWD(1023);
GEN_FWD(1024);
GEN_FWD(4095);
GEN_FWD(4096);
GEN_FWD(4097);
GEN_FWD(8192);
GEN_FWD(16384);
GEN_FWD(32768);
GEN_FWD(65536);

/* 66~90：重叠场景 —— 后向（dest > src）                       */
#define GEN_BACK(sz)                                    \
    TEST_F(MemmoveTest, Backward##sz)                   \
    {                                                   \
        fill_seq(data(), sz + 1);                       \
        std::vector<uint8_t> golden(sz);                \
        std::memcpy(golden.data(), data(), sz);         \
        MEMMOVE(data() + 1, data(), sz);                \
        EXPECT_TRUE(eq(data() + 1, golden.data(), sz)); \
    }

GEN_BACK(1);
GEN_BACK(2);
GEN_BACK(3);
GEN_BACK(4);
GEN_BACK(5);
GEN_BACK(6);
GEN_BACK(7);
GEN_BACK(8);
GEN_BACK(15);
GEN_BACK(16);
GEN_BACK(31);
GEN_BACK(32);
GEN_BACK(63);
GEN_BACK(64);
GEN_BACK(127);
GEN_BACK(128);
GEN_BACK(1023);
GEN_BACK(1024);
GEN_BACK(4095);
GEN_BACK(4096);
GEN_BACK(4097);
GEN_BACK(8192);
GEN_BACK(16384);
GEN_BACK(32768);
GEN_BACK(65536);

/* 91~95：完全重叠、空指针                                   */
TEST_F(MemmoveTest, SamePtr)
{
    fill_seq(data(), 256);
    MEMMOVE(data(), data(), 256);
    EXPECT_TRUE(eq(data(), data(), 256));
}
TEST_F(MemmoveTest, NullDstZeroLen)
{
    EXPECT_NO_FATAL_FAILURE(MEMMOVE(nullptr, data(), 0));
}
TEST_F(MemmoveTest, NullSrcZeroLen)
{
    EXPECT_NO_FATAL_FAILURE(MEMMOVE(data(), nullptr, 0));
}
TEST_F(MemmoveTest, NullBothZeroLen)
{
    EXPECT_NO_FATAL_FAILURE(MEMMOVE(nullptr, nullptr, 0));
}
TEST_F(MemmoveTest, NullDstNonZero)
{
    EXPECT_DEATH(MEMMOVE(nullptr, data(), 1), "");
}

/* 96~100：随机组合（长度、偏移、重叠方向）                    */
TEST_F(MemmoveTest, Random1000)
{
    for (int i = 0; i < 1000; ++i) {
        /* NOLINTBEGIN(*-msc50-cpp) */
        const size_t len = rand() % 4096 + 1;
        const size_t off = rand() % 16;
        const bool fwd = rand() % 2;
        /* NOLINTEND(*-msc50-cpp) */

        /* 用两块独立区域：src 不会被破坏 */
        std::vector<uint8_t> src(len + off + 16);
        std::vector<uint8_t> dst(len + off + 16);

        const auto value_origin = src.size();

        fill_seq(src.data(), value_origin);

        const auto& src_origin = src;

        std::vector<uint8_t> golden = dst;

        /* 生成 golden：先按方向拷贝到 dst */
        if (fwd) {
            std::memmove(golden.data(), src_origin.data() + off, len);
        }
        else {
            std::memmove(golden.data() + off, src_origin.data(), len);
        }

        /* 在 dst 上执行 memmove（重叠场景） */
        if (fwd) {
            MEMMOVE(dst.data(), src_origin.data() + off, len);
        }
        else {
            MEMMOVE(dst.data() + off, src_origin.data(), len);
        }

        /* 与 golden 比较 */
        if (fwd) {
            EXPECT_TRUE(eq(dst.data(), golden.data(), len));
        }
        else {
            EXPECT_TRUE(eq(dst.data(), golden.data(), len));
        }
    }
}
