<!-- Qt 源码学习 -->

- [源码概览](#源码概览)
  - [Qt Base](#qt-base)
  - [Qt 3D](#qt-3d)
  - [Qt Active](#qt-active)
  - [Qt Android extras](#qt-android-extras)
  - [Qt Canvas 3D](#qt-canvas-3d)
  - [QtCharts](#qtcharts)
  - [Qt Connectivity](#qt-connectivity)
  - [Qt 3D data visualization framework](#qt-3d-data-visualization-framework)
  - [Qt Declarative (Quick 2)](#qt-declarative-quick-2)
  - [其他](#其他)
    - [Coin](#coin)
    - [gnuwin32](#gnuwin32)
  - [补充说明](#补充说明)
- [参考链接](#参考链接)


备注： 使用的源码版本为 [Qt 5.15.2](https://github.com/qt/qt5/tree/v5.15.2)

# 源码概览

## Qt Base

- [qt/qtbase](https://github.com/qt/qtbase/tree/40143c189b7c1bf3c2058b77d00ea5c4e3be8b28)

实现 Core, Gui, Widgets, Network, Concurrent, DBus, Sql, XML, TestLib, PrintSupport, Tools (moc ,rcc ,uic ,...) , Plugins, CoreLib, Angle, OpenGL, Android ... 

## Qt 3D 

- [qt/qt3d]((https://github.com/qt/qt3d/tree/34171b1d99f55fde1627df3c57eed50480ab2ae7)) 

Qt 3D 为近实时仿真系统提供功能，支持 Qt C++ 和 Qt Quick 应用程序中的 2D 和 3D 渲染。

## Qt Active

- [qt/qtactiveqt](https://github.com/qt/qtactiveqt) 

[The Active Qt framework](https://doc.qt.io/qt-5/activeqt.html) 是一个辅助框架，使开发人员能够访问和使用任何 ActiveX 服务器提供的 ActiveX 控件和 COM 对象。 它还可以使开发人员自己的 Qt 应用程序作为 Windows 上的 COM 服务器可用。

## Qt Android extras

- [qt/qtandroidextras](https://github.com/qt/qtandroidextras/tree/505be55243ba1600f62f47f9be3011a717841096)

[Qt Android Extras](https://doc.qt.io/qt-5/qtandroidextras-index.html) - Qt 的 Android 额外功能

## Qt Canvas 3D 

- [qt/qtcanvas3d](https://github.com/qt/qtcanvas3d/tree/1319e0965f6008f44f9216a7bb76e106a1710767)

## QtCharts

- [qt/qtcharts](https://github.com/qt/qtcharts/tree/a67f812548b008e3eedcd2bb9313828a195fd23b)

[Qt Charts](https://doc.qt.io/qt-5/qtcharts-index.html) 模块提供了一组易于使用的图表组件。 它使用 Qt 图形视图框架，因此可以轻松地将图表集成到现代用户界面中。 Qt 图表可以用作 QWidgets、QGraphicsWidget 或 QML 类型。 用户可以通过选择图表主题之一轻松创建令人印象深刻的图表。

## Qt Connectivity

- [qt/qtconnectivity](https://github.com/qt/qtconnectivity/tree/ca6cc606d9fc0947ea6c27738a1ca8f12f3258ea)

提供 蓝牙，NFC 通信功能。可参见 [Networking and Connectivity](https://doc.qt.io/qt-5/topics-network-connectivity.html) 相关章节

## Qt 3D data visualization framework

- [qt/qtdatavis3d](https://github.com/qt/qtdatavis3d/tree/1168c788a117e4556e6cd0ba1e267a86ef62b0c4)

[Qt 数据可视化模块](https://doc.qt.io/qt-5/qtdatavisualization-index.html) 提供了一种将 3D 数据可视化为条形图、散点图和曲面图的方法。 它对于可视化深度图和大量快速变化的数据（例如从多个传感器接收的数据）特别有用。 图表的外观和感觉可以通过使用主题或通过向它们添加自定义项和标签来自定义。 Qt 数据可视化基于 Qt 5 和 OpenGL 构建，以利用硬件加速和 Qt Quick 2。

## Qt Declarative (Quick 2)

- [qt/qtdeclarative](https://github.com/qt/qtdeclarative/tree/104eae5b17b0ec700391e9539ee3a4f638588194)

Qt Declarative 模块提供了一个动态框架用来构建动态的交互界面，为 QML 提供运行环境；解决了 C++ 和 QML 的交互问题。

Qt Quick2 提供了5个 Qt模块 作为开发 QML 程序的框架，包括 Qt QML， Qt Quick, Qt Quick Controls, Qt Quick Dialogs, Qt Quick Layouts 共5个模块

可参见 [Porting QML Applications to Qt 5](https://doc.qt.io/qt-5/qtquick-porting-qt5.html) 部分章节

`备注:` 
- [qtdeclarative/examples/qml/tutorials/extending-qml/](https://github.com/qt/qtdeclarative/tree/104eae5b17b0ec700391e9539ee3a4f638588194/examples/qml/tutorials/extending-qml) 提供了一些例子用来扩展 QML 。

## 其他

### Coin 

- [Coin](https://github.com/qt/qt5/tree/v5.15.2/coin) 
  
该目录包含 Qt 的持续集成系统 (Coin) 使用的文件。

### gnuwin32

- [gnuwin32](https://github.com/qt/qt5/tree/v5.15.2/gnuwin32) 

此目录包含各种工具的 Windows 二进制文件，例如 bison 来自 [GnuWin32 项目](http://gnuwin32.sourceforge.net/)。构建项目也会需要它们，例如 QtWebKit。

除了 GnuWin32 二进制文件之外，该目录还包含 Win 的分发版 [flex-bison 项目](http://sourceforge.net/projects/winflexbison/)，它提供 Flex 和 Bison 的端口比 GnuWin32 提供的要新得多。

## 补充说明

可参加 [QtAdvanced/Qt之基础架构及各个模块简介/Qt之基础架构及各个模块简介](https://github.com/dragondjf/QtAdvanced/blob/master/Qt%E4%B9%8B%E5%9F%BA%E7%A1%80%E6%9E%B6%E6%9E%84%E5%8F%8A%E5%90%84%E4%B8%AA%E6%A8%A1%E5%9D%97%E7%AE%80%E4%BB%8B/Qt%E4%B9%8B%E5%9F%BA%E7%A1%80%E6%9E%B6%E6%9E%84%E5%8F%8A%E5%90%84%E4%B8%AA%E6%A8%A1%E5%9D%97%E7%AE%80%E4%BB%8B.md)


# 参考链接

- [Qt advanced tour - 如果你想使用Qt如鱼得水, 这里的东西值得你学习。](https://github.com/dragondjf/QtAdvanced)


---

- [上一级](README.md)
- 上一篇 -> [《Qml Book》 - Extending QML with C++](qmlbook_18_extendingQmlWithc++.md)
- 下一篇 -> [qwt 简单使用](qwt.md)
