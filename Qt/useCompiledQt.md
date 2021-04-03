#编译 vs2012 qt 5.3.1 64 位版本

1. 安装 vs2012 
2. 编译 qt,打开 Visual studio 2012 x64命令提示 (XX) 工具
```sh
configure -developer-build -opensource -confirm-license -mp -nomake examples -nomake tests -debug-and-release -c++11 -no-warnings-are-errors -prefix D:\qt5.3.1\msvc2012_x64
# Build with Multiple Processes
set CL=/MP
nmake
nmake install
```
3. 安装 qt-vs-addin-1.2.5
4. 添加 环境变量
```sh
# 示例
QTDIR = "D:\qt5.3.1\msvc2012_x64"
QT_QPA_PLATFORM_PLUGIN_PATH = "D:\qt5.3.1\msvc2012_x64"
Path 添加 "D:\qt5.3.1\msvc2012_x64"
```


## Link 
* [Qt 5.1.1: Application failed to start because platform plugin “windows” is missing](https://stackoverflow.com/questions/20495620/qt-5-1-1-application-failed-to-start-because-platform-plugin-windows-is-missi)
* [Qt没有被正确安装，请运行make install -- 问题的解决办法](https://blog.csdn.net/libaineu2004/article/details/104765146)
* [Building Qt5 with Visual Studio 2012 / Visual Studio 2013, and integrating with the IDE](https://stackoverflow.com/questions/15826893/building-qt5-with-visual-studio-2012-visual-studio-2013-and-integrating-with)
* [How do I utilise all the cores for nmake?](https://stackoverflow.com/questions/601970/how-do-i-utilise-all-the-cores-for-nmake)
* [Qt Creator无法debug，报错：The selected debugger may be inappropriate for the inferior. Examining symbols and setting breakpoints by file name and line number may fail. The inferior is in the Portable ...](https://www.cnblogs.com/Java-Starter/p/9915037.html)
* [How to build x86 and/or x64 on Windows from command line with CMAKE?](https://stackoverflow.com/questions/28350214/how-to-build-x86-and-or-x64-on-windows-from-command-line-with-cmake)


---
- [上一级](README.md)
- 上一篇 -> [QwtPolar 简单使用](qwtPolar.md)
- 下一篇 -> [使用 MFCMigrationFramework](useMFCMigrationFramework.md)
