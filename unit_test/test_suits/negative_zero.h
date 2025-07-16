/**
 * @file negative_zero.h
 * @author DuYicheng
 * @date 2025-07-15
 * @brief 负零
 */

#pragma once
#ifndef NVA_NEGATIVE_ZERO_H
#define NVA_NEGATIVE_ZERO_H

#ifdef NVA_NO_INF_AND_NAN

#define NEGATIVE_ZERO_PREFIX ""

#else

#define NEGATIVE_ZERO_PREFIX "-"

#endif

#endif /* !NVA_NEGATIVE_ZERO_H */
