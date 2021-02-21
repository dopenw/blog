# google protobuf 的简单使用


<!-- @import "[TOC]" {cmd="toc" depthFrom=1 depthTo=6 orderedList=false} -->
<!-- code_chunk_output -->

- [google protobuf 的简单使用](#google-protobuf-的简单使用)
  - [简要概述](#简要概述)
  - [安装 google protobuf](#安装-google-protobuf)
    - [linux 下安装 protobuf](#linux-下安装-protobuf)
    - [windows 下编译安装库及生成 Protobuf Compiling](#windows-下编译安装库及生成-protobuf-compiling)
  - [create Visual Studio 2015 solution file](#create-visual-studio-2015-solution-file)
  - [create visual studio 2015 MD/MDd](#create-visual-studio-2015-mdmdd)
  - [Compiling Your Protocol Buffers](#compiling-your-protocol-buffers)
  - [使用 protobuf](#使用-protobuf)
    - [在 linux 下使用 protobuf](#在-linux-下使用-protobuf)
    - [在 VS2015 下使用 protobuf](#在-vs2015-下使用-protobuf)
  - [Language Guide](#language-guide)
  - [链接](#链接)

<!-- /code_chunk_output -->

## 简要概述

[protobuf](https://developers.google.com/protocol-buffers/) 全称为google protocol buffer，是google内部使用的一种数据交换格式，后来开源出来了。protobuf是一种语言无关、平台无关、扩展性好的用于通信协议、数据存储的结构化数据串行化方法。很像XML、JSON等，不过它更小、更快、也更简单。你可以定义自己的数据结构，然后使用代码生成器生成的代码来读写这个数据结构。

## 安装 google protobuf

[Download protobuf](https://github.com/google/protobuf/releases/tag/v3.5.1)

### linux 下安装 protobuf

```sh
dnf install protobuf
dnf install protobuf-compiler
```

### windows 下编译安装库及生成 Protobuf Compiling

[ build protobuf with MSVC on Windows](https://github.com/google/protobuf/blob/master/cmake/README.md)

注：此处最好使用 [releases](https://github.com/google/protobuf/releases) 页面的源代码进行编译

在这里我使用的是编译为 release 版：
```sh
C:\Path\to\protobuf\cmake\build>mkdir release & cd release
C:\Path\to\protobuf\cmake\build\release>cmake -G "NMake Makefiles" ^
-DCMAKE_BUILD_TYPE=Release ^
-DCMAKE_INSTALL_PREFIX=../../../../install ^
../..
```

遇到的问题：
```sh
-- The C compiler identification is MSVC 19.0.24215.1                             
-- The CXX compiler identification is MSVC 19.0.24215.1                           
-- Check for working C compiler: D:/VisualStudio2015/VC/bin/amd64/cl.exe           
-- Check for working C compiler: D:/VisualStudio2015/VC/bin/amd64/cl.exe
-- broken CMake Error at C:/Program Files/CMake/share/cmake-3.11/Modules/CMakeTestCCompiler.cmake:52 (message):                                                                   The C compiler                                                                                                                                                     
"D:/VisualStudio2015/VC/bin/amd64/cl.exe"  is not able to compile a simple test program.                                                                
It fails with the following output:                                                                                                                                     
Change Dir: D:/Lib/protobuf/cmake/build/release/CMakeFiles/CMakeTmp                                                                                                   
Run Build Command:"nmake" "/NOLOGO" "cmTC_33657\fast"                                  
D:\VisualStudio2015\VC\BIN\amd64\nmake.exe -f
CMakeFiles\cmTC_33657.dir\build.make /nologo -L                  
CMakeFiles\cmTC_33657.dir\build    Building C object
CMakeFiles/cmTC_33657.dir/testCCompiler.c.obj                        
D:\VisualStudio2015\VC\bin\amd64\cl.exe @C:\Users\dj803\AppData\Local\Temp\nm3D72.tmp                                                                             
testCCompiler.c    Linking C executable cmTC_33657.exe                                                    
"C:\Program Files\CMake\bin\cmake.exe" -E vs_link_exe --intdir=CMakeFiles\cmTC_33657.dir --manifests  --
D:\VisualStudio2015\VC\bin\amd64\link.exe /nologo
@CMakeFiles\cmTC_33657.dir\objects1.rsp
@C:\Users\dj803\AppData\Local\Temp\nm3EFA.tmp     RC Pass 1: command
"rc /foCMakeFiles\cmTC_33657.dir/manifest.res
CMakeFiles\cmTC_33657.dir/manifest.rc" failed (exit code 0) with the
following output:               绯荤粺鎵句笉鍒版寚瀹氱殑鏂囦欢銆侼MAKE :
fatal error U1077: “"C:\Program Files\CMake\bin\cmake.exe"”: 返回代
码“0xffffffff”    Stop.                                                                    
NMAKE : fatal error U1077:
“D:\VisualStudio2015\VC\BIN\amd64\nmake.exe”: 返回代码“0x2”    
Stop.                

CMake will not be able to correctly generate this project.   
Call Stack (most recent call first):                                                 CMakeLists.txt:16 (project)                                                                                                                                                                                                               
-- Configuring incomplete, errors occurred!   See also
"D:/Lib/protobuf/cmake/build/release/CMakeFiles/CMakeOutput.log".  See also
"D:/Lib/protobuf/cmake/build/release/CMakeFiles/CMakeError.log".
```
解决办法：
```sh
set path=C:\Program Files (x86)\Windows Kits\10\bin\10.0.16299.0\x64;%path%
```

## create Visual Studio 2015 solution file
```sh
cd $probufDir/cmake
mkdir build
cd build 
mkdir solution 
cd solution
cmake -G "Visual Studio 14 2015" -DCMAKE_INSTALL_PREFIX=../../../../install ../..
```

## create visual studio 2015 MD/MDd
```sh
cd $probufDir/cmake
mkdir build
cd build 
mkdir solution1 
cd solution1
cmake -G "Visual Studio 14" -Dprotobuf_MSVC_STATIC_RUNTIME=OFF ../..
```

## Compiling Your Protocol Buffers
```sh
# 示例
.\protoc.exe --proto_path=E:\protobuf\demo -I=E:\protobuf\demo --cpp_out=E:\protobuf\demo E:\protobuf\demo\addressbook.proto 
```

## 使用 protobuf

[Protocol Buffer Basics: C++](https://developers.google.com/protocol-buffers/docs/cpptutorial)

### 在 linux 下使用 protobuf

使用示例：
```sh
g++ -lprotobuf -lpthread reader.cpp addressbook.pb.cc -o read
```

### 在 VS2015 下使用 protobuf
将上面编译后的 libprotobuf.lib 文件加入到工程中，并包含上面生成的 include 文件夹

## Language Guide 
* [proto2](https://developers.google.com/protocol-buffers/docs/proto)
* [proto3](https://developers.google.com/protocol-buffers/docs/proto3)

* [Youtube Link - Protocol Buffers (Protobuf) Tutorial Part 3: Creating an application using Visual Studio 2015](https://www.youtube.com/watch?v=x-u-hEFhDDo&feature=youtu.be)

## 链接
* [C++使用protobuf(Linux下)
](http://hahaya.github.io/use-protobuf-in-c-plus-plus/)
* [CMake生成Visual Studio Nmake文件时报错问题分析](http://www.tiger2doudou.com/blog/post/metorm/CMake%E7%94%9F%E6%88%90Visual-Studio-Nmake%E6%96%87%E4%BB%B6%E6%97%B6%E6%8A%A5%E9%94%99%E9%97%AE%E9%A2%98%E5%88%86%E6%9E%90)
* [error LNK2038: 检测到“RuntimeLibrary”的不匹配项: 值“MTd_StaticDebug”不匹配值“MDd_DynamicDebug”](https://blog.csdn.net/chan20/article/details/77040441)
* [error LNK2038: mismatch detected for '_ITERATOR_DEBUG_LEVEL': value '0' doesn't](http://huaxiamian.iteye.com/blog/1379287)
* [Protobuffs import from another directory](https://stackoverflow.com/questions/21159451/protobuffs-import-from-another-directory)
* [Statically link google protobuf lib into a dll library](https://stackoverflow.com/questions/47900268/statically-link-google-protobuf-lib-into-a-dll-library)

[上一级](README.md)
[上一篇 -> 函数参数入栈的顺序](function_arg_stack.md)
[下一篇 -> Google c++ test framework](google_test_framework.md)
