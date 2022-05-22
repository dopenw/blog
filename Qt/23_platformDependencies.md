<!-- 23. 平台相关性 -->

<!-- @import "[TOC]" {cmd="toc" depthFrom=1 depthTo=6 orderedList=false} -->

<!-- code_chunk_output -->

- [连接本地的应用程序编程接口](#连接本地的应用程序编程接口)
- [Link](#link)

<!-- /code_chunk_output -->

注：该文档为 《C++ GUI Qt 4 编程》的学习文档

如何访问一些本地应用程序编程接口 (API) 呢？

- Use [Win32](https://en.wikipedia.org/wiki/Windows_API) on windows platform
- Recommend [Cocoa](https://en.wikipedia.org/wiki/Cocoa_(API)) instead of [Carbon](https://en.wikipedia.org/wiki/Carbon_(API))  on mac platform
- Use [Xlib](https://en.wikipedia.org/wiki/Xlib) or [XCB](https://en.wikipedia.org/wiki/XCB) on X11 platform

此外，Qt 还提供了多个与平台相关的 [Qt Solutions](https://github.com/qtproject/qt-solutions),其中包括：

- The QtBrowserPlugin solution is useful for implementing plugins
for web browser.
- The QtLockedFile class extends QFile with advisory locking
functions.
- A property browser framework enabling the user to edit a set of
properties.
- The QtScript Classic component provides the original (pre-Qt 4.6) implementation
of the QtScript module.
- The QtService component is useful for developing Windows services
and Unix daemons.
- The QtSingleApplication component provides support for
applications that can be only started once per user.
- The Qt SOAP project provides basic web service support with
version 1.1 of the SOAP protocol.
- The Qt/MFC Migration Framework tool assists in the migration of
existing Win32 or MFC applications to the Qt toolkit.

## 连接本地的应用程序编程接口

Qt 完善的 API 可以满足所有平台上的绝大多数需求，但是在某些情况下，我们也许想使用底层的、与平台相关的 API 。

对于每一个平台，[QWidget](https://doc.qt.io/qt-5/qwidget.html) 都提供了一个可以返回窗口 ID 或者其句柄的 [winId()](https://doc.qt.io/qt-5/qwidget.html#winId) 函数。QWidget 还提供了一个称为 [find()](https://doc.qt.io/qt-5/qwidget.html#find) 的静态函数，他可以根据一个特定的窗口 ID 返回该窗口的 QWidget 。例如，下面的代码使用  winId() 和  mac OS X(Carbon) 上的函数把一个工具窗口的标题栏移动到了该窗口的左侧。

```c++
#ifdef Q_OS_MACOS
  ChangeWindowAttributes(HIViewGetWindow(
          HIViewRef(toolWin.winId()))
          ,kWindowSideTitlebarAttribute
          ,kWindowNoAttributes)

#endif 
```

在 X11 上，以下给出了应当如何修改一个窗口属性的代码片段：

```c++
#ifdef Q_OS_LINUX
  Atom atom = XInternAtom(QX11Info::display()
                    ,"My_PROPERTY",False);
  long data = 1;
  XChangeProperty(QX11Info::display(),window->winId()
              ,atom,atom
              ,32,PropModeReplace
              ,reinterpret_cast<uchar *>(&data),1);
#endif 
```

使用 GDI 调用在一个 Qt 窗口部件上进行绘制：

```c++
void GdiControl::paintEvent(QPaintEvent * event){
  RECT rect;
  GetClientRect(winId(),&rect);
  HDC hdc = GetDc(winId());

  FillRect(hdc,&rect,HBRUSH(COLOR_WINDOW +1));
  SetTextAlign(hdc,TA_CENTER | TA_BASELINE);
  TextOutW(hdc,width()/2,height()/2,text.utf16(),text.size());
  ReleaseDC(windId(),hdc);
}
```

要使这段代码能够工作，还必须重新实现 [QPaintDevice::paintEngine()](https://doc.qt.io/qt-5/qpaintdevice.html#paintEngine),以便可以返回一个 null 指针，并且还需要在这个窗口部件的构造函数中设置 Qt::WA_PaintOnScreen 属性。

下面的例子，说明了在一个使用 [QPaintEngine](https://doc.qt.io/qt-5/qpaintdevice.html) 的 getDc() 和 releaseDC() 函数的绘制事件处理器中，如何组合使用 QPainter 和  GDI 调用：

```c++
void MyWidget::paintEvent(QPaintEvent * event){
  QPainter painter(this);
  painter.fillRect(rect().adjust(20,20,-20,-20),Qt::red);
#ifdef Q_OS_WIN
  HDC hdc = painter.paintEngine->getDC();
  Rectangle(hdc,40,40,width()-40,height()-40);
  painter.paintEngine()->releaseDC;
#endif
}
```

像这样混合使用  QPainter 和 GDI 调用，有时可能会导致一些不可预料的后果，在 QPainter 调用出现在  GDI 调用之后的时候尤其如此，因为 QPainter 会对底层的绘制层(drawing layer) 的状态做出一些假设
。

针对不同的平台， Qt 也提供不同的符号定义：

- [Q_OS_WIN](https://doc.qt.io/qt-5/qtglobal.html#Q_OS_WIN) - Defined on all supported versions of Windows.
- [Q_OS_LINUX](https://doc.qt.io/qt-5/qtglobal.html#Q_OS_LINUX)
- [Q_OS_MACOS](https://doc.qt.io/qt-5/qtglobal.html#Q_OS_MACOS)
- [Q_OS_UNIX](https://doc.qt.io/qt-5/qtglobal.html#Q_OS_UNIX) - Defined on Any UNIX BSD/SYSV system.

详情可见 [<QtGlobal> - Global Qt Declarations](https://doc.qt.io/qt-5/qtglobal.html).

在程序运行时，我们可以通过 [QSysInfo::productVersion()](https://doc.qt.io/qt-5/qsysinfo.html#productVersion) 来区分系统版本。

除了这些操作系统宏之外，Qt 还有一系列用于编译器的宏。例如，如果编译器是 Microsoft Visual c++ ,那么就会定义宏 [Q_CC_MSVC](https://doc.qt.io/archives/qt-4.8/qtglobal.html#Q_CC_MSVC) 。在避开编译器中的 bug 时，这些宏会显得非常有用。

几个与 Qt 图形用户界面相关的类提供了一些与平台相关的函数,可见：

- [Qt Mac Extras](https://doc.qt.io/qt-5/qtmacextras-index.html) - Qt Mac Extras provide classes and functions that enable you to use miscellaneous functionality specific to the macOS and iOS operating systems.
- [Qt X11 Extras](https://doc.qt.io/qt-5/qtx11extras-index.html) - Qt X11 Extras enables the Qt programmer to write applications for the Linux/X11 platform. Applications developed with Qt can also be deployed across several other desktop and embedded operating systems without having to rewrite the source code.
- [Qt Windows Extras](https://doc.qt.io/qt-5/qtwinextras-index.html) - Qt Windows Extras provide classes and functions that enable you to use miscellaneous Windows-specific functions. For example, you can convert Qt objects to Windows object handles and manipulate DWM glass frames.

针对不同的平台，可见：

- [Qt for Windows](https://doc.qt.io/qt-5/windows.html)
- [Qt for Linux/X11](https://doc.qt.io/qt-5/linux.html)
- [Qt for macOS](https://doc.qt.io/qt-5/macos.html)

经常需要与其他工具包或者库进行交互的许多 Qt 应用程序必须在将一些底层事件（在 X11 上是 [XEvent](https://linux.die.net/man/3/xevent) ,在 Windows 上是 [MSG](https://docs.microsoft.com/en-us/windows/win32/api/winuser/ns-winuser-msg) ,在 Mac OS X 上是 [NSEvent](https://developer.apple.com/documentation/appkit/nsevent?language=objc) ） 转换成 QEvent 之前能够先访问这些底层事件。可参考 [QAbstractNativeEventFilter](https://doc.qt.io/qt-5/qabstractnativeeventfilter.html#nativeEventFilter) 和 [QWidget::nativeEvent](https://doc.qt.io/qt-5/qwidget.html#nativeEvent).

## Link

- [difference between carbon and cocoa?](https://stackoverflow.com/questions/1895800/difference-between-carbon-and-cocoa)
- [https://stackoverflow.com/questions/9026020/is-xcb-ready-for-production-use-should-i-use-xlib-instead](https://stackoverflow.com/questions/9026020/is-xcb-ready-for-production-use-should-i-use-xlib-instead)
- [Xlib and XCB](https://www.x.org/wiki/guide/xlib-and-xcb/)

---

- [上一级](README.md)
- 上一篇 -> [21. 创建插件](21_createPlugin.md)
- 下一篇 -> [Qt Creator 使用](QtCreatorTips.md)
