# qwt 简单使用


<!-- @import "[TOC]" {cmd="toc" depthFrom=1 depthTo=6 orderedList=false} -->

<!-- code_chunk_output -->

- [qwt 简单使用](#qwt-简单使用)
  - [qwt 简介](#qwt-简介)
  - [编译和安装](#编译和安装)
    - [Windows](#windows)
  - [创建环境变量](#创建环境变量)
  - [构建 qwt 应用](#构建-qwt-应用)
  - [Link](#link)

<!-- /code_chunk_output -->


## qwt 简介
[Qwt User's Guide ](https://qwt.sourceforge.io/index.html)

Qwt - Qt Widgets for Technical Applications
The Qwt library contains GUI Components and utility classes which are primarily useful for programs with a technical background. Beside a framework for 2D plots it provides scales, sliders, dials, compasses, thermometers, wheels and knobs to control or display values, arrays, or ranges of type double.

## 编译和安装
[compiled and installed](https://qwt.sourceforge.io/qwtinstall.html)

[Download qwt](https://sourceforge.net/projects/qwt/files/)

可在 qwtconfig.pri 文件中更改安装路径：

```sh
win32 {
    QWT_INSTALL_PREFIX    = C:/Qwt-$$QWT_VERSION # 更改为新的路径
    # QWT_INSTALL_PREFIX = C:/Qwt-$$QWT_VERSION-qt-$$QT_VERSION
}
```

### Windows 
Qt packages offer a command line interface, that can be found in the Qt application menu: f.e "All Programs -> Qt -> Command Prompt".

Creating a makefile usually means something like:

```sh
cd qwt-6.1.5
qmake qwt.pro
```

打开 Visual studio 命令提示 (XX) 工具

```sh
nmake
```

Finally you have to install everything below the directories you have specified in qwtconfig.pri.

```sh
nmake install
```

## 创建环境变量
新建环境变量 QMAKEFEATURES ；其值为安装目录下的 `${QWT_ROOT}\features`

## 构建 qwt 应用
在自己新建的 qt 工程 ${QWT_PROJECT}.pro 文件最后，添加 

```sh
CONFIG+=qwt
```

## Link 
* [QT 画图工具 QWT 和 QwtPolar 在 windows 和 linux 下的编译安装](https://blog.csdn.net/zhouguoqionghai/article/details/46517061)

---
- [上一级](README.md)
- 上一篇 -> [《Qml Book》 - JavaScript](qmlbook_16_javaScript.md)
- 下一篇 -> [QwtPolar 简单使用](qwtPolar.md)
