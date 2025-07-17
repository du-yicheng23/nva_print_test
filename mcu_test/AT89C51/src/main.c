/**
 * @file main.c
 * @author DuYicheng
 * @date 2025-06-29
 * @brief 主函数
 */

#include "nva/print.h"

int nva_putchar(char c)
{
    return c;
}

void main(void)
{
    nva_print("Hello, {}\n", nva_str("world!", NVA_START));
    while (1) {
    }
}
