Because this project have unit test(in `unit_test` directory) already, it is not necessary to achieve high code coverage. The focus is on ensuring that the classic test cases can run successfully on the embedded platform.

The classic test cases is already listed in `mcu_test_suits.c`. You only need to add the `mcu_test_suits.c` and `mcu_test_suits.h` files to your project and call the `mcu_test_suits_run()` function in your main function.

---

由于已经进行了单元测试（见 `unit_test`），因此这一项测试无需追求详细的测试覆盖率，只需测试一些经典的样例能否在嵌入式平台上运行即可。

这些经典的测试样例已经在 `mcu_test_suits.c` 中列出。您只需将 `mcu_test_suits.c` 和 `mcu_test_suits.h` 文件添加到您的工程中，并在主函数中调用 `mcu_test_suits_run()` 函数即可。
