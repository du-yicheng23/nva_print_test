# nva_print_test

[en_US](./README.md) | zh_CN

---

这是是项目 [nva_print](https://github.com/du-yicheng23/nva_print) 的测试仓库，
用于存储并运行 `nva_print` 的测试代码和测试用例。

## 构建并运行测试

目前有三种测试路径：`build_test`、`unit_test` 和 `mcu_test`。

除 `mcu_test` 路径下的部分平台，目前仅支持使用 CMake 进行构建和测试。
您需要预定义 CMake 变量 `NVA_TEST_ITEM_NAME` 以指定运行的测试路径，变量的值从上述测试路径中选择一个。

构建示例如下：
```bash
cmake -S . -B build -DNVA_TEST_ITEM_NAME="unit_test"  # will build unit_test. you can replace it to another directory.
cmake --build build
```

### build_test

此测试路径仅用于测试 `nva_print` 能否在 C90 标准下编译通过，编译通过即视为测试通过。

### unit_test

此路径基于 [googletest](https://github.com/google/googletest/) 和
[minunit](https://github.com/siu/minunit/) 测试框架对 `nva_print` 进行单元测试。

在构建了 `unit_test` 后，您可以运行以下命令来执行测试：
```bash
cd build
ctest
```
所有测试都将运行，并显示测试结果。

### mcu_test

此路径用于测试 `nva_print` 在嵌入式平台（如 STM32）上的兼容性和性能。

由于嵌入式平台的特殊性，部分平台不支持使用 CMake 进行构建和测试。您可以查看 `mcu_test`
下各自平台的路径内的 `README.md` 文件以获取构建并运行的方式。

对于支持 CMake 的嵌入式平台，您可以查看 `mcu_test/README.md` 文件以获取更多信息。

## 贡献指南

欢迎您为 `nva_print` 提交 PR 或 Issue，帮助我们改进和完善这个项目。

考虑到测试的内容较多，因此 `nva_print` 没有将测试代码包含在主仓库中，而是分离了出来，存储在此处。
这样分离的设计可能对您提交代码造成了一定的不便，请根据您的提交情况进行贡献。一般可以分为以下三种情况：
仅提交主仓库代码、仅提交测试仓库代码、同时提交主仓库代码和测试仓库代码。

无论是哪种情况，您都需要把主仓库与测试仓库都 fork 到您的 GitHub 账号下。

### 仅提交主仓库代码

### 仅提交测试仓库代码

### 同时提交主仓库代码和测试仓库代码

## 许可证

[MIT LICENSE](./LICENSE)
