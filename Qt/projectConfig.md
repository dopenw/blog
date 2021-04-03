# Qt Creator 的一些配置


<!-- @import "[TOC]" {cmd="toc" depthFrom=1 depthTo=6 orderedList=false} -->

<!-- code_chunk_output -->

- [Qt Creator 的一些配置](#qt-creator-的一些配置)
  - [.pro 工程文件的相关设置](#pro-工程文件的相关设置)

<!-- /code_chunk_output -->


* [Beautifying Source Code](https://doc.qt.io/qtcreator/creator-beautifier.html)

## .pro 工程文件的相关设置
* [Qt Creator, how do I add one c++ project to another?](https://stackoverflow.com/questions/16876690/qt-creator-how-do-i-add-one-c-project-to-another)
* 设置目标文件路径
```sh
DESTDIR = $${PWD}/相对目标路径
```
* 配置debug/release 输出的目标名
```sh
win32:CONFIG(release, debug|release):TARGET = 目标名
else:win32:CONFIG(debug, debug|release): TARGET = 目标名
```

---
- [上一级](README.md)
- 上一篇 -> [解决 qt 程序的依赖项](deployqt.md)
- 下一篇 -> [QML Tutorial](qml.md)
