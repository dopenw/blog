[TOC]

# QCef 简介

[QCefView](https://github.com/CefView/QCefView) 是一个 Qt Widget,可以无缝集成 [Chromium嵌入式框架](https://github.com/chromiumembedded/cef)。它使您能够在熟悉的 Qt 生态系统中利用 CEF 的强大功能构建应用程序。

和 QCefView , 你可以:

- 使用熟悉的 Qt 开发应用程序。
- 实现 Web（JavaScript）和 Native（C++) 组件之间的直接互操作性。

# 为什么选择 QCefView 而不是 Electron？

[QCefView](https://cefview.github.io/QCefView/class_q_cef_view.html) 和 Electron 有不同的用途并迎合不同的开发风格。以下是比较:

| 特点     | [QCefView](https://cefview.github.io/QCefView/class_q_cef_view.html) | Electron                                |
| -------- | ------------------------------------------------------------ | --------------------------------------- |
| 范围     | Qt UI 组件                                                   | 综合应用框架                            |
| 目标受众 | Native（C++) 开发人员                                        | 前端开发人员                            |
| 主要语言 | C++                                                          | JavaScript                              |
| 互操作性 | 直接、直接的 Web/Native 通信                                 | 需要插件进行本机集成                    |
| 用例     | 在本机应用程序中嵌入 Web UI                                  | 主要利用 Web 技术构建跨平台桌面应用程序 |



本质上:

- **[QCefView](https://cefview.github.io/QCefView/class_q_cef_view.html)** 是 Qt 框架中的一个组件,非常适合使用基于 Web 的 UI 元素增强本机应用程序。
- **Electron** 是一个使用 Web 技术构建跨平台桌面应用程序的完整框架。

# QCefView 的应用场景

- **多媒体播放器:** 利用网络技术实现丰富、动态的用户界面。
- **游戏平台/启动器:** 为本机游戏引擎创建视觉上吸引人且具有交互性的前端。
- **工具类应用程序:** 构建具有复杂 UI 的功能丰富的工具,这些工具将受益于基于 Web 的渲染。
- **自定义嵌入式浏览器（有限制）:** 嵌入对渲染过程具有高度控制的 Web 内容。

在这些上下文驱动的应用程序中,Web 前端技术非常适合显示具有引人入胜效果的列表、表格和复杂页面。[QCefView](https://cefview.github.io/QCefView/class_q_cef_view.html) 充当 WebApp 容器,允许您托管 Web UI,同时将硬核业务逻辑保留为本机组件。使用 [QCefView](https://cefview.github.io/QCefView/class_q_cef_view.html) 可无缝弥合Web和原生应用程序。

# 构建 QCefView

- Qt version 5.14.2

- github 地址 - [QCefView](https://github.com/CefView/QCefView)

- git clone QCefView
- 使用 git tag - v2025.11.14
- 运行 generate-win-x86.bat 后将会在 ${QCEF_PROJECT}/\.build/windows.x86 目录里面生成 vs 工程
- 使用 vs 打开工程，编译运行提示 “错误 error C2220: 警告被视为错误”，参考 [错误 error C2220: 警告被视为错误 - 没有生成“object”](https://blog.csdn.net/kangdi7547/article/details/81556992) 即可解决
- 构建并运行 QCefViewTest 工程即可运行 demo

本机的硬件环境是：

- Inter i7 - 12700KF
- AMD - 6500XT

运行效果如下：

![image-20251216130320922](D:\wokspace\blog\images\QCefView-20251216130320922.png)

运行资源消耗如图：

![image-20251216130912648](D:\wokspace\blog\images\QCefView-20251216130912648.png)

## 额外信息

libcef.dll 文件大小为 255 MB

将 QCefViewTest 相关打包压缩：

- 使用 7-Zip - 119 MB
- 使用 zip - 166 MB