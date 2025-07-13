/**
 * @file nva_memcpy_test.cpp
 * @author DuYicheng
 * @date 2025-07-12
 * @brief 对 nva_memcpy 更详细的测试
 */

#include "gtest/gtest.h"

#include <cstring>
#include <cstdint>
#include <vector>
#include <algorithm>

#include "gtest_extend_message_types.hpp"

#include "nva/string.h"

#define MEMCPY nva_memcpy

/*----------------------------------------------------------*/
/* 辅助函数与 fixture                                         */
static void fill_seq(uint8_t* buf, size_t len)
{
    for (size_t i = 0; i < len; ++i) {
        buf[i] = static_cast<uint8_t>(i & 0xFF);
    }
}

static bool buffers_equal(const uint8_t* a, const uint8_t* b, size_t n)
{
    return std::memcmp(a, b, n) == 0;
}

class MemcpyTest : public ::testing::Test
{
protected:
    static constexpr size_t kMax = 4096;
    uint8_t src[kMax]{};
    uint8_t dst[kMax]{};

    void SetUp() override
    {
        fill_seq(src, kMax);
        std::memset(dst, 0xAA, kMax);  // 填充 0xAA，方便发现越界
    }
};

/*----------------------------------------------------------*/
/* 1~10：零长度、一字节、小尺寸                               */
TEST_F(MemcpyTest, ZeroLength)
{
    MEMCPY(dst, src, 0);
    EXPECT_TRUE(buffers_equal(dst, src, 0));
}
TEST_F(MemcpyTest, OneByte)
{
    MEMCPY(dst, src, 1);
    EXPECT_TRUE(buffers_equal(dst, src, 1));
}
TEST_F(MemcpyTest, TwoBytes)
{
    MEMCPY(dst, src, 2);
    EXPECT_TRUE(buffers_equal(dst, src, 2));
}
TEST_F(MemcpyTest, ThreeBytes)
{
    MEMCPY(dst, src, 3);
    EXPECT_TRUE(buffers_equal(dst, src, 3));
}
TEST_F(MemcpyTest, FourBytes)
{
    MEMCPY(dst, src, 4);
    EXPECT_TRUE(buffers_equal(dst, src, 4));
}
TEST_F(MemcpyTest, SevenBytes)
{
    MEMCPY(dst, src, 7);
    EXPECT_TRUE(buffers_equal(dst, src, 7));
}
TEST_F(MemcpyTest, EightBytes)
{
    MEMCPY(dst, src, 8);
    EXPECT_TRUE(buffers_equal(dst, src, 8));
}
TEST_F(MemcpyTest, FifteenBytes)
{
    MEMCPY(dst, src, 15);
    EXPECT_TRUE(buffers_equal(dst, src, 15));
}
TEST_F(MemcpyTest, SixteenBytes)
{
    MEMCPY(dst, src, 16);
    EXPECT_TRUE(buffers_equal(dst, src, 16));
}
TEST_F(MemcpyTest, ThirtyTwoBytes)
{
    MEMCPY(dst, src, 32);
    EXPECT_TRUE(buffers_equal(dst, src, 32));
}

/* 11~20：中等尺寸、整页边界、奇数长度                        */
TEST_F(MemcpyTest, HundredBytes)
{
    MEMCPY(dst, src, 100);
    EXPECT_TRUE(buffers_equal(dst, src, 100));
}
TEST_F(MemcpyTest, ThousandBytes)
{
    MEMCPY(dst, src, 1000);
    EXPECT_TRUE(buffers_equal(dst, src, 1000));
}
TEST_F(MemcpyTest, PageMinus1)
{
    MEMCPY(dst, src, 4095);
    EXPECT_TRUE(buffers_equal(dst, src, 4095));
}
TEST_F(MemcpyTest, PageSize)
{
    MEMCPY(dst, src, 4096);
    EXPECT_TRUE(buffers_equal(dst, src, 4096));
}
TEST_F(MemcpyTest, OddSize127)
{
    MEMCPY(dst, src, 127);
    EXPECT_TRUE(buffers_equal(dst, src, 127));
}
TEST_F(MemcpyTest, OddSize129)
{
    MEMCPY(dst, src, 129);
    EXPECT_TRUE(buffers_equal(dst, src, 129));
}
TEST_F(MemcpyTest, Medium512)
{
    MEMCPY(dst, src, 512);
    EXPECT_TRUE(buffers_equal(dst, src, 512));
}
TEST_F(MemcpyTest, Medium1023)
{
    MEMCPY(dst, src, 1023);
    EXPECT_TRUE(buffers_equal(dst, src, 1023));
}
TEST_F(MemcpyTest, Medium1025)
{
    MEMCPY(dst, src, 1025);
    EXPECT_TRUE(buffers_equal(dst, src, 1025));
}
TEST_F(MemcpyTest, Medium2048)
{
    MEMCPY(dst, src, 2048);
    EXPECT_TRUE(buffers_equal(dst, src, 2048));
}

