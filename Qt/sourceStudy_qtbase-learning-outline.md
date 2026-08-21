# QtBase 设计思想与设计模式学习大纲

> **源码版本基准：** 本学习大纲基于 **QtBase 6.10.2（`qtbase-6.10.2`）** 源码编写。文中涉及的模块结构、文件路径、类接口和实现细节均以该版本为准；其他 Qt 版本可能存在目录、API 或内部实现差异。

QtBase 最适合按“机制主线”学习，而不是从 `src/` 开始逐目录通读。建议用 16 周左右完成第一轮：

```text
C++ 值语义与 ABI
        ↓
QObject + 元对象系统
        ↓
事件循环 + 线程亲和性
        ↓
异步 I/O 与网络
        ↓
QPA 平台抽象
        ↓
绘制系统
        ↓
Widgets + Model/View
        ↓
插件、测试与工程化
```

## 一、学习目标

完成后应能回答：

- 信号槽到底如何连接、调用和跨线程投递？
- `QObject` 为什么不能复制，父子对象如何管理生命周期？
- `QEventLoop` 如何连接操作系统消息循环？
- `QThread` 对象所在的线程和它管理的线程为什么不是一回事？
- Qt 如何同时实现值语义、写时复制和二进制兼容？
- `QPainter` 如何把统一接口映射到不同绘制后端？
- Windows、Linux、macOS 平台差异如何被 QPA 隔离？
- Model/View 为什么要求 `beginInsertRows()`/`endInsertRows()` 成对调用？
- Qt 如何设计可扩展插件系统，同时维持 ABI 稳定？

## 二、16 周学习安排

| 阶段 | 周期 | 主题 | 重点思想与模式 | 实践产物 |
|---|---:|---|---|---|
| 0 | 第 1 周 | 仓库与构建系统 | 模块分层、Public/Private API、Feature 配置 | 模块依赖图 |
| 1 | 第 2～3 周 | 值类型、容器、内存模型 | Value Object、隐式共享、COW、RAII、PIMPL | 一个隐式共享值类 |
| 2 | 第 4～5 周 | QObject 与元对象 | Observer、Composite、Reflection、Type Erasure | 信号槽与属性实验 |
| 3 | 第 6～7 周 | 事件系统与事件循环 | Reactor、Dispatcher、事件队列、拦截器 | 自定义事件与定时器 |
| 4 | 第 8 周 | 线程与任务 | Thread Affinity、Active Object、消息传递 | Worker Object 实验 |
| 5 | 第 9 周 | QIODevice 与网络 | Template Method、异步状态机、背压 | 异步 TCP/文件读取器 |
| 6 | 第 10 周 | QPA 平台抽象 | Abstract Factory、Bridge、Plugin | Windows 窗口创建调用图 |
| 7 | 第 11 周 | 绘制系统 | Facade、Strategy、状态栈、脏区域 | 自绘控件 |
| 8 | 第 12～13 周 | Widgets | Composite、Strategy、Command、布局系统 | 一个复合控件 |
| 9 | 第 14～15 周 | Model/View | MVC、Observer、Proxy、Delegate、Adapter | 树模型和代理模型 |
| 10 | 第 16 周 | 插件、测试、综合项目 | Factory、Metadata、契约测试、ABI 边界 | 综合项目与设计总结 |

## 三、各阶段具体内容

### 0. 建立 QtBase 整体地图

本阶段详细学习文档：[`00-qtbase-overall-map.md`](sourceStudy_00-qtbase-overall-map.md)。

先认识 QtBase 的边界：

- Core：对象模型、事件、线程、容器、I/O。
- Network：异步网络访问。
- Gui：窗口、输入、字体、图像、绘制和平台抽象。
- Widgets：传统桌面控件体系。
- TestLib：测试基础设施。
- SQL、DBus、OpenGL、PrintSupport：外围能力。

