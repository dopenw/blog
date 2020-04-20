# 解决 qt 程序的依赖项


<!-- @import "[TOC]" {cmd="toc" depthFrom=1 depthTo=6 orderedList=false} -->
<!-- code_chunk_output -->

- [解决 qt 程序的依赖项](#解决-qt-程序的依赖项)
  - [Windows](#windows)
  - [Linux](#linux)
  - [Link](#link)

<!-- /code_chunk_output -->


## Windows

可以使用 qt 提供的 [windeployqt](https://doc.qt.io/qt-5/windows-deployment.html#windeployqt) 的工具。

如果还是缺少第三方的 依赖，可以借助 [Process Explorer](https://docs.microsoft.com/zh-tw/sysinternals/downloads/process-explorer) 工具查找 dll 正确的路径。使用方法在
[QT example executables won't run, missing QT5Cored.dll](https://stackoverflow.com/questions/28732602/qt-example-executables-wont-run-missing-qt5cored-dll) 中有介绍。

## Linux

[Qt for Linux/X11 - Deployment](https://doc.qt.io/qt-5/linux-deployment.html)

## Link
* [Application failed to start because it could not find or load the QT platform plugin “windows”](https://stackoverflow.com/questions/21268558/application-failed-to-start-because-it-could-not-find-or-load-the-qt-platform-pl/22982039)
* [QT example executables won't run, missing QT5Cored.dll](https://stackoverflow.com/questions/28732602/qt-example-executables-wont-run-missing-qt5cored-dll)

[上一级](README.md)
[上一篇](21_createPlugin.md)
