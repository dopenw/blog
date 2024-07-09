<!-- CMake 学习-->

<!-- @import "[TOC]" {cmd="toc" depthFrom=1 depthTo=6 orderedList=false} -->

<!-- code_chunk_output -->

- [CMake 简介](#cmake-简介)
  - [CMake 行为准则(Do's and Don'ts)](#cmake-行为准则dos-and-donts)
    - [CMake 应避免的行为](#cmake-应避免的行为)
    - [CMake 应遵守的规范](#cmake-应遵守的规范)
  - [CMake 各个版本添加的新特性](#cmake-各个版本添加的新特性)
- [cmake 基本使用](#cmake-基本使用)
- [为 CMake 项目添加特性](#为-cmake-项目添加特性)
  - [一些实用的工具](#一些实用的工具)
    - [CCache](#ccache)
    - [一些实用工具](#一些实用工具)
    - [Clang tidy](#clang-tidy)
    - [Include what you use](#include-what-you-use)
    - [Link what you use](#link-what-you-use)
    - [Clang-format](#clang-format)
  - [CMake 中一些有用的模组](#cmake-中一些有用的模组)
    - [CMakeDependentOption](#cmakedependentoption)
    - [CMakePrintHelpers](#cmakeprinthelpers)
    - [CheckCXXCompilerFlag](#checkcxxcompilerflag)
    - [try_compile/try_run](#try_compiletry_run)
    - [FeatureSummary](#featuresummary)
  - [CMake 对 IDE 的支持](#cmake-对-ide-的支持)
    - [用文件夹来组织目标 (target)](#用文件夹来组织目标-target)
    - [用文件夹来组织文件](#用文件夹来组织文件)
    - [在 IDE 中运行CMake](#在-ide-中运行cmake)
- [链接](#链接)

<!-- /code_chunk_output -->

备注： 本文主要为 [Modern CMake 简体中文版](https://modern-cmake-cn.github.io/Modern-CMake-zh_CN/) 学习笔记

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

# cmake 基本使用

- [cmake-examples](https://github.com/ttroy50/cmake-examples)

# 为 CMake 项目添加特性

默认的构建类型
CMake 通常会设置一个 “既不是 Release 也不是Debug” 的空构建类型来作为默认的构建类型，如果你想要自己设置默认的构建类型，你可以参考 Kitware blog 中指出的方法。

```cmake
set(default_build_type "Release")
if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
  message(STATUS "Setting build type to '${default_build_type}' as none was specified.")
  set(CMAKE_BUILD_TYPE "${default_build_type}" CACHE
      STRING "Choose the type of build." FORCE)
  # Set the possible values of build type for cmake-gui
  set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS
    "Debug" "Release" "MinSizeRel" "RelWithDebInfo")
endif()
```

## 一些实用的工具

CCache 和一些其他的实用工具
在过去的一些版本中，一些能够帮助你写好代码的实用工具已经被添加到了 CMake 中。往往是通过为目标指定属性，或是设定形如 CMAKE_* 的初始化变量的值的形式启用相应工具。这个启用的规则不只是对某个特定的工具（program）起作用，一些行为相似的工具都符合此规则。

当需要启用多个工具时，所有的这些变量都通过 ; 分隔（CMake 中列表的分隔标准）来描述你在目标源程序上需要使用的工具( program) 以及选项。

### CCache

- Ccache（或 ”ccache“）是一个编译器缓存。它通过缓存之前的编译文件并且利用之前已经完成的编译过程来加速重编译)。Ccache是一个免费的软件，基于 GNU General Public License version 3 或之后更新的许可协议发布。可以查看这里的 许可协议页面 。

通过设置变量 `CMAKE_<LANG>_COMPILER_LAUNCHER` 或设置目标的 `<LANG>_COMPILER_LAUNCHER` 属性来使用一些像 CCache 的方式来“封装”目标的编译。在 CMake 的最新版本中拓展了对 CCache 的支持。在使用时，可以这么写：

```cmake
find_program(CCACHE_PROGRAM ccache)
if(CCACHE_PROGRAM)
    set(CMAKE_CXX_COMPILER_LAUNCHER "${CCACHE_PROGRAM}")
    set(CMAKE_CUDA_COMPILER_LAUNCHER "${CCACHE_PROGRAM}") # CMake 3.9+
endif()
```

### 一些实用工具

设置以下属性或是在命令行中设置以 CMAKE_* 为起始的变量来启动这些功能。它们大部分只在 make 或 ninja 生成器生成 C 和 CXX 项目时起作用。

- <LANG>_CLANG_TIDY: CMake 3.6+
- <LANG>_CPPCHECK
- <LANG>_CPPLINT
- <LANG>_INCLUDE_WHAT_YOU_USE

### Clang tidy

- clang-tidy 是一个基于 clang 的 C++ 代码分析工具。它意图提供一个可扩展的框架，用于诊断和修复典型的编程错误，如样式违规、接口误用、或通过静态分析推断出的错误。clang-tidy 是一个模块化的程序，为编写新的检查规则提供了方便的接口。

这是在命令行中运行 clang-tidy 的方法，使用的是一个列表（记住，用分号分隔的字符串是一个列表）。

这是一个使用 Clang-Tidy 的简单例子：

```sh
~/package # cmake -S . -B build-tidy -DCMAKE_CXX_CLANG_TIDY="$(which clang-tidy);-fix"
~/package # cmake --build build -j 1
```

这里的 -fix 部分是可选的，将会修改你的源文件来尝试修复 clang-tidy 警告 (warning) 的问题。如果你在一个 git 仓库中工作的话，使用 -fix 是相当安全的，因为你可以看到代码中哪部分被改变了。不过，请确保不要同时运行你的 makefile/ninja 来进行构建！如果它尝试修复一个相同的头文件两次，可能会出现预期外的错误。

如果你想明确的使用目标的形式来确保自己对某些特定的目标调用了 clang-tidy，为可以设置一个变量（例如像 DO_CLANG_TIDY，而不是名为 CMAKE_CXX_CLANG_TIDY 的变量），然后在创建目标时，将它添加为目标的属性。你可以通过以下方式找到路径中的 clang-tidy：

```cmake
find_program(
    CLANG_TIDY_EXE
    NAMES "clang-tidy"
    DOC "Path to clang-tidy executable"
)
```

### Include what you use

- 一个与 clang 一起使用，用于分析 C 和 C++ 源文件中 #include 的工具

这是一个使用 `include what you use` 的例子。首先，你需要确保系统中有这个工具，例如在一个 docker 容器中或者通过 macOS 上的 brew 利用 brew install include-what-you-use 来安装它。然后，你可以通过此方式使用此工具，而不需要修改你的源代码：

```sh
~/package # cmake -S . -B build-iwyu -DCMAKE_CXX_INCLUDE_WHAT_YOU_USE=include-what-you-use
```

最后，你可以重定向输出到文件，然后选择是否应用此修复：

```sh
~/package # cmake --build build-iwyu 2> iwyu.out
~/package # fix_includes.py < iwyu.out
```

（你应该先检查一下这些修复的正确性，或者在修复后对代码进行润色！）

### Link what you use

- 这是一个布尔类型的目标属性，`LINK_WHAT_YOU_USE` ，它将会在链接时检查与目标不相干的文件。

### Clang-format

- ClangFormat 描述了一套建立在 LibFormat 之上的工具。它可以以各种方式支持你的工作流程，包括独立的工具和编辑器的集成。

不幸的是，Clang-format 并没有真正的与 CMake 集成。你可以制作一个自定义的目标（参考 这篇文章，或者你可以尝试自己手动的去运行它。）一个有趣的项目/想法 在这里，不过我还没有亲自尝试过。它添加了一个格式化 (format) 的目标，并且你甚至没法提交没有格式化过的文件。

下面的两行可以在一个 git 仓库中，在 bash 中使用 clang-format 工具（假设你有一个 .clang-format 文件）：

```sh
gitbook $ git ls-files -- '*.cpp' '*.h' | xargs clang-format -i -style=file
gitbook $ git diff --exit-code --color
```

## CMake 中一些有用的模组

在 CMake 的 [modules](https://cmake.org/cmake/help/latest/manual/cmake-modules.7.html) 集合了很多有用的模组，但是有一些模块相比于其他的更有用。以下是一些比较出彩的：

### CMakeDependentOption

这增加了命令 `cmake_dependent_option` ，它根据另外一组变量是否为真来（决定是否）开启一个选项。下面是一个例子：

```cmake
include(CMakeDependentOption)
cmake_dependent_option(BUILD_TESTS "Build your tests" ON "VAL1;VAL2" OFF)
```

如上代码是下面的一个缩写：

```cmake
if(VAL1 AND VAL2)
    set(BUILD_TESTS_DEFAULT ON)
else()
    set(BUILD_TESTS_DEFAULT OFF)
endif()

option(BUILD_TESTS "Build your tests" ${BUILD_TESTS_DEFAULT})

if(NOT BUILD_TESTS_DEFAULT)
    mark_as_advanced(BUILD_TESTS)
endif()
```

需要注意的是，如果你使用了 `include(CTest)` ，用 `BUILD_TESTING` 来检测是否启用是更好的方式，因为它就是为此功能而生的。这里只是一个 CMakeDependentOption 的例子。

### CMakePrintHelpers

这个模块包含了几个方便的输出函数。`cmake_print_properties` 可以让你轻松的打印属性，而 `cmake_print_variables` 将打印出你给它任意变量的名称和值。

### CheckCXXCompilerFlag

这个模块允许你检查编译器是否支持某个标志，例如：

```cmake
include(CheckCXXCompilerFlag)
check_cxx_compiler_flag(-someflag OUTPUT_VARIABLE)
```

需要注意的是 `OUTPUT_VARIABLE` 也会出现在打印的配置输出中，所以请选个不错的变量名。

这只是许多类似模块中的一个，例如 `CheckIncludeFileCXX` 、 `CheckStructHasMember` 、 `TestBigEndian` 以及 `CheckTypeSize` ，它们允许你检查系统的信息（并且你可以在代码中使用这些信息）。

### try_compile/try_run

准确的说，这不是一个模块，但是它们对上述列出的许多模块至关重要。通过它你可以在配置时尝试编译（也可能是运行）一部分代码。这可以让你在配置时获取关于系统能力的信息。基本的语法如下：

```cmake
try_compile(
  RESULT_VAR
    bindir
  SOURCES
    source.cpp
)
```

这里有很多可以添加的选项，例如 `COMPILE_DEFINITIONS` 。在 CMake 3.8+ 中， 这将默认遵循 CMake 中 C/C++/CUDA 的标准设置。如果你使用的是 try_run 而不是 try_compile，它将运行生成的程序并将运行结果存储在 `RUN_OUTPUT_VARIABLE` 中。

### FeatureSummary

这是一个十分有用但是也有些奇怪的模块。它能够让你打印出找到的所有软件包以及你明确设定的所有选项。它和 find_package 有一些联系。像其他模块一样，你首先要包括模块：

```cmake
include(FeatureSummary)
```

然后，对于任何你已经运行或者将要运行的 `find_package` ，你可以这样拓展它的默认信息：

```cmake
set_package_properties(OpenMP PROPERTIES
    URL "http://www.openmp.org"
    DESCRIPTION "Parallel compiler directives"
    PURPOSE "This is what it does in my package")
```

你也可以将包的 `TYPE` 设置为 `RUNTIME` `、OPTIONAL` `、RECOMMENDED` 或者 `REQUIRED` 。但是你不能降低包的类型，如果你已经通过 `find_package` 添加了一个 `REQUIRED` 类型的包，你将会看到你不能改变它的 `TYPE` ：

并且，你可以添加任何选项让其成为 f`eature summary` 的一部分。如果你添加的选项名与包的名字一样，他们之间会互相产生影响：

```cmake
add_feature_info(WITH_OPENMP OpenMP_CXX_FOUND "OpenMP (Thread safe FCNs only)")
```

然后，你可以将所有特性 (features) 的集合打印到屏幕或日志文件中：

```cmake
if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)
    feature_summary(WHAT ENABLED_FEATURES DISABLED_FEATURES PACKAGES_FOUND)
    feature_summary(FILENAME ${CMAKE_CURRENT_BINARY_DIR}/features.log WHAT ALL)
endif()
```

你可以建立一个 WHAT 目标来集合任何你想查看的特性 (features)，或者直接使用 ALL 目标也行。

## CMake 对 IDE 的支持

一般来说，IDE 已经被标准的 CMake 的项目支持。不过这里有一些额外的东西可以帮助 IDE 表现得更好：

### 用文件夹来组织目标 (target)

一些 IDE，例如 Xcode，支持文件夹。你需要手动的设定 USE_FOLDERS 这个全局属性来允许 CMake 使用文件夹组织你的文件：

```cmake
set_property(GLOBAL PROPERTY USE_FOLDERS ON)
```

然后，你可以在创建目标后，为目标添加文件夹属性，即将其目标 MyFile 归入到 Scripts 文件夹中：

```cmake
set_property(TARGET MyFile PROPERTY FOLDER "Scripts")
```

文件夹可以使用 / 进行嵌套。

你可以使用正则表达式或在 `source_group` 使用列表来控制文件在文件夹中是否可见。

### 用文件夹来组织文件

你也可以控制文件夹对目标是否可见。有两种方式，都是使用 `source_group` 命令，传统的方式是：

```cmake
source_group("Source Files\\New Directory" REGULAR_EXPRESSION ".*\\.c[ucp]p?")
```

你可以用 `FILES` 来明确的列出文件列表，或者使用 REGULAR_EXPRESSION 来进行筛选。通过这个方式你可以完全的掌控文件夹的结构。不过，如果你的文件已经在硬盘中组织的很好，你可能只是想在 CMake 中复现这种组织。在 CMake 3.8+ 中，你可以用新版的 `source_group` 命令非常容易的做到上述情形：

```cmake
source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}/base/dir" PREFIX "Header Files" FILES ${FILE_LIST})
```

对于 `TREE` 选项，通常应该给出一个以 `${CMAKE_CURRENT_SOURCE_DIR}` 起始的完整路径（因为此命令的文件解析路径是相对于构建目录的）。这个 `PREFIX` 设置文件将在 IDE 结构中的位置，而 `FILES` 选项是包含一些文件的列表 (FILE_LIST)。CMake 将会解析 `TREE` 路径下 `FILE_LIST` 中包含的文件，并将每个文件添加到 `PREFIX` 结构下，这构成了 IDE 的文件夹结构。

注意：如果你需要支持低于 3.8 版本的CMake，我不建议你使用上述命令，只建议在 CMake 3.8+ 中使用上述文件夹布局。

### 在 IDE 中运行CMake

要使用 IDE，如果 CMake 可以生成对应 IDE 的文件（例如 Xcode，Visual Studio），可以通过 `-G"name of IDE"` 来完成，或者如果 IDE 已经内置了对 CMake 的支持（例如 CLion，QtCreator 和一些其他的 IDE），你可以直接在 IDE 中打开 `CMakeLists.txt` 来运行 CMake。

# 链接

- [Modern CMake 简体中文版](https://modern-cmake-cn.github.io/Modern-CMake-zh_CN/)

---

- [上一级](README.md)
- 上一篇 -> [Clock 和 Timer](chrono.md)
- 下一篇 -> [判断浮点数是否相等](compareDoubleFloat.md)