模块装配入口可从 [`src/CMakeLists.txt`](https://github.com/qt/qtbase/blob/v6.10.2/src/CMakeLists.txt) 开始。

需要理解：

- QtBase 不是完整的 Qt。
- Qt Quick/QML 的主体在 `qtdeclarative`，不应混入第一轮。
- `*_p.h` 是私有实现，不保证源码或 ABI 稳定。
- `Q_DECLARE_PRIVATE`、`Q_D`、`Q_Q` 是理解 Qt 源码的基本语法。

阶段输出：画一张 `Core → Gui → Widgets` 的依赖图，并说明为什么依赖不能反转。

### 1. 值语义、隐式共享与 ABI

本阶段详细学习文档：[`01-value-semantics-implicit-sharing-abi.md`](sourceStudy_01-value-semantics-implicit-sharing-abi.md)。

阅读：

- [`qshareddata.h`](https://github.com/qt/qtbase/blob/v6.10.2/src/corelib/tools/qshareddata.h)
- `src/corelib/tools/qarraydata.h`
- `src/corelib/tools/qarraydatapointer.h`
- `src/corelib/tools/qlist.h`
- `src/corelib/text/qstring.h`
- `src/corelib/text/qbytearray.h`
- `src/corelib/tools/qscopedpointer.h`
- `src/corelib/tools/qscopeguard.h`

重点：

- 引用计数和 Copy-on-Write。
- const/non-const 接口为什么影响 detach。
- 移动语义和隐式共享如何共存。
- PIMPL 如何降低编译依赖、保护 ABI。
- Qt 容器与 STL 容器的设计取舍。
- 为什么有些 Qt 类型是值类型，有些必须是 `QObject`。

练习：

1. 使用 `QSharedDataPointer` 实现一个 `Person` 值类。
2. 验证复制后共享、修改时分离。
3. 比较复制大量 `QString`、`std::string` 的行为。
4. 记录“值语义”和“对象身份语义”的适用边界。

### 2. QObject、MOC 与元对象系统

本阶段详细学习文档：[`02-qobject-moc-metaobject-system.md`](sourceStudy_02-qobject-moc-metaobject-system.md)。

核心入口：

- [`qobject.h`](https://github.com/qt/qtbase/blob/v6.10.2/src/corelib/kernel/qobject.h)
- [`qobject_p.h`](https://github.com/qt/qtbase/blob/v6.10.2/src/corelib/kernel/qobject_p.h)
- [`qobjectdefs.h`](https://github.com/qt/qtbase/blob/v6.10.2/src/corelib/kernel/qobjectdefs.h)
- [`qobject.cpp`](https://github.com/qt/qtbase/blob/v6.10.2/src/corelib/kernel/qobject.cpp)
- `src/corelib/kernel/qmetaobject.cpp`
- `src/corelib/kernel/qmetatype.h`
- `src/corelib/kernel/qproperty.h`

重点：

- `Q_OBJECT` 经 MOC 生成了什么。
- `staticMetaObject`、方法索引、信号索引。
- 信号槽与传统 Observer 的区别：生命周期感知、类型检查、跨线程排队、自动断开和运行时调用。
- 父子关系既是 Composite，也是生命周期协议。
- `QMetaType`、`QVariant` 的类型擦除。
- Qt 6 的 `QProperty`、`QBindable` 响应式依赖。

练习：

- 查看一个类生成的 `moc_*.cpp`。
- 分别测试 Direct、Queued、BlockingQueued、AutoConnection。
- 删除 sender、receiver、context，观察连接生命周期。
- 使用 `QMetaObject::invokeMethod()` 动态调用。
- 写一张“emit 到 slot”的完整调用图。

### 3. 事件循环与事件分发

本阶段详细学习文档：[`03-event-loop-and-event-dispatch.md`](sourceStudy_03-event-loop-and-event-dispatch.md)。

核心入口：

- [`qeventloop.cpp`](https://github.com/qt/qtbase/blob/v6.10.2/src/corelib/kernel/qeventloop.cpp)
- [`qcoreapplication.cpp`](https://github.com/qt/qtbase/blob/v6.10.2/src/corelib/kernel/qcoreapplication.cpp)
- [`qabstracteventdispatcher.h`](https://github.com/qt/qtbase/blob/v6.10.2/src/corelib/kernel/qabstracteventdispatcher.h)
- `src/corelib/kernel/qevent.cpp`
- `src/corelib/kernel/qtimer.cpp`

重点：

- `sendEvent()` 与 `postEvent()` 的同步/异步差异。
- Posted Event Queue 的所有权和优先级。
- Event Dispatcher 如何接入原生消息循环。
- `QObject::event()` 的 Template Method 特征。
- Event Filter 的拦截器/责任链特征。
- Timer、Socket Notifier 如何统一进入事件循环。
- 嵌套事件循环、重入和 `processEvents()` 风险。
- `deleteLater()` 为什么依赖事件循环。

练习：

- 定义自定义 `QEvent`。
- 同时使用 `sendEvent()` 和 `postEvent()`，记录调用栈。
- 安装多层 event filter，验证传播顺序。
- 制造一个嵌套事件循环，观察重入现象。

### 4. QThread 与并发模型

本阶段详细学习文档：[`04-qthread-and-concurrency-model.md`](sourceStudy_04-qthread-and-concurrency-model.md)。

入口：

- [`qthread.h`](https://github.com/qt/qtbase/blob/v6.10.2/src/corelib/thread/qthread.h)
- `src/corelib/thread/qthread.cpp`
- `src/corelib/thread/qthreadpool.cpp`
- `src/corelib/thread/qrunnable.cpp`
- `src/corelib/thread/qmutex.cpp`
- `src/corelib/thread/qfuture*`
- `src/concurrent`

重点理解三个概念：

1. `QThread` 对象自身的线程亲和性。
2. `QThread::run()` 创建并运行的操作系统线程。
3. 移入工作线程的 Worker `QObject`。

需要掌握：

- Worker Object 模式。
- Queued Connection 本质上是向目标线程事件队列投递调用事件。
- `moveToThread()` 的约束。
- `deleteLater()` 和线程退出的组合。
- BlockingQueuedConnection 的死锁条件。
- 消息传递与共享内存两种并发模型。
- `QThreadPool`、`QtConcurrent`、专用线程的选择。

实践：实现一个支持启动、进度、取消、超时和安全销毁的后台任务。

### 5. QIODevice 与异步网络

本阶段详细学习文档：[`05-qiodevice-and-async-network.md`](sourceStudy_05-qiodevice-and-async-network.md)。

入口：

- [`qiodevice.h`](https://github.com/qt/qtbase/blob/v6.10.2/src/corelib/io/qiodevice.h)
- `src/corelib/io/qfile.cpp`
- `src/corelib/io/qbuffer.cpp`
- `src/network/socket/qabstractsocket.cpp`
- `src/network/access/qnetworkaccessmanager.cpp`
- `src/network/access/qnetworkreply.cpp`

重点：

- `QIODevice` 如何用 Template Method 统一文件、内存、Socket。
- Sequential 与 Random Access 设备。
- `readyRead()` 为什么不等于“一条完整消息已到达”。
- 部分读取、部分写入、缓冲区和协议分帧。
- 网络对象内部异步状态迁移。
- 超时、取消、错误和对象销毁之间的竞争。

实践：实现 length-prefixed TCP 客户端，正确处理粘包、半包、断线和超时。

### 6. QPA 与跨平台设计

本阶段详细学习文档：[`06-qpa-and-cross-platform-design.md`](sourceStudy_06-qpa-and-cross-platform-design.md)。

入口：

- [`qplatformintegration.h`](https://github.com/qt/qtbase/blob/v6.10.2/src/gui/kernel/qplatformintegration.h)
- [`qplatformintegrationplugin.h`](https://github.com/qt/qtbase/blob/v6.10.2/src/gui/kernel/qplatformintegrationplugin.h)
- [`Windows 平台插件入口`](https://github.com/qt/qtbase/blob/v6.10.2/src/plugins/platforms/windows/main.cpp)
- `src/plugins/platforms/windows`
- `src/plugins/platforms/minimal`

重点：

- `QGuiApplication → QWindow → QPlatformWindow`。
- 公共 API 与平台实现之间的 Bridge。
- `QPlatformIntegration` 作为 Abstract Factory。
- 平台插件的运行时选择。
- 原生窗口消息如何转换为 Qt 事件。
- Capability 查询为什么优于大量平台宏判断。

实践：在 Windows 下跟踪一次 `QWindow::show()`，画出从 Qt API 到 Win32 窗口创建的调用序列。

### 7. QPainter 绘制架构

本阶段详细学习文档：[`07-qpainter-painting-architecture.md`](sourceStudy_07-qpainter-painting-architecture.md)。

入口：

- [`qpainter.h`](https://github.com/qt/qtbase/blob/v6.10.2/src/gui/painting/qpainter.h)
- [`qpaintengine.h`](https://github.com/qt/qtbase/blob/v6.10.2/src/gui/painting/qpaintengine.h)
- `src/gui/painting/qpainter.cpp`
- `src/gui/painting/qpaintdevice.cpp`
- `src/gui/painting/qpaintengine_raster.cpp`
- `src/gui/painting/qbackingstore.cpp`
- `src/gui/painting/qdrawhelper.cpp`

重点：

- `QPainter` 是统一绘制门面。
- `QPaintDevice` 表示目标。
- `QPaintEngine` 是后端策略/桥接点。
- Painter State、`save()`/`restore()`。
- 坐标变换、裁剪、Composition Mode。
- Raster Engine 和 SIMD 优化。
- Backing Store、脏区域和局部刷新。

实践：实现一个带缩放、旋转、裁剪、透明合成的自绘控件，并观察 `repaint()`/`update()` 的差别。

### 8. QWidget 体系

本阶段详细学习文档：[`08-qwidget-system.md`](sourceStudy_08-qwidget-system.md)。

入口：

- [`qwidget.h`](https://github.com/qt/qtbase/blob/v6.10.2/src/widgets/kernel/qwidget.h)
- `src/widgets/kernel/qwidget.cpp`
- `src/widgets/kernel/qapplication.cpp`
- `src/widgets/kernel/qlayout.cpp`
- `src/widgets/styles/qstyle.cpp`
- `src/widgets/styles/qproxystyle.h`
- `src/gui/kernel/qaction.cpp`

重点：

- QWidget 同时继承 `QObject` 和 `QPaintDevice` 的意义。
- Widget Tree、对象树和原生窗口树并不总是同一棵树。
- 布局系统的 SizeHint/Minimum/Maximum 协议。
- Style 作为策略，`QProxyStyle` 作为装饰/代理。
- `QAction` 的 Command 特征。
- Focus、Shortcut、Mouse Grab、Modal 等输入状态。

### 9. Model/View 架构

本阶段详细学习文档：[`09-model-view-architecture.md`](sourceStudy_09-model-view-architecture.md)。

入口：

- [`qabstractitemmodel.h`](https://github.com/qt/qtbase/blob/v6.10.2/src/corelib/itemmodels/qabstractitemmodel.h)
- [`qabstractitemview.h`](https://github.com/qt/qtbase/blob/v6.10.2/src/widgets/itemviews/qabstractitemview.h)
- `src/corelib/itemmodels/qabstractproxymodel.h`
- `src/corelib/itemmodels/qsortfilterproxymodel.h`
- `src/widgets/itemviews/qstyleditemdelegate.h`
- `src/corelib/itemmodels/qitemselectionmodel.cpp`

重点：

- Model、View、Delegate、Selection Model 的职责分离。
- `QModelIndex` 为什么不是数据对象。
- Internal Pointer 的生命周期约束。
- `QPersistentModelIndex` 如何跟随结构变化。
- Proxy Model 的 Proxy/Adapter 模式。
- Delegate 的 Strategy 特征。
- `beginInsertRows()` 等接口建立的事务式通知协议。
- Lazy Fetch 和大数据模型。

实践项目：

- 自定义树模型。
- 动态增删节点。
- 使用 `QSortFilterProxyModel` 过滤排序。
- 自定义 Delegate。
- 使用 model tester 验证模型契约。

### 10. 插件、测试和工程化

本阶段详细学习文档：[`10-plugins-testing-engineering.md`](sourceStudy_10-plugins-testing-engineering.md)。

阅读：

- [`qfactoryloader_p.h`](https://github.com/qt/qtbase/blob/v6.10.2/src/corelib/plugin/qfactoryloader_p.h)
- `src/corelib/plugin/qpluginloader.cpp`
- `src/corelib/plugin/qplugin.h`
- Windows/minimal 平台插件。
- `tests/auto/corelib/kernel/qobject`
- `tests/auto/corelib/kernel/qeventloop`
- `tests/benchmarks`

重点：

- IID、接口声明、插件元数据。
- 静态插件与动态插件。
- Factory、Registry、Plugin 架构。
- QtTest、data-driven test、`QSignalSpy`。
- `tests/auto` 与 `tests/benchmarks` 的区别。
- 测试用例如何充当“可执行设计文档”。
- Export Macro、Private Header、二进制兼容。

## 四、设计模式速查表

| 设计思想 | QtBase 中的代表 |
|---|---|
| Observer | Signals/Slots、Model 变更信号 |
| Composite | QObject 父子树、Widget 树 |
| Reactor | Event Loop、Timer、Socket Notifier |
| Dispatcher | QCoreApplication、QAbstractEventDispatcher |
| Chain of Responsibility | Event Filter、事件传播 |
| Template Method | QObject::event、QIODevice read/write |
| Abstract Factory | QPlatformIntegration |
| Bridge | QWindow/QPlatformWindow、QPainter/QPaintEngine |
| Strategy | QPaintEngine、QStyle、Delegate |
| Proxy | QAbstractProxyModel、QSortFilterProxyModel、QProxyStyle |
| Command | QAction、Posted Event、Queued Invocation |
| Adapter | Model/View、平台事件到 QEvent 的转换 |
| Type Erasure | QVariant、QMetaType、部分连接实现 |
| PIMPL | QObjectPrivate 及大量 `FooPrivate` |
| Copy-on-Write | QString、QByteArray、Qt 容器、QSharedDataPointer |
| Plugin | QPluginLoader、QFactoryLoader、平台插件 |

不要只给类贴 GoF 标签。每次都继续追问：

- 它解决了什么生命周期问题？
- 是否跨线程？
- 是否要求事件循环？
- 同步还是异步？
- 是否允许重入？
- 如何保持 ABI？
- 扩展点在哪里，哪些部分刻意不允许扩展？

## 五、推荐的源码阅读方法

每个主题固定按以下顺序读：

1. 公共头文件：理解契约和扩展点。
2. `*_p.h`：理解真实状态和内部数据结构。
3. `.cpp` 中的一条行为链：按行为路径定向阅读。
4. 一个具体后端或子类：理解抽象如何落地。
5. 对应 `tests/auto`：确认边界条件和设计不变量。
6. 使用调试器设置断点，记录真实调用栈。
7. 写一页自己的设计笔记。

例如研究一次跨线程信号槽：

```text
QObject::connect
    → 保存 Connection
    → emit
    → QMetaObject::activate
    → 判断连接类型和线程
    → 创建调用事件
    → postEvent 到目标线程
    → 目标线程 EventLoop
    → 执行 slot
```

## 六、综合结课项目

建议做一个“实时日志查看器”：

- 使用 `QIODevice` 接收文件或网络日志。
- Worker Object 在后台线程解析。
- 通过 Queued Connection 传递结果。
- 自定义 `QAbstractItemModel` 保存日志。
- 使用 `QSortFilterProxyModel` 过滤。
- 自定义 Delegate 高亮。
- 自绘统计面板。
- 数据源通过插件扩展。
- 使用 QtTest 测试线程退出、模型更新、取消和错误。
- 最后画出对象生命周期图、线程图、事件流图和模块图。

这个项目能把 QtBase 最重要的几条设计主线真正串起来。第一轮完成后，再根据方向进入 `qtdeclarative`、多媒体、WebEngine 或更深的平台后端。

## 七、文档维护检查

从仓库根目录运行：

```bash
python3 Qt/check_sourceStudy_links.py
python3 -m unittest Qt/test_sourceStudy_links.py
```

第一条命令检查所有 `sourceStudy_*.md` 的本地章节链接，并拒绝逃出 `Qt/` 文档目录的相对链接；QtBase 源码证据应使用固定到 `v6.10.2` 的官方链接。第二条命令验证检查器自身对有效链接、缺失目标和越界目标的处理。
