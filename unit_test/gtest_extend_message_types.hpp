/**
 * @file gtest_extend_message_types.hpp
 * @author DuYicheng
 * @date 2025-07-13
 * @brief gtest 更多的消息输出函数
 */

#pragma once
#ifndef NVA_GTEST_EXTEND_MESSAGE_TYPES_HPP
#define NVA_GTEST_EXTEND_MESSAGE_TYPES_HPP

#include "gtest/gtest.h"

template<typename T>
testing::Message& operator<<(testing::Message& ostream, const std::vector<T>& array)
{
    ostream.operator<<("[");

    for (auto it = array.begin(); it < array.end() - 1; ++it) {
        ostream << *it << ", ";
    }

    ostream << *(array.end() - 1) << "]";

    return ostream;
}

template<typename T, std::size_t sz>
testing::Message& operator<<(testing::Message& ostream, const std::array<T, sz>& array)
{
    ostream.operator<<("[");

    for (auto it = array.begin(); it < array.end() - 1; ++it) {
        ostream << *it << ", ";
    }

    ostream << *(array.end() - 1) << "]";

    return ostream;
}

template<typename T, std::size_t sz>
testing::Message& operator<<(testing::Message& ostream, const T (&array)[sz])
{
    ostream.operator<<("[");

    for (std::size_t i = 0U; i < sz - 1; ++i) {
        ostream << array[i] << ", ";
    }

    ostream << array[sz - 1] << "]";

    return ostream;
}

#endif /* !NVA_GTEST_EXTEND_MESSAGE_TYPES_HPP */
