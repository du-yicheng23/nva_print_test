# nva_print_test

[en_US](./README.md) | zh_CN

---

这是是项目 [nva_print](https://github.com/du-yicheng23/nva_print) 的测试仓库，
用于存储并运行 `nva_print` 的测试代码和测试用例。

## 构建并运行测试

目前有三种测试路径：`build_test`、`unit_test` 和 `mcu_test`。

除了 `mcu_test`，目前仅支持使用 CMake 进行构建和测试。
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
路径内的 `README.md` 文件以获取构建并运行的方式。

## 贡献指南

欢迎您为 `nva_print` 与 `nva_print_test` 提交 PR 或 Issue，帮助我们改进和完善这个项目。

考虑到测试的内容较多，因此 `nva_print` 没有将测试代码包含在主仓库中，而是分离了出来，存储在此处。
这样分离的设计可能对您提交代码造成了一定的不便，请根据您的提交情况进行贡献。一般可以分为以下三种情况：
仅提交主仓库代码、仅提交测试仓库代码、同时提交主仓库代码和测试仓库代码。

无论是哪种情况，您都需要把主仓库与测试仓库都 fork 到您的 GitHub 账号下， 然后将您的 fork 仓库（包含主仓库和测试仓库）分别克隆到本地。

接着，将您测试仓库的 submodule 指向您的 fork 的主仓库。
```bash
cd nva_print
git remote set-url origin <your fork url>
```

接下来的操作根据您的提交情况进行选择：

### 仅提交主仓库代码

在测试仓库完成测试后，将主仓库的代码提交到您的 fork 的主仓库中，然后在 GitHub 上发起 PR。

### 仅提交测试仓库代码

在测试仓库完成测试后，将测试仓库的代码提交到您的 fork 的测试仓库中，然后在 GitHub 上发起 PR。

### 同时提交主仓库代码和测试仓库代码

在测试仓库完成测试后，分别将主仓库与测试仓库的代码提交到您 fork 的主仓库和测试仓库中，
然后先在主仓库发起 PR1，然后在测试仓库发起 PR2，并在测试仓库的 PR2 添加 PR1 的链接以便于维护者维护。

维护者注意：对于这种情况，需要先合并主仓库的 PR1，然后把测试仓库的 PR2 的 submodule 指针更新为主仓库的最新提交之后再合并 PR2。

## 许可证

[MIT LICENSE](./LICENSE)
