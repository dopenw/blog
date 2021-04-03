# 8. 二维图形


<!-- @import "[TOC]" {cmd="toc" depthFrom=1 depthTo=6 orderedList=false} -->
<!-- code_chunk_output -->

- [8. 二维图形](#8-二维图形)
  - [用 QPainter 绘图](#用-qpainter-绘图)
  - [坐标系统转换](#坐标系统转换)
    - [OvenTimer.h](#oventimerh)
    - [OvenTimer.cpp](#oventimercpp)
  - [用 QImage 高质量绘图](#用-qimage-高质量绘图)
  - [基于项的图形视图](#基于项的图形视图)
    - [图标编辑器](#图标编辑器)
    - [CityScape](#cityscape)
  - [打印](#打印)
  - [Link](#link)

<!-- /code_chunk_output -->

Qt 的二维图形引擎是基于 [QPainter](https://doc.qt.io/qt-5/qpainter.html)。 QPainter 即可以绘制几何形状（点、线、矩形、椭圆、弧形、弦型、饼状图、多边形和贝塞尔曲线），也可以绘制像素映射、图像和文字。此外，QPainter 还支持一些高级特性，例如反走样（针对文字和图形边缘）、像素混合、渐变填充和矢量路径等。QPainter 也支持线性变换，例如平移、旋转、错切和缩放。

QPainter 可以画在 “绘图设备”上，例如 QWidget、QPixmap、 [QImage](https://doc.qt.io/qt-5/qimage.html) 或者 [QSvgGenerator](https://doc.qt.io/qt-5/qsvggenerator.html)。 QPainter 也可以与 [QPrinter](https://doc.qt.io/qt-5/qprinter.html) 一起使用来打印文件和创建 PDF 文档。这意味着通常可以用相同的代码在屏幕上显示数据，也可以生成打印形式的报告。

重新实现 QWidget::paintEvent() 可用于定制窗口部件，并且随心所欲地控制它们地外观。定制预定义 Qt 窗口部件地外观，可以指定某一风格地表单或者创建一个 [QStyle](https://doc.qt.io/qt-5/qstyle.html) 的子类。

一项普通的需求是在二维画板上显示大量的、轻量级的、可与用户交互的、任意形状的项。Qt 4.2 围绕着 [QGraphicsView ](https://doc.qt.io/qt-5/qgraphicsview.html)、 [QGraphicsScene](https://doc.qt.io/qt-5/qgraphicsscene.html)和 [QGraphicsItem](https://doc.qt.io/qt-5/qgraphicsitem.html) 类引入了全新的 “图形视图”体系。这个体系提供了一个操作基于项的图形的高级接口，并且支持用户对项的操作，包括移动、选取、分组。项本身依然用 QPainter 画出，并且可以独立变换。本章稍后将会描述这一体系。

可以使用 [OpenGL](https://doc.qt.io/qt-5/qtopengl-index.html) 命令来代替 QPainter。 OpenGL 是一个绘制三维图形的标准库。后面章节再说。

## 用 QPainter 绘图
要想在绘图设备上绘图，只需创建一个 QPainter ，再将指针传递到该设备中。
```c++
void MyWidget::paintEvent(QPaintEvent *event)
{
  QPainter painter(this);
  ...
}
```
使用 QPainter 的 draw...() 函数，可以绘制各种各样的形状。

最重要的一些函数：
![](../images/8_painter_202004041211_1.png)

绘制的效果取决于 QPainter 的设置。主要的设置：
* 画笔 - 用来画线和边缘。它包含颜色、宽度、线型、拐点风格以及连接风格。

![](../images/8_painter_202004041211_2.png)

* 画刷 - 用来填充几何形状的图案。它一般由颜色和风格组成，但同时也可以是纹理（一个不断重复的图案）或者是一个渐变。

![](../images/8_painter_202004041211_3.png)

* 字体 - 用来绘制文字。字体有很多属性，包括字体族和磅值大小。

可以随时调用 [QPen](https://doc.qt.io/qt-5/qpen.html) 、 [QBrush]() 或者 [QFont](https://doc.qt.io/qt-5/qfont.html) 的 setPen() setBrush() setFont() 来修改这些设置。

绘制椭圆的代码：
```c++
  QPainter painter(this);
  // 启用反走样，它会告诉 QPainter 用不同的颜色强度绘制边框以减少视觉扭曲
  // ，这种扭曲一般会在边框转换为像素时发生。
  painter.setRenderHint(QPainter::Antialiasing, true);
  painter.setPen(QPen(Qt::black, 12, Qt::DashDotLine, Qt::RoundCap));
  painter.setBrush(QBrush(Qt::green, Qt::SolidPattern));
  painter.drawEllipse(80, 80, 400, 240);
```

![](../images/8_painter_202004041211_4.png)

绘制饼状图：
```c++
  QPainter painter(this);
  painter.setRenderHint(QPainter::Antialiasing, true);
  painter.setPen(QPen(Qt::black, 15, Qt::SolidLine, Qt::RoundCap,
  Qt::MiterJoin));
  painter.setBrush(QBrush(Qt::blue, Qt::DiagCrossPattern));
  // drawPie 的最后两个参数以 1/16 为单位
  painter.drawPie(80, 80, 400, 240, 60 * 16, 270 * 16);
```

![](../images/8_painter_202004041211_5.png)

绘制 “三次贝塞尔曲线”：
```c++
  QPainter painter(this);
  painter.setRenderHint(QPainter::Antialiasing, true);
  QPainterPath path;
  path.moveTo(80, 320);
  path.cubicTo(200, 80, 320, 80, 480, 320);
  painter.setPen(QPen(Qt::black, 8));
  painter.drawPath(path);
```

![](../images/8_painter_202004041211_6.png)

[QPainterPath](https://doc.qt.io/qt-5/qpainterpath.html) 类可以通过连接基本的图形元素来确定任意的矢量形状：直线、椭圆、多边形、弧形、贝塞尔曲线和其他绘制路径。绘制路径是最基本的图元。从这个意义上来说，任何图形或图形组合都可以用来绘制路径描述。

路径可以确定一个边缘，由边缘锁定的区域可以用画刷来填充。上面的例子中没有设置画刷，所以只能看见边缘。

这三个例子利用内置的画刷模式(Qt::SolidPattern、Qt::DiagCrossPattern 、 Qt::NoBrush） 。在现代应用中，渐变填充已成为单色填充的流行替代品。渐变填充利用颜色插值使得两个或更多颜色之间能够平滑过渡。它们常被用来创建三维效果 ,[Plastique](https://doc.qt.io/archives/qt-4.8/gallery-plastique.html) 和 [Cleanlooks](https://doc.qt.io/archives/qt-4.8/gallery-cleanlooks.html) 就是使用渐变来渲染 QPushButton的。

Qt 支持三种类型的渐变：线性渐变、锥形渐变和辐射渐变。下一节中的烤箱定时器的例子就是在一个独立的窗口部件上结合了这三种类型的渐变，从而看起来更加逼真。

* 线性渐变(linear gradient) 由两个控制点定义，连接这两个点的线上有一系列的颜色断点。 [QLinearGradient](https://doc.qt.io/qt-5/qlineargradient.html)
 例如：
```c++
  // 在两个控制点之间的不同位置指定三种颜色。位置用 0 和 1 之间的浮点数来指定
  //，0 对应第一个控制点，1 对应第二个控制点。两个指定断点之间的颜色由线性插值得出。
  QLinearGradient gradient(50, 100, 300, 350);
  gradient.setColorAt(0.0, Qt::white);
  gradient.setColorAt(0.2, Qt::green);
  gradient.setColorAt(1.0, Qt::black);
```

![](../images/8_painter_202004041535_1.png)

* 辐射渐变(radio gradient) 由一个中心点(xc,yc)、半径 r、一个焦点(xf,yf),以及颜色断点定义。中心点和半径定义一个圆。颜色从焦点向外扩散，焦点可以是中心点或者圆内的其他点。 [QRadialGradient](https://doc.qt.io/qt-5/qradialgradient.html)

![](../images/8_painter_202004041535_2.png)

* 锥形渐变(conical gradient)由一个中心点(xc,yc) 和一个角度 a 定义。颜色在中心点周围像钟表的秒针一样扩散。 [QConicalGradient](https://doc.qt.io/qt-5/qconicalgradient.html)

![](../images/8_painter_202004041535_3.png)

另外，QPainter 还有其他影响图形和文字绘制方式的设置：
* 当背景模式是 Qt::OpaqueMode(默认设置是 Qt::TransparentMode) 时，背景画刷可以用来填充几何图形的背景（画刷模式下）、文字或者位图。
* 画刷的原点是画刷模式的启动点，通常是窗口部件的左上角。
* 剪切区域是设备的绘图区域。在剪切区域以外绘图不起作用。
* 视图、窗口和世界矩阵决定了如何将 QPainter 的逻辑坐标映射到设备的物理绘图坐标。默认情况下，为了使逻辑坐标和物理坐标保持一致，这些是设置好的。下一节描述坐标系统。
* 复合模式指定新绘制的像素如何更绘图设备上已经显示的像素相互作用。默认方式是 "source over"，在这种情况下像素会被 alpha 混合在存在的像素上。只有特定的设备支持这种模式，本章稍后将描述这种模式。

可以通过调用 save() 而随时将设备的当前状态存入一个内部堆栈，稍后通过调用 restore() 恢复。这对我们如果想临时修改一些设备的设置然后恢复到以前的状态会很有用，会在下一节见到这种情况。

## 坐标系统转换

在 QPainter 的默认坐标体系中，点(0.0) 位于绘图设备的左上角，x 坐标向右增长，y坐标向下增长。默认坐标系的每个像素占 1*1 大小的区域。

理论上，像素的中心取决于半像素的坐标。例如，窗口部件的左上角像素覆盖了点(0,0)到点(1,1)的区域，它的中心点在 (0.5,0.5) 位置。如果告诉 QPainter 绘制一个像素，例如(100,100)，它会相应地在两个方向做 + 0.5 的偏移，使得像素地中心位置在(100.5,100.5)。

这一差别初看起来理论性很强，但他在实践中却很重要。首先，只有当反走样无效时（默认情况）才偏移 +0.5; 如果反走样有效，并且我们试图在 (100,100) 的位置绘制一个黑色像素，实际上 QPainter 会为 (99.5,99.5) (99.5,100.5) (100.5,99.5) (100.5,100.5) 四个像素点着浅灰色，给人的印象是一个像素正好位于四个像素的重合处。如果不需要这种效果，可以通过指定半像素坐标或者通过偏移 QPainter(+0.5,+0.5) 来避免这种效果。

当绘制图形时，例如线、矩形和椭圆，可以使用相似的规则。

未经反走样处理的 drawRect(2,2,6,5) 函数的绘制效果

![](../images/8_painter_202004041535_4.png)

反走样处理的 drawRect(2,2,6,5) 函数的绘制效果

![](../images/8_painter_202004041535_5.png)

反走样处理的 drawRect(2.5,2.5,6,5) 函数的绘制效果

![](../images/8_painter_202004041535_6.png)

窗口和视口密不可分。视口是物理坐标系下指定的任意矩形。窗口也是指同一矩形，只不过是在逻辑坐标系下。当绘制图形时，在逻辑坐标系下指定的点，这些坐标都是基于当前窗口-视口设定并以线性代数的方式转换为物理坐标的。

默认情况下，视口和窗口都被设置成设备的矩形。例如，如果设备是 320 x 200 的矩形，视口和窗口都是左上角为（0,0）的 320 x 200 的相同矩形。这种情况下，逻辑坐标和物理坐标系是一致的。

这种窗口 - 视口机制对于编写独立于绘制设备大小和分辨率的绘制代码是很有用的。例如，如果想让逻辑坐标从 (-50,-50) 到 (+50,+50),并且(0,0) 在中间，可以这样设置：
```c++
painter.setWindow(-50,-50,100,100);
```
(-50,-50)指定了原点，(100,100) 指定了宽和高。这意味着逻辑坐标(-50,-50)对应物理坐标(0,0)，而逻辑坐标(+50,+50) 对应物理坐标（320,200）。如图所示。这个例子没有改变视口。

![](../images/8_painter_202004041814_1.png)

现在来介绍世界变换。世界变换(world transform)是在 窗口 - 视口转换之外使用的变换矩阵。它允许移动、缩放、旋转或者拉伸绘制的项。 [Transformations Example](https://doc.qt.io/qt-5/qtwidgets-painting-transformations-example.html), [QTransform](https://doc.qt.io/qt-5/qtransform.html)
例如，如果想以 45度 角绘制文本，可以这样：
```c++
  QTransform transform;
  transform.rotate(+45.0);
  painter.setWorldTransform(transform);
  // 会被世界变换转换，然后使用 窗口 - 视口 设置映射到物理。
  painter.drawText(pos, tr("Sales"));
```
如果进行了多次变换，它们会按照给定的顺序生效。例如，如果想要使用点 （50,50） 作为旋转的中心点，可以移动窗口到(+50,+50)，执行旋转，然后在把窗口移回原来的初始位置。
```c++
  QTransform transform;
  transform.translate(+50.0, +50.0);
  transform.rotate(+45.0);
  transform.translate(-50.0, -50.0);
  painter.setWorldTransform(transform);
  painter.drawText(pos, tr("Sales"));
```

坐标转换的一种更为简单的方式是使用 QPainter 的 translate() 、 scale() 、 rotate() 和 shear() 这些简便函数。
```c++
  painter.translate(-50.0, -50.0);
  painter.rotate(+45.0);
  painter.translate(+50.0, +50.0);
  painter.drawText(pos, tr("Sales"));
```
如果想重复使用相同的变换，可以把他们保存到一个 QTransform 对象中，这样会更高效。在需要变换的时候，再把世界变换设置到绘制器上。

示例： OvenTimer 模仿烤箱的定时器。用户可以单击刻度来设置持续时间。转轮会自动地逆时针转到 0， OvenTimer 在一点发射 timeout() 信号。

![](../images/8_painter_202004041814_2.png)

### OvenTimer.h
```c++
#ifndef OVENTIMER_H
#define OVENTIMER_H

#include <QDateTime>
#include <QWidget>

QT_BEGIN_NAMESPACE
class QTimer;
QT_END_NAMESPACE

class OvenTimer : public QWidget
{
    Q_OBJECT

public:
    OvenTimer(QWidget * parent = 0);

    void setDuration(int secs);
    int duration() const;
    void draw(QPainter * painter);

signals:
    void timeout();

protected:
    void paintEvent(QPaintEvent * event) override;
    void mousePressEvent(QMouseEvent * event) override;

private:
    // 保存日期和时间，避免了由于当前时间是在午夜之前并且完成时间是在午夜之后
    // 而产生的折回缺陷(wrap-around bug)
    QDateTime finishTime;
    QTimer * updateTimer;
    QTimer * finishTimer;
};

#endif
```

### OvenTimer.cpp
```c++
#include <QtWidgets>
#include <cmath>

#ifndef M_PI
#define M_PI 3.14159265359
#endif

#include "OvenTimer.h"

const double DegreesPerMinute = 7.0;
const double DegreesPerSecond = DegreesPerMinute / 60;
const int MaxMinutes = 45;
const int MaxSeconds = MaxMinutes * 60;
const int UpdateInterval = 5;

OvenTimer::OvenTimer(QWidget *parent)
    : QWidget(parent)
{
    finishTime = QDateTime::currentDateTime();

    // 用来每隔 5 秒刷新窗口部件的外观
    updateTimer = new QTimer(this);
    connect(updateTimer, SIGNAL(timeout()), this, SLOT(update()));

    // 在定时器达到 0 时发射 timeout() 信号
    finishTimer = new QTimer(this);
    // 设置只执行一次
    finishTimer->setSingleShot(true);
    connect(finishTimer, SIGNAL(timeout()), this, SIGNAL(timeout()));
    // 当定时器停止运行时停止更新窗口部件
    connect(finishTimer, SIGNAL(timeout()), updateTimer, SLOT(stop()));

    QFont font;
    // 设置字体的磅值为 8
    font.setPointSize(8);
    setFont(font);
}

void OvenTimer::setDuration(int secs)
{
    secs = qBound(0, secs, MaxSeconds);
    // 使用qBound(),可以避免这样的代码
    // if (secs < 0) {
    //   secs = 0;
    // } else if (secs > MaxSeconds) {
    //   secs = MaxSeconds;
    // }

    // 得到结束时间
    finishTime = QDateTime::currentDateTime().addSecs(secs);

    if (secs > 0) {
        updateTimer->start(UpdateInterval * 1000);
        finishTimer->start(secs * 1000);
    } else {
        updateTimer->stop();
        finishTimer->stop();
    }
    // 重绘
    update();
}

// 返回定时器完成前剩余的秒数
int OvenTimer::duration() const
{
    int secs = QDateTime::currentDateTime().secsTo(finishTime);
    if (secs < 0)
        secs = 0;
    return secs;
}

void OvenTimer::mousePressEvent(QMouseEvent *event)
{
    // 找到离点击位置最近的刻度
    // 并且使用这个结果来设置新的持续时间
    // 然后安排一个重绘
    QPointF point = event->pos() - rect().center();
    double theta = std::atan2(-point.x(), -point.y()) * 180.0 / M_PI;
    setDuration(duration() + int(theta / DegreesPerSecond));
    update();
}

void OvenTimer::paintEvent(QPaintEvent * /* event */)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);

    int side = qMin(width(), height());

    // 设置视口为正方形。如果没有的话，当窗口部件被缩放为非正方形的矩形时，烤箱定时器就会变成椭圆。
    // 为了避免这种变形，必须把视口和窗口设置成具有相同纵横比的矩形。
    painter.setViewport((width() - side) / 2, (height() - side) / 2,
                        side, side);

    // 设置窗口
    painter.setWindow(-50, -50, 100, 100);

    draw(&painter);
}

void OvenTimer::draw(QPainter *painter)
{
    static const int triangle[3][2] = {
        { -2, -49 }, { +2, -49 }, { 0, -47 }
    };
    QPen thickPen(palette().foreground(), 1.5);
    QPen thinPen(palette().foreground(), 0.5);
    QColor niceBlue(150, 150, 200);

    painter->setPen(thinPen);
    painter->setBrush(palette().foreground());
    // 在窗口部件顶部的 0 位置绘制一个小三角形
    painter->drawPolygon(QPolygon(3, &triangle[0][0]));

    // 绘制外面的圆，用锥形渐变填充。锥形的中心点位于 (0,0) ，角度是 -90 度。
    QConicalGradient coneGradient(0, 0, -90.0);
    coneGradient.setColorAt(0.0, Qt::darkGray);
    coneGradient.setColorAt(0.2, niceBlue);
    coneGradient.setColorAt(0.5, Qt::white);
    coneGradient.setColorAt(1.0, Qt::darkGray);

    painter->setBrush(coneGradient);
    painter->drawEllipse(-46, -46, 92, 92);

    // 用辐射渐变填充内部的圆。渐变的中心点和焦点都在 (0,0) 。渐变半径是 20.
    QRadialGradient haloGradient(0, 0, 20, 0, 0);
    haloGradient.setColorAt(0.0, Qt::lightGray);
    haloGradient.setColorAt(0.8, Qt::darkGray);
    haloGradient.setColorAt(0.9, Qt::white);
    haloGradient.setColorAt(1.0, Qt::black);

    painter->setPen(Qt::NoPen);
    painter->setBrush(haloGradient);
    painter->drawEllipse(-20, -20, 40, 40);

    QLinearGradient knobGradient(-7, -25, 7, -25);
    knobGradient.setColorAt(0.0, Qt::black);
    knobGradient.setColorAt(0.2, niceBlue);
    knobGradient.setColorAt(0.3, Qt::lightGray);
    knobGradient.setColorAt(0.8, Qt::white);
    knobGradient.setColorAt(1.0, Qt::black);

    // 旋转绘图器的坐标系统。
    // 在旧的坐标系统中， 0 minute 的标记在最上面；
    // 现在，这个标记移到了对应着剩余时间的位置
    painter->rotate(duration() * DegreesPerSecond);
    painter->setBrush(knobGradient);
    painter->setPen(thinPen);
    // 绘制矩形把手
    painter->drawRoundRect(-7, -25, 14, 50, 99, 49);

    // 绘制外面圆的边缘标记
    for (int i = 0; i <= MaxMinutes; ++i) {
      painter->save();
        if (i % 5 == 0) {
            painter->setPen(thickPen);
            painter->drawLine(0, -41, 0, -44);
            painter->drawText(-15, -41, 30, 30,
                              Qt::AlignHCenter | Qt::AlignTop,
                              QString::number(i));
        } else {
            painter->setPen(thinPen);
            painter->drawLine(0, -42, 0, -44);
        }
        painter->rotate(-DegreesPerMinute);
    }
    painter->restore();
}

// 添加 painter->save() 和 painter->restore() 为每次迭代保存和加载原始的矩阵。
// 为了避免浮点数的舍入误差不断累积，得到越来越不准确的世界变换。
```
实现烤箱定时器的另外一个办法是自己计算(x,y) 的位置，使用 sin() ,cos() 函数找到圆上的位置。但之后仍然需要利用移动和旋转并以一定的角度来绘制文本。


## 用 QImage 高质量绘图
绘图时，我们可能要面对速度和准确率折中的问题。例如，在 X11 和 Mac OS X 系统中，要在 QWidget 或 QPixmap 上绘图，需要依赖于平台自带的绘图引擎。在 X11 上，这保证了与 X 服务器的通信限制在一个最小集；仅仅发送绘图命令，而不是图像数据。这一方法的主要缺点是 Qt 受限与平台的内在支持：
* 在 X11 上，类似反走样以及对分数坐标的支持只有当 X 服务器上存在 X 渲染扩展时才有效。
* 在 Mac OS X 上，内置的走样绘图引擎使用与 X11 和 windows 不同的算法绘制多边形，绘制结果也稍有不同。

当准确率比效率更为重要的时候，我们可以画到一个 [QImage](https://doc.qt.io/qt-5/qimage.html) 上，然后把结果复制到屏幕上。这样可以总是使用 Qt 自己内置的绘图引擎，在所有平台上得到同样的结果。唯一的限制就是 QImage 在被创建时会用到 QImage::Format_RGB32 或者 QImage::Format_ARGB32_Premultiplied 参数。

预乘 ARGB32 格式与常规的 ARGB32 格式差不多是一样的，不同之处在于 红、绿和蓝通道自左乘 alpha(透明)通道。这意味着，RGB 通道的值，一般是从 0x00 到 0xFF ，变换为从 0x00 到透明通道的值。例如，50% 透明的蓝色用 ARGB32表示为 0x7F0000FF ,但预乘 ARGB32表示为 0x7F00007F。同样的 75% 透明的黑绿用 ARGB32表示为 0x3F008000 ,但预乘 ARGB32表示为 0x3F002000。

即设我们想应用反走样绘制一个窗口部件，并且想在没有 X 渲染扩展的 X11 系统上获得很好的结果。原始的 paintEvent() 处理器，反走样依赖于 X 渲染的代码类似于：
```c++
void MyWidget::paintEvent(QPaintEvent *event)
{
  QPainter painter(this);
  painter.setRenderHint(QPainter::Antialiasing, true);
  draw(&painter);
}
```
使用 Qt 的平台无关的绘图引擎重写窗口部件的 paintEvent() 函数：
```c++
// 这种方法在所有平台上产生同样高质量的结果，对于字体渲染，依赖于安装的字体库
void MyWidget::paintEvent(QPaintEvent *event)
{
  // 我们以预乘 ARGB32 格式创建一个跟窗口部件大小一致的 QImage
  // ,以及一个 QPainter 引用此图像。
  QImage image(size(), QImage::Format_ARGB32_Premultiplied);
  QPainter imagePainter(&image);
  // 初始化画笔背景和字体
  imagePainter.initFrom(this);
  imagePainter.setRenderHint(QPainter::Antialiasing, true);
  imagePainter.eraseRect(rect());
  draw(&imagePainter);
  imagePainter.end();
  QPainter widgetPainter(this);
  // 把图像复制到窗口部件上
  widgetPainter.drawImage(0, 0, image);
}
```

Qt 的图像引擎的一个特别强大的特性是它支持复合模式。这规范了源和目的像素如何在绘制时复合在一起。该模式可以用于所有绘制操作，包括画笔，画刷，渐变以及图像绘制。

默认的复合模式是 QImage::CompositionMode_sourceOver，这意味着源像素（正在绘制的像素）被混合在目的像素（已存在的像素）上，这样，源图像的透明部分给我们以透明效果。下图列举了应用不同模式在（玻璃钢化时的）风嘴印图像（目的图像）上绘制一个半透明的蝴蝶（源图像）的效果。

![](../images/8_painter_202004051057_1.png)

使用 QPainter::setCompositionMode() 可以设置各种复合模式。例如，下面就是如何设置蝴蝶和风嘴印图像 XOR 复合模式的代码：
```c++
  QImage resultImage = checkerPatternImage;
  QPainter painter(&resultImage);
  painter.setCompositionMode(QPainter::CompositionMode_Xor);
  painter.drawImage(0, 0, butterflyImage);
```
值得注意的是，QImage::CompositionMode_Xor 操作也会影响到透明通道。这意味着，如果白色(0xFFFFFFFF)对自己做 XOR 复合，会得到透明色(0x00000000),而不是黑色(0xFF000000)。

## 基于项的图形视图

对于用户自定义的窗口部件和绘制一个或者几个项来说，使用 QPainter 是理想的。在绘图中，如果需要处理从几个到几万的项时，而且要求用户能够单击、拖动和选取项，Qt 的视图类提供了这一问题的解决方案。

[Graphics View Examples](https://doc.qt.io/qt-5/examples-graphicsview.html)

Qt 的视图体系包括一个由  [QGraphicsScene](https://doc.qt.io/qt-5/qgraphicsscene.html) 充当的场景和一些
[QGraphicsItem](https://doc.qt.io/qt-5/qgraphicsitem.html) 的子类充当场景中的项。场景（以及它的项）在视图中显示，这样用户就可以看到了，它由 [QGraphicsView ](https://doc.qt.io/qt-5/qgraphicsview.html) 类充当。同一场景可以在多个视图中显示 - 例如，便于部分的显示一个大的场景，或者以不同的变换来显示场景。eg：

![](../images/8_painter_202004061057_1.png)

Qt 提供了几个预定义的 QGraphicsItem 子类，包括
* [QGraphicsLineItem](https://doc.qt.io/qt-5/qgraphicslineitem.html)
* [QGraphicsPixmapItem](https://doc.qt.io/qt-5/qgraphicspixmapitem.html)
* [QGraphicsSimpleTextItem](https://doc.qt.io/qt-5/qgraphicssimpletextitem.html)（用于纯文本）
* [QGraphicsTextItem](https://doc.qt.io/qt-5/qgraphicstextitem.html)(用于多文本)
* ...

![](../images/8_painter_202004061057_2.png)

QGraphicsScene 是一个图形项的集合。一个场景有三层：背景层(background layer)、顶层(item layer)、前景层(foreground layer)。背景层和前景层通常由 QBrush 指定，但也可能需要重新实现 drawBackground() 和 drawForeground()。如果想要一个图片作为背景，可以简单地创建该图片作为 QBrush 纹理。前景画刷可以设置成半透明的白色，给人一种褪色的效果，或者设置成交叉模式，提供一种格子覆盖的效果。

场景可以告诉我们哪些项是重叠的，哪些是被选取的，以及哪些是在一个特定的点处，或者在一个特定的区域内。场景中的项或者是最高层的项（场景就是其父对象），或者是子项（它们的父对象是另外的项）。任何应用于项的变换都会自动地应用于子对象。

视图体系提供了两种分组项地方法。一种方法是简单地使一个项成为另一个项的子项。另外一种方法是使用 [QGraphicsItemGroup](https://doc.qt.io/qt-5/qgraphicsitemgroup.html) 。

QGraphicsView 是一窗口部件，这个窗口部件可以显示场景，在需要的情况下提供滚动条，以及影响场景绘制方式的变换能力。这有利于支持缩放和旋转，帮助浏览场景。

默认情况下，QGraphicsView 使用 Qt 内置的二维图形引擎绘图，但这可以改变，在其创建完后调用 setViewport() 可改为使用 OpenGL 窗口部件。

这个体系使用三种不同的坐标系统 - 视口坐标、场景坐标、项坐标 - 而且还包含从一个坐标系统映射到另一个坐标的函数。视口坐标是 QGraphicsView 的坐标。场景坐标是逻辑坐标，用来布置场景中的项。项坐标针对某一项，并且以 (0,0) 点为中心。当在场景中移动项时，项坐标保持不变。在示例应用中，我们常常只关心场景坐标（用于布置上层的项），以及项坐标（用于布置子项和绘制项）。依照本身的坐标系统绘制项意味着我们不用去关心项在场景中的位置或者关心需要应用的变换。

视图类用起来很简单，而且具有很强大的功能；eg：
1. 一个简单的图标编辑器，我们将看到怎样创建项，以及怎样处理用户交互。
2. 一个有注解的地图程序，介绍了如何处理大量的地图对象，以及如何以不同的缩放比例高效地绘制它们。

### 图标编辑器

![](../images/8_painter_202004061057_3.png)

该应用程序可以让用户创建节点和 Link。节点就是项，是可以在内部显示文本的圆角矩形，而 Link 是连接两个节点的线。被选中的节点用比不同线粗的虚线边缘表示。

Link.h:
```c++
#ifndef LINK_H
#define LINK_H

#include <QGraphicsLineItem>

QT_BEGIN_NAMESPACE
class Node;
QT_END_NAMESPACE

// QGraphicsLineItem 不是 QObject 的子类，
// 但如果需要在 Link 类中添加信号和槽，
// 可以使用 QObject 做多重继承
class Link : public QGraphicsLineItem
{
public:
    Link(Node * fromNode, Node * toNode);
    ~Link();

    Node * fromNode() const;
    Node * toNode() const;

    void setColor(const QColor &color);
    QColor color() const;

    void trackNodes();

private:
    Node * myFromNode;
    Node * myToNode;
};

#endif

```

Link.cpp:
```c++
#include <QtWidgets>

#include "Link.h"
#include "Node.h"

Link::Link(Node *fromNode, Node *toNode)
{
    myFromNode = fromNode;
    myToNode = toNode;

    // 每个节点保存一个 Link 的集合，而且可以保存任意数量的 Link 。
    myFromNode->addLink(this);
    myToNode->addLink(this);

    // 图形项有几个标识：但在这种情况下，只需要 Link 可以被选中，从而用户可以选中并删除它。
    setFlags(QGraphicsItem::ItemIsSelectable);

    // 每个项都有一个 (x,y) 坐标，以及一个 z 值，指定他在场景中的前后位置。因为我们打算
    // 从一个节点的中心向另一个节点的中心画线，所以给直线一个负的z值，这样它就会被绘制到
    // 所连接的节点的下面。这样，Link 就是它所连接的节点与最近边框之间的线。
    setZValue(-1);

    setColor(Qt::darkRed);
    trackNodes();
}

Link::~Link()
{
    myFromNode->removeLink(this);
    myToNode->removeLink(this);
}

Node *Link::fromNode() const
{
    return myFromNode;
}

Node *Link::toNode() const
{
    return myToNode;
}

void Link::setColor(const QColor &color)
{
    setPen(QPen(color, 1.0));
}

QColor Link::color() const
{
    return pen().color();
}

// 在用户拖动一个连接点到一个不同的位置时，用来更新线的端点
void Link::trackNodes()
{
    // QGraphicsItem::pos() 函数返回项相对于场景的位置（针对上层项），或者相对于父项的位置（针对子项）。
    setLine(QLineF(myFromNode->pos(), myToNode->pos()));
}

```


Node.h:
```c++
#ifndef NODE_H
#define NODE_H

#include <QApplication>
#include <QColor>
#include <QGraphicsItem>
#include <QSet>

QT_BEGIN_NAMESPACE
class Link;
QT_END_NAMESPACE

class Node : public QGraphicsItem
{
    // 用来给类添加一个  tr() 函数，尽管它不是 QObject 的子类。
    Q_DECLARE_TR_FUNCTIONS(Node)

public:
    Node();
    ~Node();

    void setText(const QString &text);
    QString text() const;
    void setTextColor(const QColor &color);
    QColor textColor() const;
    // 节点边缘
    void setOutlineColor(const QColor &color);
    QColor outlineColor() const;
    void setBackgroundColor(const QColor &color);
    QColor backgroundColor() const;

    void addLink(Link * link);
    void removeLink(Link * link);

    // 自己实现绘图
    // 视图体系用外接矩形来决定一个项是否需要被绘制。这使得 QGraphicsView 可以很迅速地显示任意大的场景
    //，尽管此时只有一小部分是可见的。
    // shape() 用来决定一个点是否在项内，或者是否两个项是重合的。
    QRectF boundingRect() const override;
    QPainterPath shape() const override;
    void paint(QPainter * painter,
               const QStyleOptionGraphicsItem * option, QWidget * widget) override;

protected:
    // 允许双击节点修改文字
    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent * event) override;
    // 如果节点被移动了，必须确保与其连接的 Link 做相应的更新。每当项的属性（包括其位置）改变时被调用。
    QVariant itemChange(GraphicsItemChange change,
                        const QVariant &value) override;

private:
    // 返回由节点绘制的矩形
    QRectF outlineRect() const;
    // 返回一个基于矩形宽度和高度的合适的圆度系数
    int roundness(double size) const;

    QSet<Link * > myLinks;
    QString myText;
    QColor myTextColor;
    QColor myBackgroundColor;
    QColor myOutlineColor;
};

#endif
```

Node.cpp:
```c++
#include <QtWidgets>

#include "Link.h"
#include "Node.h"

Node::Node()
{
    myTextColor = Qt::darkGreen;
    myOutlineColor = Qt::darkBlue;
    myBackgroundColor = Qt::white;

    setFlags(ItemIsMovable | ItemIsSelectable | ItemSendsGeometryChanges);
}

Node::~Node()
{
    foreach (Link *link, myLinks)
        delete link;
}

// 无论何时修改了项，都会影响到它的显示，所以必须调用 update() 来安排一个重绘。
// 例如项的外接矩形可能会改变（因为新的文字可能比现在的文字短或者长），必须在做
// 影响项的外接矩形的修改之前立即调用 prepareGeometryChange().
void Node::setText(const QString &text)
{
    prepareGeometryChange();
    myText = text;
    update();
}

QString Node::text() const
{
    return myText;
}

void Node::setTextColor(const QColor &color)
{
    // 没有必要调用 prepareGeometryChange() ，
    // 因为项的大小不会受颜色改变的影响。
    myTextColor = color;
    // 安排重绘，以便使用新的颜色绘制项
    update();
}

QColor Node::textColor() const
{
    return myTextColor;
}

void Node::setOutlineColor(const QColor &color)
{
    myOutlineColor = color;
    update();
}

QColor Node::outlineColor() const
{
    return myOutlineColor;
}

void Node::setBackgroundColor(const QColor &color)
{
    myBackgroundColor = color;
    update();
}

QColor Node::backgroundColor() const
{
    return myBackgroundColor;
}

void Node::addLink(Link *link)
{
    myLinks.insert(link);
}

void Node::removeLink(Link *link)
{
    myLinks.remove(link);
}

// 会由 QGraphicsView 调用，以决定是否需要绘制项。
// 我们使用边缘矩形，但留些额外的边白，因为如果需要绘制边缘，
// 由这个函数返回的矩形必须留出至少半个画笔宽的距离。
QRectF Node::boundingRect() const
{
    const int Margin = 1;
    return outlineRect().adjusted(-Margin, -Margin, +Margin, +Margin);
}

// 由 QGraphicsView 调用，用来做精确的碰撞检测。通常，可以忽略它，由项基于外接矩形自行计算形状。
// 这里重新实现了它，由其 返回一个 QPainterPath 对象，该对象代表了一个圆角矩形。
// 因此，如果点击圆角矩形外、外接矩形内的区域则不会选中项。
QPainterPath Node::shape() const
{
    QRectF rect = outlineRect();

    QPainterPath path;
    path.addRoundRect(rect, roundness(rect.width()),
                      roundness(rect.height()));
    return path;
}

// QStyleOptionGraphicsItem 类型是一个不寻常的类，因为它提供了几个公有成员变量。
// 这包括当前的布局方向、字体规格、调色板、矩形、状态、变换矩阵和细节级别。
void Node::paint(QPainter *painter,
                 const QStyleOptionGraphicsItem *option,
                 QWidget * /* widget */)
{
    QPen pen(myOutlineColor);
    // 是否被选中
    if (option->state & QStyle::State_Selected) {
        pen.setStyle(Qt::DotLine);
        pen.setWidth(2);
    }
    painter->setPen(pen);
    painter->setBrush(myBackgroundColor);

    QRectF rect = outlineRect();
    painter->drawRoundRect(rect, roundness(rect.width()),
                           roundness(rect.height()));

    painter->setPen(myTextColor);
    painter->drawText(rect, Qt::AlignCenter, myText);
}

void Node::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)
{
    QString text = QInputDialog::getText(event->widget(),
                           tr("Edit Text"), tr("Enter new text:"),
                           QLineEdit::Normal, myText);
    if (!text.isEmpty())
        setText(text);
}

// 一旦用户拖动一个节点，就会调用 itemChange()
QVariant Node::itemChange(GraphicsItemChange change,
                          const QVariant &value)
{
    if (change == ItemPositionHasChanged) {
        foreach (Link *link, myLinks)
            link->trackNodes();
    }
    return QGraphicsItem::itemChange(change, value);
}

QRectF Node::outlineRect() const
{
    // 该矩形由 8 像素的边距
    const int Padding = 8;
    QFontMetricsF metrics = static_cast<QFontMetricsF>(qApp->font());
    QRectF rect = metrics.boundingRect(myText);
    rect.adjust(-Padding, -Padding, +Padding, +Padding);
    rect.translate(-rect.center());
    return rect;
}

// 计算合适的圆角率,确保节点的转角是直径为 12 的 四分之一圆。
// 圆角的范围必须在 0 （直角形） 到 99 （满圆形）之间。
int Node::roundness(double size) const
{
    const int Diameter = 12;
    return 100 * Diameter / int(size);
}
```

DiagramWindow.h:
```c++
#ifndef DIAGRAMWINDOW_H
#define DIAGRAMWINDOW_H

#include <QMainWindow>
#include <QPair>

QT_BEGIN_NAMESPACE
class QAction;
class QGraphicsItem;
class QGraphicsScene;
class QGraphicsView;
class Link;
class Node;
QT_END_NAMESPACE

class DiagramWindow : public QMainWindow
{
    Q_OBJECT

public:
    DiagramWindow();

private slots:
    void addNode();
    void addLink();
    void del();
    void cut();
    void copy();
    void paste();
    void bringToFront();
    void sendToBack();
    void properties();
    void updateActions();

private:
    typedef QPair<Node * , Node * > NodePair;

    void createActions();
    void createMenus();
    void createToolBars();
    void setZValue(int z);
    void setupNode(Node * node);
    Node * selectedNode() const;
    Link * selectedLink() const;
    NodePair selectedNodePair() const;

    QMenu * fileMenu;
    QMenu * editMenu;
    QToolBar * editToolBar;
    QAction * exitAction;
    QAction * addNodeAction;
    QAction * addLinkAction;
    QAction * deleteAction;
    QAction * cutAction;
    QAction * copyAction;
    QAction * pasteAction;
    QAction * bringToFrontAction;
    QAction * sendToBackAction;
    QAction * propertiesAction;

    QGraphicsScene * scene;
    QGraphicsView * view;

    int minZ;
    int maxZ;
    int seqNumber;
};

#endif
```

DiagramWindow.cpp:
```c++
#include <QtWidgets>

#include "diagramwindow.h"
#include "Link.h"
#include "Node.h"
#include "Propertiesdialog.h"

DiagramWindow::DiagramWindow()
{
    // 创建一个原点（0,0）宽为 600 ，高为 500 的场景
    scene = new QGraphicsScene(0, 0, 600, 500);

    view = new QGraphicsView;
    view->setScene(scene);
    // 可以通过圈选选中它们
    view->setDragMode(QGraphicsView::RubberBandDrag);
    view->setRenderHints(QPainter::Antialiasing
                         | QPainter::TextAntialiasing);
    view->setContextMenuPolicy(Qt::ActionsContextMenu);
    setCentralWidget(view);

    minZ = 0;
    maxZ = 0;
    seqNumber = 0;

    createActions();
    createMenus();
    createToolBars();

    connect(scene, SIGNAL(selectionChanged()),
            this, SLOT(updateActions()));

    setWindowTitle(tr("Diagram"));
    updateActions();
}

void DiagramWindow::addNode()
{
    Node * node = new Node;
    node->setText(tr("Node %1").arg(seqNumber + 1));
    setupNode(node);
}

void DiagramWindow::addLink()
{
    NodePair nodes = selectedNodePair();
    if (nodes == NodePair())
        return;

    Link * link = new Link(nodes.first, nodes.second);
    scene->addItem(link);
}

void DiagramWindow::del()
{
    QList<QGraphicsItem * > items = scene->selectedItems();
    QMutableListIterator<QGraphicsItem * > i(items);
    while (i.hasNext()) {
        Link * link = dynamic_cast<Link * >(i.next());
        if (link) {
            delete link;
            i.remove();
        }
    }

    qDeleteAll(items);
}

void DiagramWindow::cut()
{
    Node * node = selectedNode();
    if (!node)
        return;

    copy();
    delete node;
}

void DiagramWindow::copy()
{
    Node * node = selectedNode();
    if (!node)
        return;

    QString str = QString("Node %1 %2 %3 %4")
                  .arg(node->textColor().name())
                  .arg(node->outlineColor().name())
                  .arg(node->backgroundColor().name())
                  .arg(node->text());
    QApplication::clipboard()->setText(str);
}

void DiagramWindow::paste()
{
    QString str = QApplication::clipboard()->text();
    QStringList parts = str.split(" ");

    if (parts.count() >= 5 && parts.first() == "Node") {
        Node * node = new Node;
        node->setText(QStringList(parts.mid(4)).join(" "));
        node->setTextColor(QColor(parts[1]));
        node->setOutlineColor(QColor(parts[2]));
        node->setBackgroundColor(QColor(parts[3]));
        setupNode(node);
    }
}

// 比任何其他节点都靠前
void DiagramWindow::bringToFront()
{
    ++maxZ;
    setZValue(maxZ);
}

// 比任何其他节点都靠后
void DiagramWindow::sendToBack()
{
    --minZ;
    setZValue(minZ);
}

void DiagramWindow::properties()
{
    Node * node = selectedNode();
    Link * link = selectedLink();

    if (node) {
        PropertiesDialog dialog(node, this);
        dialog.exec();
    } else if (link) {
        QColor color = QColorDialog::getColor(link->color(), this);
        if (color.isValid())
            link->setColor(color);
    }
}

void DiagramWindow::updateActions()
{
    bool hasSelection = !scene->selectedItems().isEmpty();
    bool isNode = (selectedNode() != 0);
    bool isNodePair = (selectedNodePair() != NodePair());

    cutAction->setEnabled(isNode);
    copyAction->setEnabled(isNode);
    addLinkAction->setEnabled(isNodePair);
    deleteAction->setEnabled(hasSelection);
    bringToFrontAction->setEnabled(isNode);
    sendToBackAction->setEnabled(isNode);
    propertiesAction->setEnabled(isNode);

    foreach (QAction *action, view->actions())
        view->removeAction(action);

    foreach (QAction *action, editMenu->actions()) {
        if (action->isEnabled())
            view->addAction(action);
    }
}

void DiagramWindow::createActions()
{
    exitAction = new QAction(tr("E&xit"), this);
    exitAction->setShortcut(tr("Ctrl+Q"));
    connect(exitAction, SIGNAL(triggered()), this, SLOT(close()));

    addNodeAction = new QAction(tr("Add &Node"), this);
    addNodeAction->setIcon(QIcon(":/images/node.png"));
    addNodeAction->setShortcut(tr("Ctrl+N"));
    connect(addNodeAction, SIGNAL(triggered()), this, SLOT(addNode()));

    addLinkAction = new QAction(tr("Add &Link"), this);
    addLinkAction->setIcon(QIcon(":/images/link.png"));
    addLinkAction->setShortcut(tr("Ctrl+L"));
    connect(addLinkAction, SIGNAL(triggered()), this, SLOT(addLink()));

    deleteAction = new QAction(tr("&Delete"), this);
    deleteAction->setIcon(QIcon(":/images/delete.png"));
    deleteAction->setShortcut(tr("Del"));
    connect(deleteAction, SIGNAL(triggered()), this, SLOT(del()));

    cutAction = new QAction(tr("Cu&t"), this);
    cutAction->setIcon(QIcon(":/images/cut.png"));
    cutAction->setShortcut(tr("Ctrl+X"));
    connect(cutAction, SIGNAL(triggered()), this, SLOT(cut()));

    copyAction = new QAction(tr("&Copy"), this);
    copyAction->setIcon(QIcon(":/images/copy.png"));
    copyAction->setShortcut(tr("Ctrl+C"));
    connect(copyAction, SIGNAL(triggered()), this, SLOT(copy()));

    pasteAction = new QAction(tr("&Paste"), this);
    pasteAction->setIcon(QIcon(":/images/paste.png"));
    pasteAction->setShortcut(tr("Ctrl+V"));
    connect(pasteAction, SIGNAL(triggered()), this, SLOT(paste()));

    bringToFrontAction = new QAction(tr("Bring to &Front"), this);
    bringToFrontAction->setIcon(QIcon(":/images/bringtofront.png"));
    connect(bringToFrontAction, SIGNAL(triggered()),
            this, SLOT(bringToFront()));

    sendToBackAction = new QAction(tr("&Send to Back"), this);
    sendToBackAction->setIcon(QIcon(":/images/sendtoback.png"));
    connect(sendToBackAction, SIGNAL(triggered()),
            this, SLOT(sendToBack()));

    propertiesAction = new QAction(tr("P&roperties..."), this);
    connect(propertiesAction, SIGNAL(triggered()),
            this, SLOT(properties()));
}

void DiagramWindow::createMenus()
{
    fileMenu = menuBar()->addMenu(tr("&File"));
    fileMenu->addAction(exitAction);

    editMenu = menuBar()->addMenu(tr("&Edit"));
    editMenu->addAction(addNodeAction);
    editMenu->addAction(addLinkAction);
    editMenu->addAction(deleteAction);
    editMenu->addSeparator();
    editMenu->addAction(cutAction);
    editMenu->addAction(copyAction);
    editMenu->addAction(pasteAction);
    editMenu->addSeparator();
    editMenu->addAction(bringToFrontAction);
    editMenu->addAction(sendToBackAction);
    editMenu->addSeparator();
    editMenu->addAction(propertiesAction);
}

void DiagramWindow::createToolBars()
{
    editToolBar = addToolBar(tr("Edit"));
    editToolBar->addAction(addNodeAction);
    editToolBar->addAction(addLinkAction);
    editToolBar->addAction(deleteAction);
    editToolBar->addSeparator();
    editToolBar->addAction(cutAction);
    editToolBar->addAction(copyAction);
    editToolBar->addAction(pasteAction);
    editToolBar->addSeparator();
    editToolBar->addAction(bringToFrontAction);
    editToolBar->addAction(sendToBackAction);
}

void DiagramWindow::setZValue(int z)
{
    Node * node = selectedNode();
    if (node)
        node->setZValue(z);
}

void DiagramWindow::setupNode(Node *node)
{
    node->setPos(QPoint(80 + (100 * (seqNumber % 5)),
                        80 + (50 * ((seqNumber / 5) % 7))));
    scene->addItem(node);
    ++seqNumber;

    scene->clearSelection();
    node->setSelected(true);
    // 确保了新节点比任何其他节点都靠前
    bringToFront();
}

Node *DiagramWindow::selectedNode() const
{
    // 获取被选中的节点列表
    QList<QGraphicsItem * > items = scene->selectedItems();
    if (items.count() == 1) {
        return dynamic_cast<Node * >(items.first());
    } else {
        return 0;
    }
}

Link *DiagramWindow::selectedLink() const
{
    QList<QGraphicsItem * > items = scene->selectedItems();
    if (items.count() == 1) {
        return dynamic_cast<Link * >(items.first());
    } else {
        return 0;
    }
}

DiagramWindow::NodePair DiagramWindow::selectedNodePair() const
{
    QList<QGraphicsItem * > items = scene->selectedItems();
    if (items.count() == 2) {
        Node * first = dynamic_cast<Node * >(items.first());
        Node * second = dynamic_cast<Node * >(items.last());
        if (first && second)
            return NodePair(first, second);
    }
    return NodePair();
}

```

### CityScape

Cityscape 显示了城市中的建筑物、街区和公园的虚拟地图，最为重要的那些建筑会用它们的名字加以标注。它可以让用户通过鼠标和键盘来移动和缩放地图。

![](../images/8_painter_202004091707_1.png)

CityScape.h:
```c++
#ifndef CITYSCAPE_H
#define CITYSCAPE_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
class QGraphicsScene;
class CityView;
QT_END_NAMESPACE

class Cityscape : public QMainWindow
{
    Q_OBJECT

public:
    Cityscape();

private:
    void generateCityBlocks();

    QGraphicsScene * scene;
    // 从 QGraphicsView 类派生过来。
    CityView * view;
};

#endif
```

Cityscape.cpp :
```c++
#include <QtWidgets>

#include "Annotation.h"
#include "CityBlock.h"
#include "CityScape.h"
#include "CityView.h"

Cityscape::Cityscape()
{
    scene = new QGraphicsScene(-22.25, -22.25, 1980, 1980);
    scene->setBackgroundBrush(QColor(255, 255, 238));
    generateCityBlocks();

    view = new CityView;
    view->setScene(scene);
    setCentralWidget(view);

    setWindowTitle(tr("Cityscape"));
}

// 生成一副地图，这幅地图包含 2000 个街区和 200 个标注
void Cityscape::generateCityBlocks()
{
    QSet<QString> names;
    names << "Adams" << "Agnew" << "Arthur" << "Breckinridge"
          << "Buchanan" << "Burr" << "Bush" << "Calhoun" << "Carter"
          << "Cheney" << "Cleveland" << "Clinton" << "Colfax"
          << "Coolidge" << "Curtis" << "Dallas" << "Dawes"
          << "Eisenhower" << "Fairbanks" << "Fillmore" << "Ford"
          << "Garfield" << "Garner" << "Gerry" << "Gore" << "Grant"
          << "Hamlin" << "Harding" << "Harrison" << "Hayes"
          << "Hendricks" << "Hobart" << "Hoover" << "Humphrey"
          << "Jackson" << "Jefferson" << "Johnson" << "Kennedy"
          << "King" << "Lincoln" << "Madison" << "Marshall"
          << "McKinley" << "Mondale" << "Monroe" << "Morton"
          << "Nixon" << "Pierce" << "Polk" << "Quayle" << "Reagan"
          << "Rockefeller" << "Roosevelt" << "Sherman" << "Stevenson"
          << "Taft" << "Taylor" << "Tompkins" << "Truman" << "Tyler"
          << "Van Buren" << "Wallace" << "Washington" << "Wheeler"
          << "Wilson";

    QSetIterator<QString> i(names);
    for (int y = 0; y < 44; ++y) {
        for (int x = 0; x < 44; ++x) {
            int percentile;
            if (x > 20 && x < 24 && y > 20 && y < 24) {
                percentile = std::rand() % (std::rand() % 2 != 0
                                            ? 10 : 100);
            } else if (x > 18 && x < 26 && y > 18 && y < 26) {
                percentile = std::rand() % (rand() % 3 != 0
                                            ? 10 : 100);
            } else if (x > 15 && x < 29 && y > 15 && y < 29) {
                percentile = std::rand() % (std::rand() % 5 != 0
                                            ? 10 : 100);
            } else {
                percentile = std::rand() % 100;
            }

            CityBlock::Kind kind;
            QString name;

            if (percentile == 0) {
                kind = CityBlock::Park;
                name = tr("%1 Park");
            } else if (percentile <= 2) {
                kind = CityBlock::SmallBuilding;
            } else if (percentile <= 4) {
                kind = CityBlock::Hospital;
                name = tr("%1 Hospital");
            } else if (percentile == 5) {
                kind = CityBlock::Hall;
                name = tr("%1 Hall");
            } else if (percentile <= 7) {
                kind = CityBlock::Building;
                name = tr("%1 Bldg");
            } else if (percentile <= 9) {
                kind = CityBlock::Tower;
                name = tr("%1 Tower");
            } else if (percentile <= 15) {
                kind = CityBlock::LShapedBlock;
            } else if (percentile <= 30) {
                kind = CityBlock::LShapedBlockPlusSmallBlock;
            } else if (percentile <= 70) {
                kind = CityBlock::TwoBlocks;
            } else {
                kind = CityBlock::BlockPlusTwoSmallBlocks;
            }

            CityBlock * block = new CityBlock(kind);
            block->setPos(QPointF(x * 44.5, y * 44.5));
            scene->addItem(block);

            if (!name.isEmpty()) {
                if (!i.hasNext())
                    i.toFront();

                bool major = (std::rand() % 10 == 0);
                Annotation * annotation =
                        new Annotation(name.arg(i.next()), major);
                annotation->setPos(block->pos());
                scene->addItem(annotation);
            }
        }
    }
}
```

CityBlock.h:
```c++
#ifndef CITYBLOCK_H
#define CITYBLOCK_H

#include <QColor>
#include <QGraphicsItem>
#include <QPainterPath>

QT_BEGIN_NAMESPACE
class QGradient;
QT_END_NAMESPACE

class CityBlock : public QGraphicsItem
{
public:
    enum Kind { Park, SmallBuilding, Hospital, Hall, Building, Tower,
                LShapedBlock, LShapedBlockPlusSmallBlock, TwoBlocks,
                BlockPlusTwoSmallBlocks };

    CityBlock(Kind kind);

    QRectF boundingRect() const override;
    void paint(QPainter * painter,
               const QStyleOptionGraphicsItem * option, QWidget * widget) override;

private:
    int kind;
    QColor color;
    QPainterPath shape;
};

#endif
```
一个城市的街区会有类型、颜色和形状属性。因为城市街区是不可选的，所以不用担心像前面的例子中 Node类那样去重新实现 shape() 函数。

CityBlock.cpp:
```c++
#include <QtWidgets>
#include <cmath>

#include "CityBlock.h"

CityBlock::CityBlock(Kind kind)
{
    this->kind = kind;

    // 设置一种随机颜色，并根据需要显示的街区的类型生成一个合适的 QPainterPath.
    int green = 96 + (std::rand() % 64);
    int red = 16 + green + (std::rand() % 64);
    int blue = 16 + (std::rand() % green);
    color = QColor(red, green, blue);

    if (kind == Park) {
        color = QColor(192 + (std::rand() % 32), 255,
                       192 + (std::rand() % 16));
        shape.addRect(boundingRect());
    } else if (kind == SmallBuilding) {
        QRectF block(-7.5, -7.5, 15, 15);
        block.moveBottomLeft(QPointF((std::rand() % 6) - 3,
                                     (std::rand() % 6) - 3));
        shape.addRect(block);
    } else if (kind == Hospital) {
        int a = (std::rand() % 6) + 10;
        int b = (std::rand() % 6) + 10;
        QPolygonF block;
        block << QPointF(-5, -a) << QPointF(-5, -5) << QPointF(-10, -5)
              << QPointF(-10, 5) << QPointF(-5, 5)  << QPointF(-5, 10)
              << QPointF(5, 10)  << QPointF(5, 5)   << QPointF(b, 5)
              << QPointF(b, -5)  << QPointF(5, -5)  << QPointF(5, -a);
        shape.addPolygon(block);
    } else if (kind == Hall) {
        int padding1 = (std::rand() % 8) + 2;
        int padding2 = (std::rand() % 8) + 2;
        shape.addEllipse(boundingRect().adjusted(+padding1, +padding1,
                                                 -padding2, -padding2));
    } else if (kind == Building) {
        shape.addRect(boundingRect());
    } else if (kind == Tower) {
        int padding1 = (std::rand() % 8) + 2;
        int padding2 = (std::rand() % 8) + 2;
        shape.addRect(boundingRect().adjusted(+padding1, +padding1,
                                              -padding2, -padding2));
    } else if (kind == LShapedBlock
               || kind == LShapedBlockPlusSmallBlock) {
        int a = (std::rand() % 6) + 10;
        int b = (std::rand() % 6) + 10;
        int s = qMin(a, b) / 2;
        QPolygonF block;
        block << QPointF(-a, -a) << QPointF(-a, +a) << QPointF(-s, +a)
              << QPointF(-s, -s) << QPointF(+b, -s) << QPointF(+b, -a);
        shape.addPolygon(block);
        if (kind == LShapedBlockPlusSmallBlock) {
            int inset = (std::rand() % 4) + 4;
            shape.addRect(QRectF(-s + inset, -s + inset, a, b));
        }
    } else if (kind == TwoBlocks) {
        int w1 = (std::rand() % 10) + 8;
        int h1 = (std::rand() % 28) + 8;
        int w2 = (std::rand() % 10) + 8;
        int h2 = (std::rand() % 24) + 8;
        shape.addRect(QRectF(-16, -16, w1, h1));
        shape.addRect(QRectF(-16 + w1 + 4, -16 + (std::rand() % 4),
                             w2, h2));
    } else if (kind == BlockPlusTwoSmallBlocks) {
        int w1 = (std::rand() % 10) + 8;
        int h1 = (std::rand() % 28) + 8;
        int w2 = (std::rand() % 10) + 8;
        int h2 = (std::rand() % 10) + 8;
        int w3 = (std::rand() % 6) + 8;
        int h3 = (std::rand() % 6) + 8;
        int y = (std::rand() % 4) - 16;
        shape.addRect(QRectF(-16, -16, w1, h1));
        shape.addRect(QRectF(-16 + w1 + 4, y, w2, h2));
        shape.addRect(QRectF(-16 + w1 + 4,
                             y + h2 + 4 + (std::rand() % 4), w3, h3));
    }
}

// 每个街区都会占用一个 40 x 40 的正方形方格，其中心点是 (0,0);
QRectF CityBlock::boundingRect() const
{
    return QRectF(-20, -20, 40, 40);
}

void CityBlock::paint(QPainter *painter,
                      const QStyleOptionGraphicsItem *option,
                      QWidget * /* widget */)
{
    // option->levelOfDetail ，1.0 表示以原始大小显示场景，0.5 意味着以原始大小的一半显示场景，
    // 2.5 表示该显示的大小是原始尺寸的 2.5 倍。
    // 使用“详细程度”信息可以让我们在那些放得太大以至于不能显示细节的场景中使用更快的绘图算法。
    // 如果缩放因子比 4.0 小，则使用纯色填充图形
    if (option->levelOfDetail < 4.0) {
        painter->fillPath(shape, color);
    } else {
        // 使用 QLinearGradient 填充图形，以产生一种奇妙的观照效果。
        QLinearGradient gradient(QPoint(-20, -20), QPoint(+20, +20));
        int coeff = 105 + int(std::log(option->levelOfDetail - 4.0));
        gradient.setColorAt(0.0, color.lighter(coeff));
        gradient.setColorAt(1.0, color.darker(coeff));
        painter->fillPath(shape, gradient);
    }
}
```

CityBlock 类很好用，但事实上，当场景缩放时造成项也被缩放，这样就给显示文字的那些项带来了问题。一般情况下，我们不希望文字随着场景缩放。视图体系为这一问题提供了通用的解决方式，即使用 ItemIgnoresTransformations 标识。Annotation 类就使用此标识：

Annotation.h:
```c++
#ifndef ANNOTATION_H
#define ANNOTATION_H

#include <QGraphicsItem>

class Annotation : public QGraphicsItem
{
public:
    Annotation(const QString &text, bool major = false);

    void setText(const QString &text);
    QString text() const;

    QRectF boundingRect() const override;
    void paint(QPainter * painter,
               const QStyleOptionGraphicsItem * option, QWidget * widget) override;

private:
    QFont font;
    QString str;
    // 该注解是主注解还是辅注解，这将影响到字体的大小
    bool major;
    double threshold;
    int y;
};

#endif
```
Annotation.cpp:
```c++
#include <QtWidgets>
#include <cmath>

#include "Annotation.h"

Annotation::Annotation(const QString &text, bool major)
{
    font = qApp->font();
    font.setBold(true);
    if (major) {
        // 设置成较大而且加粗的字体
        font.setPointSize(font.pointSize() + 2);
        font.setStretch(QFont::SemiExpanded);
    }

    if (major) {
        threshold = 0.01 * (40 + (std::rand() % 40));
    } else {
        threshold = 0.01 * (100 + (std::rand() % 100));
    }

    str = text;
    this->major = major;
    y = 20 - (std::rand() % 40);

    // 设置 z 的值为 1000，以确保注解显示在最上面，而且使用 ItemIgnoresTransformations
    // 标识来确保无论场景如何缩放，注解的大小都可以保持不变。

    setZValue(1000);
    setFlag(ItemIgnoresTransformations, true);
}

void Annotation::setText(const QString &text)
{
    // 文字可能比以前更长或者更短了
    prepareGeometryChange();
    str = text;
    update();
}

QString Annotation::text() const
{
    return str;
}

QRectF Annotation::boundingRect() const
{
    // 根据字体计算出文字的外接矩形
    QFontMetricsF metrics(font);
    QRectF rect = metrics.boundingRect(str);
    rect.moveCenter(QPointF(0, y));
    // 外接矩形左边和右边的额外像素使文字相对于边框还有一些空间
    rect.adjust(-4, 0, +4, 0);
    return rect;
}

void Annotation::paint(QPainter *painter,
                       const QStyleOptionGraphicsItem *option,
                       QWidget * /* widget */)
{
    // 如果场景缩小的尺寸超过了注解的阈值，就不再绘制注解。
    // 如果场景被放得足够大，就事先绘制一个半透明的白色矩形，这有助于在黑色的块上显示文字。
    if (option->levelOfDetail <= threshold)
        return;

    painter->setFont(font);

    QRectF rect = boundingRect();

    int alpha = int(30 * std::log(option->levelOfDetail));
    if (alpha >= 32)
        painter->fillRect(rect, QColor(255, 255, 255, qMin(alpha, 63)));

    // 我们绘制文字两次，一次用白色，一次用蓝色。白色的文字在水平和竖直方向上平移一个像素来创建一个阴影效果
    // ，使文字更容易读出来。
    painter->setPen(Qt::white);
    painter->drawText(rect.translated(+1, +1), str,
                      QTextOption(Qt::AlignCenter));
    painter->setPen(Qt::blue);
    painter->drawText(rect, str, QTextOption(Qt::AlignCenter));
}
```

CityView 类用于提供用户使用鼠标缩放的功能：

CityView.h:
```c++
#ifndef CITYVIEW_H
#define CITYVIEW_H

#include <QGraphicsView>

class CityView : public QGraphicsView
{
    Q_OBJECT

public:
    CityView(QWidget * parent = 0);

protected:
    void wheelEvent(QWheelEvent * event) override;
};

#endif
```

CityView.cpp:
```c++
#include <QtWidgets>
#include <cmath>

#include "CityView.h"

CityView::CityView(QWidget *parent)
    : QGraphicsView(parent)
{
    // 支持通过鼠标拖动来滚动屏幕
    setDragMode(ScrollHandDrag);
}

void CityView::wheelEvent(QWheelEvent *event)
{
    // 按照滚轴步长的 1.125 倍上下移动场景
    double numDegrees = -event->delta() / 8.0;
    double numSteps = numDegrees / 15.0;
    double factor = std::pow(1.125, numSteps);
    scale(factor, factor);
}
```

这样就完成了两个图形视图的例子。Qt 的图形视图体系内容很多，因此还有很多这里没有篇幅去介绍的。比如支持拖动和释放操作，图形项可以有提示和自定义光标。动画效果可以通过几种方式实现 - 例如，在希望显示动画的项上使用 [QGraphicsItemAnimation](https://doc.qt.io/qt-5/qgraphicsitemanimation.html),使用 [QTimeLine](https://doc.qt.io/qt-5/qtimeline.html) 播放动画。也可以通过继承 QObject(应用多重继承)创建图形项的子类，重新实现 QObject::timeEvent() 显示动画。

## 打印
Qt 中的打印与在 QWidget、QPixmap 或者 QImage 上的绘制非常相似。它包括以下步骤：
1. 创建一个当作绘制设备的 [QPrinter](https://doc.qt.io/qt-5/qprinter.html)
2. 弹出一个 [QPrintDialog](https://doc.qt.io/qt-5/qprintdialog.html) 对话框，以允许用户选择打印机并且设置一些选项
3. 创建一个在 QPrinter 上操作的 QPainter.
4. 使用 QPainter 绘制一页。
5. 调用 QPainter::newPage() 来进行下一页的绘制。
6. 重复步骤 4 和步骤5,直到所有页都被打印为止。

在 Windows 和 Mac OS X 上，QPrinter 会使用系统的打印机驱动程序。在 UNIX 上，它会产生 PostScript 并且把它发送给 lp 或者 lpr 命令【或者是由 QPrinter::setPrintProgram() 设置的其他程序】。QPrinter 也可以通过调用 setOutputFormat(QPrinter::PdfFormat)来生成PDF文件。

一个只有单页打印的简单实例，用于打印一个 QImage ：
```c++
void PrintWindow::printImage(const QImage &image)
{
  QPrintDialog printDialog(&printer, this);
  if (printDialog.exec()) {
    QPainter painter(&printer);
    QRect rect = painter.viewport();
    QSize size = image.size();
    size.scale(rect.size(), Qt::KeepAspectRatio);
    painter.setViewport(rect.x(), rect.y(),
    size.width(), size.height());
    painter.setWindow(image.rect());
    painter.drawImage(0, 0, image);
  }
}
```
打印不超过一页的项是非常简单的，但是很多应用程序通常都需要打印多页。对于这些情况，需要一次绘制一页并且调用 newPage() 来前进到下一页。这将会导致一个问题，也就是如何决定可以在一页上打印多少信息。在 Qt 中，由两种方式处理多页文档：
* 可以把数据转换为 HTML ,并且使用 Qt 的富文本引擎 [QTextDocument](https://doc.qt.io/qt-5/qtextdocument.html) 进行显示
* 可以执行绘制并且手动分页

在书中讲述了这两种方法的使用，这里暂时略过这两个实例。

## Link
* [qt5-book-code/chap08/](https://github.com/mutse/qt5-book-code/tree/master/chap08)
* [itemChanged never called on QGraphicsItem](https://stackoverflow.com/questions/8187807/itemchanged-never-called-on-qgraphicsitem)

---
- [上一级](README.md)
- 上一篇 -> [7. 事件处理](7_event.md)
- 下一篇 -> [9. 拖放](9_drag.md)