/* 21~30：重叠检测（memcpy 本身不保证支持，但不能导致崩溃）   */
TEST_F(MemcpyTest, OverlapSrcBeforeDst)
{
    std::memset(src, 0x55, 100);
    std::memset(src + 50, 0xAA, 50);  // src[50:99] = 0xAA
    EXPECT_NO_FATAL_FAILURE(MEMCPY(src + 10, src, 100));
}
TEST_F(MemcpyTest, OverlapDstBeforeSrc)
{
    std::memset(src, 0xCC, 200);
    EXPECT_NO_FATAL_FAILURE(MEMCPY(src, src + 17, 100));  // 非重叠
}
TEST_F(MemcpyTest, SameSrcDst)
{
    std::memset(src, 0x77, 256);
    EXPECT_NO_FATAL_FAILURE(MEMCPY(src, src, 256));
}
TEST_F(MemcpyTest, OverlapExactPage)
{
    std::vector<uint8_t> buf(8192);
    fill_seq(buf.data(), 8192);
    MEMCPY(buf.data() + 4096, buf.data(), 4096);
    EXPECT_TRUE(buffers_equal(buf.data() + 4096, buf.data(), 4096));
}
TEST_F(MemcpyTest, OverlapOneByteShift)
{
    std::vector<uint8_t> buf(1024);
    fill_seq(buf.data(), 1024);
    EXPECT_NO_FATAL_FAILURE(MEMCPY(buf.data() + 1, buf.data(), 1023));
}

/* 26~30：对齐测试                                             */
TEST_F(MemcpyTest, SrcUnalignedDstAligned)
{
    MEMCPY(dst + 1, src + 3, 64);
    EXPECT_TRUE(buffers_equal(dst + 1, src + 3, 64));
}
TEST_F(MemcpyTest, SrcAlignedDstUnaligned)
{
    MEMCPY(dst + 3, src + 1, 64);
    EXPECT_TRUE(buffers_equal(dst + 3, src + 1, 64));
}
TEST_F(MemcpyTest, BothUnaligned)
{
    MEMCPY(dst + 5, src + 7, 128);
    EXPECT_TRUE(buffers_equal(dst + 5, src + 7, 128));
}
TEST_F(MemcpyTest, Misaligned64Bytes)
{
    MEMCPY(dst + 2, src + 6, 64);
    EXPECT_TRUE(buffers_equal(dst + 2, src + 6, 64));
}
TEST_F(MemcpyTest, MisalignedOddStart)
{
    MEMCPY(dst + 1, src + 2, 99);
    EXPECT_TRUE(buffers_equal(dst + 1, src + 2, 99));
}

/* 31~35：极端大小和空指针                                     */
TEST_F(MemcpyTest, Large64k)
{
    std::vector<uint8_t> big_src(64 * 1024);
    std::vector<uint8_t> big_dst(64 * 1024);
    fill_seq(big_src.data(), big_src.size());
    MEMCPY(big_dst.data(), big_src.data(), big_src.size());
    EXPECT_TRUE(buffers_equal(big_dst.data(), big_src.data(), big_src.size()));
}

// 仅测试无崩溃即可
TEST_F(MemcpyTest, NullDst)
{
    EXPECT_NO_FATAL_FAILURE(MEMCPY(nullptr, src, 0));
}
TEST_F(MemcpyTest, NullSrc)
{
    EXPECT_NO_FATAL_FAILURE(MEMCPY(dst, nullptr, 0));
}
TEST_F(MemcpyTest, NullBothZeroLen)
{
    EXPECT_NO_FATAL_FAILURE(MEMCPY(nullptr, nullptr, 0));
}

// 可能崩溃的情况
TEST_F(MemcpyTest, NullBothWithLen)
{
    EXPECT_DEATH(MEMCPY(nullptr, nullptr, 1), "");
    SUCCEED();
}

