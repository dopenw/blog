# 解决 qt 程序的依赖项


<!-- @import "[TOC]" {cmd="toc" depthFrom=1 depthTo=6 orderedList=false} -->
<!-- code_chunk_output -->

- [解决 qt 程序的依赖项](#解决-qt-程序的依赖项)
  - [Windows](#windows)
  - [Linux](#linux)
  - [Link](#link)

<!-- /code_chunk_output -->


## Windows

### Qt5
可以使用 qt 提供的 [windeployqt](https://doc.qt.io/qt-5/windows-deployment.html#windeployqt) 的工具。

如果还是缺少第三方的 依赖，可以借助 [Process Explorer](https://docs.microsoft.com/zh-tw/sysinternals/downloads/process-explorer) 工具查找 dll 正确的路径。使用方法在
[QT example executables won't run, missing QT5Cored.dll](https://stackoverflow.com/questions/28732602/qt-example-executables-wont-run-missing-qt5cored-dll) 中有介绍。

### Qt4
[Deploying an Application on Windows](https://doc.qt.io/archives/qt-4.8/deployment-windows.html)

* 借助 Process Explorer 找到缺失的动态库
* 设置自定义 plugin 路径 [QApplication::addLibraryPath()](https://doc.qt.io/archives/qt-4.8/qcoreapplication.html#addLibraryPath) or [QApplication::setLibraryPaths()](https://doc.qt.io/archives/qt-4.8/qcoreapplication.html#setLibraryPaths).
```c++
qApp->addLibraryPath("C:/customPath/plugins");
```

注意：可以使用 qt.conf 文件（放置在 Qt 应用程序相同文件夹内）设置插件目录的路径。但是 qt.conf 内必须使用绝对路径，这不太容易确定。

## Linux

[Qt for Linux/X11 - Deployment](https://doc.qt.io/qt-5/linux-deployment.html)

## Link
* [Application failed to start because it could not find or load the QT platform plugin “windows”](https://stackoverflow.com/questions/21268558/application-failed-to-start-because-it-could-not-find-or-load-the-qt-platform-pl/22982039)
* [QT example executables won't run, missing QT5Cored.dll](https://stackoverflow.com/questions/28732602/qt-example-executables-wont-run-missing-qt5cored-dll)
* [设置qt插件路径](https://www.cnblogs.com/findumars/p/6353066.html)

[上一级](README.md)
[上一篇](QtCreatorTips.md)
[下一篇](projectConfig.md)
