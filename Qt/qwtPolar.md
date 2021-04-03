#QwtPolar 简单使用


<!-- @import "[TOC]" {cmd="toc" depthFrom=1 depthTo=6 orderedList=false} -->

<!-- code_chunk_output -->

- [QwtPolar 简介](#qwtpolar-简介)
- [下载和编译](#下载和编译)
- [添加环境变量](#添加环境变量)
- [构建 QwtPolar 应用](#构建-qwtpolar-应用)
- [demo 示例](#demo-示例)

<!-- /code_chunk_output -->

## QwtPolar 简介

[Qwt Polar User's Guide](https://qwtpolar.sourceforge.io/)

The QwtPolar library contains classes for displaying values on a polar coordinate system.

## 下载和编译
[Download QwtPolar](https://sourceforge.net/projects/qwtpolar/)

编译需要依赖 qwt,可参考 [qwt-简单使用](./qwt.md)

可在 qwtpolarconfig.pri 文件中更改安装路径：
## 添加环境变量
在环境变量 QMAKEFEATURES 后添加 `${QWT_POLAR_ROOT}\features`

## 构建 QwtPolar 应用
在自己新建的 qt 工程 ${QWT_PROJECT}.pro 文件最后，添加 

```sh
CONFIG+=qwt
CONFIG+=qwtpolar
```

## demo 示例
![](../images/qwtPolar_202010201509_1.png)
![](../images/qwtPolar_202010201509_2.png)



[上一篇](qwt.md)
[下一篇](useMFCMigrationFramework.md)
---
- [上一级](README.md)
- 上一篇 -> [qwt 简单使用](qwt.md)
- 下一篇 -> [编译 vs2012 qt 5.3.1 64 位版本](useCompiledQt.md)
