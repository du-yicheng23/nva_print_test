/**
 * @file main.c
 * @author DuYicheng
 * @date 2025-06-29
 * @brief 主函数
 */

#include <REGX51.H>

#include "nva/print.h"
#include "mcu_test_suits.h"

void serialInit(void);

int nva_putchar(char c)
{
    SBUF = c;

	while (TI == 0) {
    }
	TI = 0;

    return 1;
}

void main(void)
{
    serialInit();

    nva_mcu_test_run();

    while (1) {
    }
}

void serialInit(void)
{
    SCON = 0x40;
    PCON = 0x00;

	TMOD = 0x20;
	TH1 = 0xFD;
	TL1 = 0xFD;
	TI = 0;
	TR1 = 1;
}
