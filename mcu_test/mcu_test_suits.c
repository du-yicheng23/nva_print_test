/**
 * @file mcu_test_suits.c
 * @author DuYicheng
 * @date 2025-07-18
 * @brief mcu test suits
 */

#include "mcu_test_suits.h"

#include "nva/print.h"

void nva_mcu_test_run(void)
{
    nva_print("{:*^30}\n\n", nva_str("mcu_test begin", NVA_START));

    nva_print(
        "std::array<std::array<int, 3>, 3> arr{{{{1, 2, 3}}, {{4, 5, 6}}, {{7, 8, 9}}}}; std::cout << arr[0][0] << "
        "std::endl;\n",
        NVA_START);
    nva_print("1\n\n", NVA_START);

    nva_print("Hello, {}!\n\n", nva_str("world", NVA_START));

    nva_print("array = [{2}, {0}, {1}].\n\n", nva_int(1, nva_int(2, nva_int(3, NVA_START))));

    nva_print("{0}{1}{0}\n\n", nva_str("abra", nva_str("cad", NVA_START)));

    nva_print("Number: {1}, Hex: {0:x}, FloatPoint: {2:.2f}\n\n",
              nva_int(0xFF, nva_int(42, nva_float(3.14159f, NVA_START))));

    nva_print("{:=^30}\n\n", nva_str("mcu_test end", NVA_START));
}
