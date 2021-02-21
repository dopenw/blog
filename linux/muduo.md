# 开源库 muduo 学习笔记


<!-- @import "[TOC]" {cmd="toc" depthFrom=1 depthTo=6 orderedList=false} -->
<!-- code_chunk_output -->

* [开源库 muduo 学习笔记](#开源库-muduo-学习笔记)
	* [muduo 简介](#muduo-简介)
	* [编译 muduo](#编译-muduo)
	* [将 muduo 导入 eclipse](#将-muduo-导入-eclipse)
	* [参考链接](#参考链接)

<!-- /code_chunk_output -->
## muduo 简介
[muduo](https://github.com/chenshuo/muduo) 是一个基于 Reactor 模式的现代 C++ 网络库，它采用非阻塞 IO 模型，基于事件驱动和回调，原生支持多核多线程，适合编写 Linux 服务端多线程网络应用程序。

## 编译 muduo
在 fedora28 环境下，出现无法成功编译的情况：
解决方法如下：
1. 在 bulid.sh 中添加如下选项，使用自定义boost库
```sh
# 经过测试，目前 boost 1.68 可以正常使用
TARGET=$HOME"/boost168"
cmake \
-DBoost_NO_BOOST_CMAKE=TRUE \
-DBoost_NO_SYSTEM_PATHS=TRUE \
-DBOOST_ROOT=$TARGET\
-DBoost_LIBRARY_DIRS:FILEPATH=${TARGET}/lib\
-DBoost_INCLUDE_DIR=${TARGET}/include\
```
2. 修改 CMakeList.txt:
```sh
# 在 set(CXX_FLAGS )中去掉一些编译器附加选项
# 添加：
add_definitions(-DBOOST_ERROR_CODE_HEADER_ONLY)
```
3. 修改代码
```sh
#编译过程中提示
no matching function for call to 'hash_value'
# 根据搜索到的提示，将报错的地方更改为 std library 即可解决；
```

## 将 muduo 导入 eclipse
1. 打开 eclipse
2. File -> New -> MakeFile Project With Existing Code
3. 找到 muduo 路径


## 参考链接
* [开源访谈】Muduo 作者陈硕访谈实录](https://www.oschina.net/question/28_61182)
* [C++ Boost: undefined reference to boost::system::generic_category()](https://stackoverflow.com/questions/13467072/c-boost-undefined-reference-to-boostsystemgeneric-category/13468280)
* [How can I get CMake to find my alternative Boost installation?](https://stackoverflow.com/questions/3016448/how-can-i-get-cmake-to-find-my-alternative-boost-installation)
* [no matching function for call to 'hash_value' while compiling program with clang on osx](https://svn.boost.org/trac10/ticket/13501#no1)
* [How to build Boost on Linux](https://codeyarns.com/2017/01/24/how-to-build-boost-on-linux/)
- [上一级](README.md)
- 上一篇 -> [linux 下遇到的一些问题、](linux.md)
- 下一篇 -> [进程相关](process.md)
