# 7. 事件处理


<!-- @import "[TOC]" {cmd="toc" depthFrom=1 depthTo=6 orderedList=false} -->
<!-- code_chunk_output -->

- [7. 事件处理](#7-事件处理)
  - [重新实现事件处理器](#重新实现事件处理器)
    - [Ticker.h](#tickerh)
    - [Ticker.cpp](#tickercpp)

<!-- /code_chunk_output -->


事件 ([event](https://doc.qt.io/qt-5/events.html)) 是由窗口系统或者 Qt 自身产生的，用以响应所发生的各类事情。比如:键盘事件、鼠标事件、绘制事件等。大多数事件是作为用户动作的响应而产生的，但是也有一些例外，比如像定时器事件，则由系统独立产生的。

不应该混淆 "事件" 和 “信号” 这两个概念。一般情况下，在使用窗口部件的时候，信号是十分有用的；而在实现窗口部件时，事件则是十分有用的。

## 重新实现事件处理器

在 Qt 中，事件就是 [QEvent](https://doc.qt.io/qt-5/qevent.html) 子类的一个实例。Qt 处理的事件类型有一百多种，其中的每一种都可以通过一个枚举值来进行时别。例如， QEvent::type() 可以返回用于处理鼠标按键事件的 QEvent::MouseButtonPress。

通过继承 [QObject](https://doc.qt.io/qt-5/qobject.html) ，事件通过它们的 event() 函数来通知对象。在 QWidget 中的 event() 实现把绝大多数常用类型的事件提前传递给特定的事件处理器，比如 mousePressEvent() 、keyPressEvent() 以及 paintEvent().

再此将会回顾两种值得详述的常用事件类型：键盘事件和定时器事件。

通过重新实现 keyPressEvent() 和 keyReleaseEvent()，就可以处理键盘事件了。通常，我们只需要重新实现 keyPressEvent() 就可以了，因为用于表明释放重要性的键只有 Ctrl、Shift、和 Alt 这些修饰键，而这些键的状态可以在 keyPressEvent() 中使用 QKeyEvent::modifiers() 检测出来。例如，我们要区分按下的键是 Home 还是 Ctrl + Home ，它的 keyPressEvent() 函数中的部分内容看起来应该是这样的：
```c++
void CodeEditor::keyPressEvent(QKeyEvent *event)
{
  switch (event->key()) {
    case Qt::Key_Home:
    if (event->modifiers() & Qt::ControlModifier) {
      goToBeginningOfDocument();
    } else {
      goToBeginningOfLine();
      }
      break;
      case Qt::Key_End:
      ...
      default:
      QWidget::keyPressEvent(event);
    }
}
```

Tab 键 和 BackTab(Shift + Tab) 键是两种特殊情况。在窗口部件调用 keyPressEvent() 之前， QWidget::event() 会先处理它们，它所包含的语义就是用于把焦点传递给焦点序列中的下一个或者上一个窗口部件。如果我们想更改 Tab 键起到缩放文本行的作用，可以这样：
```c++
bool CodeEditor::event(QEvent *event)
{
if (event->type() == QEvent::KeyPress) {
    QKeyEvent * keyEvent = static_cast<QKeyEvent * >(event);
    if (keyEvent->key() == Qt::Key_Tab) {
      insertAtCurrentPosition('\t');
      return true;
    }
  }
  return QWidget::event(event);
}
```

实现键绑定的一种更为高级的方法是使用 [QAction](https://doc.qt.io/qt-5/qaction.html) 。 例如：
```c++
MainWindow::MainWindow()
{
  editor = new CodeEditor;
  setCentralWidget(editor);
  goToBeginningOfLineAction =
      new QAction(tr("Go to Beginning of Line"), this);
  goToBeginningOfLineAction->setShortcut(tr("Home"));
  connect(goToBeginningOfLineAction, SIGNAL(activated()),
      editor, SLOT(goToBeginningOfLine()));
  goToBeginningOfDocumentAction =
      new QAction(tr("Go to Beginning of Document"), this);
  goToBeginningOfDocumentAction->setShortcut(tr("Ctrl+Home"));
  connect(goToBeginningOfDocumentAction, SIGNAL(activated()),
      editor, SLOT(goToBeginningOfDocument()));
  ...
}
```
默认情况下，一旦把包含窗口部件的窗口激活，就可以在该窗口部件上使用 QAction 或 [QShortCut](https://doc.qt.io/qt-5/qshortcut.html) 来启用所设置的绑定键。但使用 QAction::setShortcutContext() 或者 QShortCut::setContext() 可以改变这一点。

另外一种常用的事件类型是定时器事件。eg：Ticker 窗口部件

![](../images/7_event_202004021301_1.png)

### Ticker.h

```c++
#ifndef TICKER_H
#define TICKER_H

#include <QWidget>

class Ticker : public QWidget
{
    Q_OBJECT
    Q_PROPERTY(QString text READ text WRITE setText)

public:
    Ticker(QWidget * parent = 0);

    void setText(const QString &newText);
    QString text() const { return myText; }
    QSize sizeHint() const;

protected:
    void paintEvent(QPaintEvent * event) override;
    void timerEvent(QTimerEvent * event) override;
    void showEvent(QShowEvent * event) override;
    void hideEvent(QHideEvent * event) override;

private:
    QString myText;
    int offset;
    int myTimerId;
};

#endif
```

### Ticker.cpp
```c++
#include <QtWidgets>

#include "Ticker.h"

Ticker::Ticker(QWidget *parent)
    : QWidget(parent)
{
    offset = 0;
    // 定时器的 ID 通常是非零的，所以可以使用 0 来表示定时器还没有启动。
    myTimerId = 0;
}

void Ticker::setText(const QString &newText)
{
    myText = newText;
    // 强制执行一个重绘操作
    update();
    // 通知 Ticker 窗口部件负责的任意布局管理器，提示该窗口部件的大小发生了变化。
    updateGeometry();
}

// 以文本所需的空间大小作为窗口部件的理想尺寸
QSize Ticker::sizeHint() const
{
    return fontMetrics().size(0, text());
}

void Ticker::paintEvent(QPaintEvent * /* event */)
{
    QPainter painter(this);

    // 确定文本在水平方向上所需要的空间
    int textWidth = fontMetrics().width(text());
    if (textWidth < 1)
        return;

    // 多次绘制，直到能够填充整个窗口部件的宽度为止。
    int x = -offset;
    while (x < width()) {
        painter.drawText(x, 0, textWidth, height(),
                         Qt::AlignLeft | Qt::AlignVCenter, text());
        x += textWidth;
    }
}

void Ticker::showEvent(QShowEvent * /* event */)
{
    // 启动一个定时器，并返回定时器 ID
    myTimerId = startTimer(30); // 大约每30ms Qt 都会产生一个定时器事件。
    // 至于具体的时间精度，则取决于所在的操作系统
    // 我们本可以在构造函数中调用 startTimer(),但是在这里只有在窗口部件实际可见的时候
    // ，才有必要保存由 Qt 产生的定时器事件的那些资源
}

// 系统每隔一段时间，都会调用一次 timerEvent()
void Ticker::timerEvent(QTimerEvent *event)
{
    if (event->timerId() == myTimerId) {
        ++offset;
        if (offset >= fontMetrics().width(text()))
            offset = 0;
        // 把窗口部件的内容向左滚动一个像素
        scroll(-1, 0);
        // 本来也足可以调用 update() 代替 scroll(),
        // 但使用 scroll() 会更有效率，因为它只是简单地移动屏幕上已经存在的像素并且
        // 只对这个窗口部件的新显示区域（这里是 1 * width）产生一个绘制事件。
    } else {
        // 如果不是我们所关注的那个定时器，就可以把它传递给基类
        QWidget::timerEvent(event);
    }
}

void Ticker::hideEvent(QHideEvent * /* event */)
{
    // 停止定时器
    killTimer(myTimerId);
    myTimerId = 0;
}
```

定时器事件是一种低级事件，而且如果需要多个定时器时，保持对所有定时器 ID 的跟踪将会变得很麻烦。在这种情况下，通常更为简单的方式是为每一个定时器分别创建一个 [QTimer](https://doc.qt.io/qt-5/qtimer.html) 对象。QTimer 会在每个时间间隔发射 timeout() 信号。

```c++
// Example for a one second (1000 millisecond) timer
   QTimer *timer = new QTimer(this);
   connect(timer, &QTimer::timeout, this, QOverload<>::of(&AnalogClock::update));
   timer->start(1000);
```
QTimer 提供了非常方便的接口，可用于单触发定时器。
```c++
// You can set a timer to time out only once by calling setSingleShot(true).
//  You can also use the static QTimer::singleShot() function
//  to call a slot after a specified interval:
  QTimer::singleShot(200, this, SLOT(updateCaption()));
```


[上一级](README.md)
[上一篇](6_layoutManage.md)
[下一篇](14_multiThread.md)
