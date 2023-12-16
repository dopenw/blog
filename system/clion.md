<!-- clion 开发环境配置 -->

# 基础

- [QT - Clion配置qt creator和uic_zju_cxl的博客-CSDN博客](https://blog.csdn.net/hrbust_cxl/article/details/121369515)
- [BashSupport Pro - IntelliJ IDEs Plugin | Marketplace](https://plugins.jetbrains.com/plugin/13841-bashsupport-pro)
- [Shell Script - IntelliJ IDEs Plugin | Marketplace](https://plugins.jetbrains.com/plugin/13122-shell-script)
- [Qt projects | CLion](https://www.jetbrains.com/help/clion/qt-tutorial.html)

# 调试时无法显示qt 变量值

解决方法：

参考 msvc debug renderes [Qt projects | CLion](https://www.jetbrains.com/help/clion/qt-tutorial.html#debug-renderers)，不同的是不使用里面的 qt5.natvis 文件，改为使用 qt 官方出的 qt5.natvis 文件（可使用 everything 搜索 qt5.natvis 文件，如果没有找到则安装 qt vs tool 工具）

该文件前缀是这样的：

```xml
<?xml version="1.0" encoding="utf-8"?>
<!--
    ****************************************************************************
    **
    ** Copyright (C) 2016 The Qt Company Ltd.
    ** Contact: https://www.qt.io/licensing/
    **
    ** This file is part of the Qt VS Tools.
    **
    ** $QT_BEGIN_LICENSE:GPL-EXCEPT$
    ** Commercial License Usage
    ** Licensees holding valid commercial Qt licenses may use this file in
    ** accordance with the commercial license agreement provided with the
    ** Software or, alternatively, in accordance with the terms contained in
    ** a written agreement between you and The Qt Company. For licensing terms
    ** and conditions see https://www.qt.io/terms-conditions. For further
    ** information use the contact form at https://www.qt.io/contact-us.
    **
    ** GNU General Public License Usage
    ** Alternatively, this file may be used under the terms of the GNU
    ** General Public License version 3 as published by the Free Software
    ** Foundation with exceptions as appearing in the file LICENSE.GPL3-EXCEPT
    ** included in the packaging of this file. Please review the following
    ** information to ensure the GNU General Public License requirements will
    ** be met: https://www.gnu.org/licenses/gpl-3.0.html.
    **
    ** $QT_END_LICENSE$
    **
    ****************************************************************************
-->
```

# 禁用windows下CLion命令行内自动换行

- [禁用windows下CLion&IDEA等JetBrains软件命令行内自动换行 - 传说哥 - 博客园](https://www.cnblogs.com/liux-pro/p/16933357.html)

参考链接：

- [https://blog.csdn.net/libaineu2004/article/details/89503132?utm_medium=distribute.pc_relevant.none-task-blog-2~default~baidujs_baidulandingword~default-4-89503132-blog-122040348.pc_relevant_3mothn_strategy_and_data_recovery&spm=1001.2101.3001.4242.3&utm_relevant_index=7](https://blog.csdn.net/libaineu2004/article/details/89503132?utm_medium=distribute.pc_relevant.none-task-blog-2~default~baidujs_baidulandingword~default-4-89503132-blog-122040348.pc_relevant_3mothn_strategy_and_data_recovery&spm=1001.2101.3001.4242.3&utm_relevant_index=7)

---

- [上一级](README.md)
- 上一篇 -> [C++ VS Java](c++VSjava.md)
- 下一篇 -> [开发者测试](developTest.md)