/* 36~40：单字节、单字、单双字边界值                           */
TEST_F(MemcpyTest, BoundaryU8)
{
    uint8_t a = 0xAB, b;
    MEMCPY(&b, &a, 1);
    EXPECT_EQ(b, 0xAB);
}
TEST_F(MemcpyTest, BoundaryU16)
{
    uint16_t a = 0xDEAD, b;
    MEMCPY(&b, &a, 2);
    EXPECT_EQ(b, 0xDEAD);
}
TEST_F(MemcpyTest, BoundaryU32)
{
    uint32_t a = 0xCAFEBABE, b;
    MEMCPY(&b, &a, 4);
    EXPECT_EQ(b, 0xCAFEBABE);
}
TEST_F(MemcpyTest, BoundaryU64)
{
    uint64_t a = 0x0123456789ABCDEFULL, b;
    MEMCPY(&b, &a, 8);
    EXPECT_EQ(b, 0x0123456789ABCDEFULL);
}
TEST_F(MemcpyTest, Boundary128Bit)
{
    alignas(16) uint8_t a[16] = {0};
    alignas(16) uint8_t b[16];
    fill_seq(a, 16);
    MEMCPY(b, a, 16);
    EXPECT_TRUE(buffers_equal(a, b, 16));
}

/* NOLINTBEGIN(*-msc50-cpp) */

/* 41~45：随机长度随机对齐                                     */
TEST_F(MemcpyTest, RandomTiny)
{
    for (int i = 0; i < 1000; ++i) {
        const size_t len = rand() % 64;
        const size_t src_off = rand() % 8;
        const size_t dst_off = rand() % 8;

        const auto& src_origin = src;

        std::array<uint8_t, kMax> golden{};

        std::memcpy(golden.data() + dst_off, src_origin + src_off, len);

        MEMCPY(dst + dst_off, src_origin + src_off, len);
        EXPECT_TRUE(buffers_equal(dst + dst_off, golden.data() + dst_off, len));
    }
}
TEST_F(MemcpyTest, RandomMedium)
{
    for (int i = 0; i < 1000; ++i) {
        const size_t len = rand() % (4096 - 16);
        const size_t src_off = rand() % 16;
        const size_t dst_off = rand() % 16;

        const auto& src_origin = src;

        std::array<uint8_t, kMax> golden{};

        std::memcpy(golden.data() + dst_off, src_origin + src_off, len);

        MEMCPY(dst + dst_off, src_origin + src_off, len);
        EXPECT_TRUE(buffers_equal(dst + dst_off, golden.data() + dst_off, len))
            << "len=" << len << ", src_off= " << src_off << ", dst_off=" << dst_off << "\ngolden=" << golden
            << "\ndst=" << dst;
    }
}

/* NOLINTEND(*-msc50-cpp) */

/* 46~50：性能/压力（仅检查正确性，不真正跑性能）               */
TEST_F(MemcpyTest, Stress1M)
{
    std::vector<uint8_t> big_src(1 * 1024 * 1024);
    std::vector<uint8_t> big_dst(1 * 1024 * 1024);
    fill_seq(big_src.data(), big_src.size());
    MEMCPY(big_dst.data(), big_src.data(), big_src.size());
    EXPECT_TRUE(buffers_equal(big_dst.data(), big_src.data(), big_src.size()));
}
TEST_F(MemcpyTest, Stress16M)
{
    std::vector<uint8_t> big_src(16 * 1024 * 1024);
    std::vector<uint8_t> big_dst(16 * 1024 * 1024);
    fill_seq(big_src.data(), big_src.size());
    MEMCPY(big_dst.data(), big_src.data(), big_src.size());
    EXPECT_TRUE(buffers_equal(big_dst.data(), big_src.data(), big_src.size()));
}
TEST_F(MemcpyTest, StressUnaligned16M)
{
    std::vector<uint8_t> big_src(16 * 1024 * 1024 + 7);
    std::vector<uint8_t> big_dst(16 * 1024 * 1024 + 7);
    fill_seq(big_src.data(), big_src.size());
    MEMCPY(big_dst.data() + 1, big_src.data() + 3, big_src.size() - 4);
    EXPECT_TRUE(buffers_equal(big_dst.data() + 1, big_src.data() + 3, big_src.size() - 4));
}
TEST_F(MemcpyTest, StressZeroAfterLarge)
{
    std::vector<uint8_t> buf(4096);
    fill_seq(buf.data(), 4096);
    MEMCPY(buf.data() + 2048, buf.data(), 2048);
    EXPECT_TRUE(buffers_equal(buf.data() + 2048, buf.data(), 2048));
    EXPECT_NO_FATAL_FAILURE(MEMCPY(buf.data(), buf.data() + 2048, 0));  // 零长度
}
TEST_F(MemcpyTest, StressAllBytes)
{
    uint8_t all[256];
    for (int i = 0; i < 256; ++i) all[i] = static_cast<uint8_t>(i);
    uint8_t out[256];
    MEMCPY(out, all, 256);
    EXPECT_TRUE(buffers_equal(out, all, 256));
}
