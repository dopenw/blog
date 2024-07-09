<!-- CMake 学习-->



<!-- @import "[TOC]" {cmd="toc" depthFrom=1 depthTo=6 orderedList=false} -->

<!-- code_chunk_output -->

- [CMake 简介](#cmake-简介)
  - [CMake 行为准则(Do's and Don'ts)](#cmake-行为准则dos-and-donts)
    - [CMake 应避免的行为](#cmake-应避免的行为)
    - [CMake 应遵守的规范](#cmake-应遵守的规范)
  - [CMake 各个版本添加的新特性](#cmake-各个版本添加的新特性)
- [链接](#链接)

<!-- /code_chunk_output -->


# CMake 简介 

CMake是一个跨平台开源工具家族，用于构建、测试和打包软件。CMake通过简单的平台无关且编译器无关的配置文件来控制软件的编译流程，并能够生成原生的Makefile和工作空间，以便用于用户所选择的编译环境。

请使用现代的 CMake 。是 CMake 3.4+ ，甚至是 CMake 3.21+ ！ 它简洁、强大、优雅，所以你能够花费你的大部分时间在编写代码上，而不是在一个不可读、不可维护的 Make （或 CMake 2） 文件上浪费时间。 并且 CMake 3.11+ 的构建速度应该也会更加的快！！！

为什么我需要一个好的构建系统？以下情况是否适用于你？
- 你想避免将路径硬编码
- 你需要在不止一台电脑上构建软件包
- 你想在项目中使用 CI（持续集成）
- 你需要支持不同的操作系统（甚至可能只是 Unix 的不同版本）
- 你想支持多个编译器
- 你想使用 IDE，但也许不总是使用
- 你想从逻辑上描述你的程序是如何结构的，而不是通过某些标志和命令
- 你想使用一个第三方库
- 你想使用工具，比如 Clang-Tidy，来帮助你编码
- 你想使用调试器来 Debug

如果是这样，你会从类似 CMake 的构建系统中受益。

## CMake 行为准则(Do's and Don'ts)

### CMake 应避免的行为
接下来的两个列表很大程度上基于优秀的 [gist Effective Modern CMake](https://gist.github.com/mbinna/c61dbb39bca0e4fb7d1f73b0d66a4fd1). 那个列表更长且更详细，也非常欢迎你去仔细阅读它。

- `不要使用具有全局作用域的函数`：这包含 link_directories、 include_libraries 等相似的函数。
- `不要添加非必要的 PUBLIC 要求`：你应该避免把一些不必要的东西强加给用户（-Wall）。相比于 PUBLIC，更应该把他们声明为 PRIVATE。
- `不要在file函数中添加 GLOB 文件`：如果不重新运行 CMake，Make 或者其他的工具将不会知道你是否添加了某个文件。值得注意的是，CMake 3.12 添加了一个 `CONFIGURE_DEPENDS` 标志能够使你更好的完成这件事。
- `将库直接链接到需要构建的目标上`：如果可以的话，总是显式的将库链接到目标上。
- `当链接库文件时，不要省略 PUBLIC 或 PRIVATE 关键字`：这将会导致后续所有的链接都是缺省的。

### CMake 应遵守的规范

- `把 CMake 程序视作代码`：它是代码。它应该和其他的代码一样，是整洁并且可读的。
- `建立目标的观念`：你的目标应该代表一系列的概念。为任何需要保持一致的东西指定一个 （导入型）INTERFACE 目标，然后每次都链接到该目标。
- `导出你的接口`：你的 CMake 项目应该可以直接构建或者安装。
- `为库书写一个 Config.cmake 文件`：这是库作者为支持客户的体验而应该做的。
- `声明一个 ALIAS 目标以保持使用的一致性`：使用 add_subdirectory 和 find_package 应该提供相同的目标和命名空间。
- `将常见的功能合并到有详细文档的函数或宏中`：函数往往是更好的选择。
- `使用小写的函数名`： CMake 的函数和宏的名字可以定义为大写或小写，但是一般都使用小写，变量名用大写。
- `使用 cmake_policy 和/或 限定版本号范围`： 每次改变版本特性 (policy) 都要有据可依。应该只有不得不使用旧特性时才降低特性 (policy) 版本。

## CMake 各个版本添加的新特性

详见 [CMake 各个版本添加的新特性](https://modern-cmake-cn.github.io/Modern-CMake-zh_CN/chapters/intro/newcmake.html)

- CMake 3.0：接口库
- CMake 3.1：支持 C++11 和编译特性
- CMake 3.2：UTF8
- CMake 3.3：if 中添加 IN_LIST
- CMake 3.4：Swift & CCache
- CMake 3.5：ARM
- CMake 3.6：Clang-Tidy
- CMake 3.7：Android & CMake 的服务器模式
- CMake 3.8：C# & CUDA
- CMake 3.9：IPO
- CMake 3.10：CppCheck
- CMake 3.11：更快 & IMPORTED INTERFACE
- `CMake 3.12：版本范围和CONFIGURE_DEPENDS` --- 非常牛的版本，包含了许多长期要求添加的小功能。
- CMake 3.13：连接控制
- CMake 3.14：文件工具 (AKA CMake π)
- CMake 3.15：升级CLI
- CMake 3.16：统一构建
- CMake 3.17：原生支持 CUDA
- CMake 3.18：CUDA 与Clang & CMake宏特性
- CMake 3.19：预设
- CMake 3.20：文档
- CMake 3.21：配色
- CMake 3.22：方便的环境变量
- CMake 3.23：纯头文件库
- `CMake 3.24：包查找器` --- 一个很棒的版本。软件包编写者正在实现find_package和FetchContent的集成，这可以完成“丢失时下载”的工作，并且可以由软件包编写者配置。
- CMake 3.25：块作用域和 SYSTEM
- ...





# 链接
- [Modern CMake 简体中文版](https://modern-cmake-cn.github.io/Modern-CMake-zh_CN/)


---