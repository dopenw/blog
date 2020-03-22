# 5. 创建自定义窗口部件


<!-- @import "[TOC]" {cmd="toc" depthFrom=1 depthTo=6 orderedList=false} -->
<!-- code_chunk_output -->

- [5. 创建自定义窗口部件](#5-创建自定义窗口部件)
  - [自定义 Qt 窗口部件](#自定义-qt-窗口部件)
    - [HexSpinBox.h](#hexspinboxh)
    - [HexSpinBox.cpp](#hexspinboxcpp)
  - [子类化 QWidget](#子类化-qwidget)
    - [IconEditor.h](#iconeditorh)
    - [IconEditor.cpp](#iconeditorcpp)
  - [在 Qt 设计师中集成自定义窗口部件](#在-qt-设计师中集成自定义窗口部件)
  - [Link](#link)

<!-- /code_chunk_output -->


这一章讲解如何使用 Qt 开发自定义窗口部件。通过对一个已经存在的 Qt 窗口部件进行子类化或者直接对 QWidget 进行子类化，就可以创建自定义窗口部件。本章将示范这两种方式，并且也会说明如何把自定义窗口部件集成到 Qt 设计师中，这样就可以像使用内置的 Qt 窗口部件一样来使用它们了。

## 自定义 Qt 窗口部件

对 Qt 窗口部件进行更多的自定义定制，一个简单而直接的解决方法就是对相关的窗口部件类进行子类化并且使它能够满足我们的需要。

比如一个十六进制的微调框。

![](../images/5_createCustomWidget_202003221928_1.png)

### HexSpinBox.h
```c++
#ifndef HEXSPINBOX_H
#define HEXSPINBOX_H

#include <QSpinBox>

QT_BEGIN_NAMESPACE
class QRegExpValidator;
QT_END_NAMESPACE

class HexSpinBox : public QSpinBox
{
    Q_OBJECT

public:
    HexSpinBox(QWidget * parent = 0);

protected:
  QValidator::State validate(QString &text, int &pos) const override;
  int valueFromText(const QString &text) const override;
  QString textFromValue(int value) const override;

private:
    QRegExpValidator * validator;
};

#endif
```

### HexSpinBox.cpp
```c++
#include <QtWidgets>

#include "HexSpinBox.h"

HexSpinBox::HexSpinBox(QWidget *parent)
    : QSpinBox(parent)
{
    // 设置默认范围是从 0 - 255 (即从 0x00 - 0x FF)
    setRange(0, 255);
    // 严格控制用户输入的数据必须是合法的十六进制数字
    validator = new QRegExpValidator(QRegExp("[0-9A-Fa-f]{1,8}"), this);
}

// 这个函数由 QSpinBox 调用，用来检查目前为止用户输入文本的合法性。
// 有三种结果可能会出现：Invalid(无效，输入的文本与常规表达式不匹配)
// Intermediate(部分有效部分无效，输入的文本是一个有效值中似是而非的一部分)
// Acceptable （可以接受，输入的文本合法有效）
QValidator::State HexSpinBox::validate(QString &text, int &pos) const
{
    return validator->validate(text, pos);
}

// 当用户在微调框的编辑器部分输入一个值并且按下 Enter 时，QSpinBox 就会调用它
int HexSpinBox::valueFromText(const QString &text) const
{
    bool ok;
    return text.toInt(&ok, 16);
}

// 当用户按下微调框的向上或者向下箭头时，QSpinBox 会调用它来更新微调框的编辑器部分。
QString HexSpinBox::textFromValue(int value) const
{
    return QString::number(value, 16).toUpper();
}
```

## 子类化 QWidget

如果窗口部件本身没有任何信号和槽，并且它也没有重新实现任何虚函数，那么我们甚至还有可能通过对现有窗口部件的组合而不是通过子类化的方式来生成这样的窗口部件。比如第一章创建的 Age应用程序。

当手里没有任何一个 Qt 窗口部件能够满足任务要求，并且也没有办法通过组合现有窗口部件来满足所需的期望结果时，仍旧可以创建出我们想要的窗口部件来。要实现这一点，只需要通过子类化 QWidget ，并且通过重新实现一些用来绘制窗口部件和响应鼠标点击事件的事件处理器即可。这一方法给了我们定义并且控制自己的窗口部件的外观和行为的完全自由。

eg: 编写一个自定义窗口部件的 IconEditor 窗口部件。

![](../images/5_createCustomWidget_202003221928_2.png)

### IconEditor.h

* [Q_PROPERTY](https://doc.qt.io/qt-5/properties.html) The Property System

```c++
#ifndef ICONEDITOR_H
#define ICONEDITOR_H

#include <QColor>
#include <QImage>
#include <QWidget>

class IconEditor : public QWidget
{
    Q_OBJECT

    // 使用 Q_PROPERTY 声明了三个自定义属性：penColor、iconImalge 和 zoomFactor.
    // 每个属性都有一个数据类型、一个 “读” 函数和一个作为可选项的 “写” 函数。
    // 例如：penColor 属性类型是 QColor,并且可以使用 penColor() 和 setPenColor() 函数对他进行读写。

    // 当我们在 Qt 设计师中使用这个窗口部件时，在 Qt 设计师属性编辑器里
    //，那些继承于 QWidget 的属性下面，将会显示这些自定义属性。
    // 这些属性可以是由 QVariant 所支持的任何类型。对于定义属性的类，Q_OBJECT 宏是必需的。

    Q_PROPERTY(QColor penColor READ penColor WRITE setPenColor)
    Q_PROPERTY(QImage iconImage READ iconImage WRITE setIconImage)
    Q_PROPERTY(int zoomFactor READ zoomFactor WRITE setZoomFactor)

public:
    IconEditor(QWidget * parent = 0);

    void setPenColor(const QColor &newColor);
    QColor penColor() const { return curColor; }
    void setZoomFactor(int newZoom);
    int zoomFactor() const { return zoom; }
    void setIconImage(const QImage &newImage);
    QImage iconImage() const { return image; }
    QSize sizeHint() const override;

protected:
  void mousePressEvent(QMouseEvent * event) override;
  void mouseMoveEvent(QMouseEvent * event) override;
  void paintEvent(QPaintEvent * event) override;

private:
    void setImagePixel(const QPoint &pos, bool opaque);
    QRect pixelRect(int i, int j) const;

    QColor curColor;
    QImage image;
    int zoom;
};

#endif

```

### IconEditor.cpp

* [QImage](https://doc.qt.io/qt-5/qimage.html) 使用一种与硬件无关的方式来存储图像。可以把它设置成使用 1 位、8位或者32位色深。一个具有32位色深的图像分别对每一个像素各使用 8 位来存储它的红、绿、蓝分量。剩余的8位存储这个像素的 alpha 分量（即不透明度）。
* [QColor](https://doc.qt.io/qt-5/qcolor.html) The QColor class provides colors based on RGB, HSV or CMYK values
* [QPainter](https://doc.qt.io/qt-5/qpainter.html) The QPainter class performs low-level painting on widgets and other paint devices


当产生一个绘制事件并且调用 paintEvent() 函数的时候，就会出现如下几种情况：
* 在窗口部件第一次显示时，系统会自动产生一个绘制事件，从而强制绘制这个窗口部件本身。
* 当重新调整窗口部件大小的时候，系统也会产生一个绘制事件。
* 当窗口部件被其他窗口部件遮挡，然后又再次显示出来的时候，就会对那些隐藏的区域产生一个绘制事件（除非这个窗口系统存储了整个区域）

也可以通过调用 QWidget::update() 或者 QWidget::repaint() 强制产生一个绘制事件。这两个函数之间的区别：
* repaint() 函数会强制产生一个即时的重绘事件
* update() 函数则是在 Qt 下一次处理事件时才简单地调用一个绘制事件（如果窗口部件在屏幕上是不可见地，那么这两个函数会什么都不做）。如果多次调用 update() , Qt 就会把连续多次的绘制事件压缩成一个单一的绘制事件，这样就可以避免闪烁现象。


对于 QPainter::drawLine() 的调用遵循这样的语法：
```c++
painter.drawLine(x1,y1,x2,y2);
```
Qt 窗口部件的左上角处的位置坐标是 (0,0) ,右下角的位置坐标是 (width()-1,height()-1)。
使用 QPainter 绘制一条线段如下。

![](../images/5_createCustomWidget_202003221928_3.png)

每一个窗口部件都会配备一个调色板，由它来确定做什么事应该使用什么颜色。例如，对于窗口部件的背景色会有一个对应的调色板条目（通常是亮灰色），并且对于文本的背景色也会对应一个调色板条目（通常是黑色）。默认情况下，一个窗口部件的调色板会采用窗口系统的颜色主题。通过使用调色板中的这些颜色，可以确保 IconEditor 能够尊重用户的选择。

一个窗口部件的调色板由三个颜色组构成：激活组(Active)、非激活组(Inactive)和不可用组(Disabled)。应该使用哪一个颜色组取决于该窗口部件的当前状态：
* Active : 可用于当前激活窗口中的那些窗口部件。
* Inactive: 可用于其他窗口中的那些窗口部件。
* Disabled： 可用于任意窗口中的那些不可用窗口部件。

QWidget::palette() 函数可以返回窗口部件的调色板，它是一个 [QPalette](https://doc.qt.io/qt-5/qpalette.html) 型对象。

在构造函数中setAttribute(Qt::WA_StaticContents)，这个属性告诉 Qt,当重新改变窗口部件的大小时，这个窗口部件的内容并没有发生变化，而且它的内容仍旧保留从窗口部件左上角开始的特性。当重新定义窗口部件的大小时，通过使用这个信息，Qt 就可以避免对已经显示区域的重新绘制。

通常情况下，当重新定义一个窗口部件的大小时，Qt 会为窗口部件的整个可见区域生成一个绘制事件。但是如果该窗口部件在创建时使用了 Qt::WA_StaticContents 属性，那么绘制事件的区域就会被严格限定在之前没有被显示的像素部分上。这也就意味着，如果重新把窗口部件改变为比原来还要小的尺寸，那么就根本不会产生任何绘制事件。

```c++
#include <QtWidgets>

#include "IconEditor.h"

IconEditor::IconEditor(QWidget *parent)
    : QWidget(parent)
{
    //In the constructor, we set the Qt::WA_StaticContents attribute for the widget
    //, indicating that the widget contents are rooted to the top-left corner
    // and don't change when the widget is resized.
    // Qt uses this attribute to optimize paint events on resizes.
    // This is purely an optimization and should only be used for widgets whose
    // contents are static and rooted to the top-left corner.
    setAttribute(Qt::WA_StaticContents);

    // 调用 以 QSizePolicy::Minimum 为水平和垂直大小策略的 setSizePolicy(),
    // 会告诉负责管理这个窗口部件的任意布局管理器，这个窗口部件的大小提示就是它的最小尺寸大小。

    setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);

    // 画笔的颜色被设置为黑色。
    curColor = Qt::black;

    // 缩放因子(zoom factor) 被设置为 8 ，也就是说，图表中的每一个像素都将会显示成一个 8 x 8 的正方形。
    zoom = 8;

    // 初始化为 16 x 16 的像素大小和 32 位的 ARGB 颜色格式，这种颜色格式可以支持半透明的效果。
    image = QImage(16, 16, QImage::Format_ARGB32);
    image.fill(qRgba(0, 0, 0, 0));
}

// Qt 提供了两种存储颜色的类型： QRgb 和 QColor 。
// 虽然 qRgb 仅仅是一个用在 QImage 中 32 位像素数据的类型别名，
// 但 QColor 则是一个具有许多有用函数并且在 Qt 中广泛用于存储颜色的类。
// 在 QIconEditor 窗口部件中，只有在处理 QImage 时，我们才使用 qRgb,
// 而对于其他任意东西，包括这里的 penColor 属性，我们都只使用 QColor.

void IconEditor::setPenColor(const QColor &newColor)
{
    curColor = newColor;
}

// 设置图像的缩放因子
void IconEditor::setZoomFactor(int newZoom)
{
    if (newZoom < 1)
        newZoom = 1;

    if (newZoom != zoom) {
        zoom = newZoom;
        // 重绘，通知布局变化
        update();
        updateGeometry();
    }
}

// 设置需要编辑的图像
void IconEditor::setIconImage(const QImage &newImage)
{
    if (newImage != image) {
        // 调用 convertToFormat（） 把它变成一个带 alpha 缓冲的 32 位图像。
        image = newImage.convertToFormat(QImage::Format_ARGB32);
        // 调用 QWidget::update() ，它会使用新的图像强制重绘这个窗口部件。
        update();
        // 告诉包含这个窗口部件的任意布局，这个窗口部件的大小提示已经发生改变了。
        // 于是，该布局将会自动适应这个新的大小提示。
        updateGeometry();
    }
}

// 返回窗口部件的理想大小
// 在和布局联合使用时，窗口部件的大小提示非常有用。
// 当 Qt 的布局管理器摆放一个窗体的子窗口部件时，它会尽可能多地考虑这些窗口部件的大小提示。
QSize IconEditor::sizeHint() const
{
    QSize size = zoom * image.size();
    if (zoom >= 3)
        // 额外增加一个像素，以便可以容纳一个网格线
        size += QSize(1, 1);
    return size;
}

// “鼠标按下”事件
void IconEditor::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        setImagePixel(event->pos(), true);
    } else if (event->button() == Qt::RightButton) {
        setImagePixel(event->pos(), false);
    }
}

// “鼠标移动”事件。默认情况下，只有当用户按住一个键不放的时候，才会产生这些事件。
// 通过调用 QWidget::setMouseTracking() 则有可能改变这一行为，但是在这个例子中不需要这样做。
void IconEditor::mouseMoveEvent(QMouseEvent *event)
{
    // 由于有可能会同时按下多个按键，所以最终结果实际是 QMouseEvent::buttons() 的返回值
    // 与鼠标的按键按照按位“或”（OR）运算之后的结果。可以使用"&" 来测试某个特定键是否按下了。
    if (event->buttons() & Qt::LeftButton) {
        setImagePixel(event->pos(), true);
    } else if (event->buttons() & Qt::RightButton) {
        setImagePixel(event->pos(), false);
    }
}

// 只要需要重新绘制窗口部件，就会调用它
void IconEditor::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);

    if (zoom >= 3) {
        // 绘制构成网格的水平线段和垂直线段。
        // 使用窗口部件的调色板(palette)颜色来指定线段的颜色
        painter.setPen(palette().foreground().color());
        for (int i = 0; i <= image.width(); ++i)
            painter.drawLine(zoom * i, 0,
                             zoom * i, zoom * image.height());
        for (int j = 0; j <= image.height(); ++j)
            painter.drawLine(0, zoom * j,
                             zoom * image.width(), zoom * j);
    }

    for (int i = 0; i < image.width(); ++i) {
        for (int j = 0; j < image.height(); ++j) {
            // 调用返回一个 QRect,其中定义了需要重新绘制的区域
            // 作为简单的优化处理方法，我们没有对落在这个区域之外的像素进行重新绘制。
            QRect rect = pixelRect(i, j);
            if (!event->region().intersected(rect).isEmpty()) { // intersect(rect) not used in Qt5
                QColor color = QColor::fromRgba(image.pixel(i, j));

                // 如果该颜色并非完全不透明（它的 alalpha 通道小于 255），就会先绘制出一个白色的背景来
                if (color.alpha() < 255)   
                    // 绘制一个缩放后的像素。   
                    painter.fillRect(rect, Qt::white);
                // 绘制一个缩放后的像素。
                painter.fillRect(rect, color);
            }
        }
    }
}

void IconEditor::setImagePixel(const QPoint &pos, bool opaque)
{
    // 把鼠标的位置从窗口部件的坐标转换到图像的坐标。
    int i = pos.x() / zoom;
    int j = pos.y() / zoom;

    // 检查该点是否位于正确的范围之内。
    if (image.rect().contains(i, j)) {
        if (opaque) {
            image.setPixel(i, j, penColor().rgba());
        } else {
            image.setPixel(i, j, qRgba(0, 0, 0, 0));
        }

        // 对需要重新绘制的区域调用带 QRect 的 update()
        update(pixelRect(i, j));
    }
}

QRect IconEditor::pixelRect(int i, int j) const
{
  // QRect构造函数具有 QRect(x,y,width,height) 的语法形式
  // ，这里的 (x,y) 是这个矩形左上角的位置坐标，而 width x height 就是矩形的尺寸大小。
  // 如果缩放因子是 3 或者更大，则可以在矩形的水平和竖直方向大小上都减去一个像素
  //，以便在填充时不会覆盖那些网格线。
    if (zoom >= 3) {
        return QRect(zoom * i + 1, zoom * j + 1, zoom - 1, zoom - 1);
    } else {
        return QRect(zoom * i, zoom * j, zoom, zoom);
    }
}

```

## 在 Qt 设计师中集成自定义窗口部件
在 Qt 设计师中使用自定义窗口部件之前，我们必须让 Qt 设计师先察觉到它们的存在。有两种方法可以完成这一任务：
* 改进法([promotion](https://doc.qt.io/qt-5/designer-using-custom-widgets.html))
* 插件法([plugin](https://doc.qt.io/qt-5/designer-creating-custom-widgets.html))

改进法是最为快捷和简单的方法。这种方法包括：选择一个内置的 Qt 窗口部件，但该窗口部件要和我们自定义的窗口部件具有相类似的应用程序编程接口，并在 Qt 设计师的自定义窗口部件对话框中填写一些与这个窗口部件相关的信息。然后，这个自定义窗口部件就可用于由 Qt 设计师开发的窗体中，尽管在编辑或者预览该窗体时它有可能仍旧显示为与之相关的内置 Qt 窗口部件形式。

![](../images/5_createCustomWidget_202003221928_4.png)

eg：在窗体中插入一个 HexSpinBox 窗口部件的步骤：
1. 从 Qt 设计师的窗口部件工具盒中拖动一个 QSpinBox 到窗体中，由此创建一个 QSpinBox 。
2. 右键单击这个微调框，并且从上下文菜单中选择 "Promte to Custom Widget"(改进成自定义窗口部件)。
3. 在弹出的对话框中，填入 "HexSpinBox"作为类的名字，填入 "HexSpinBox.h" 作为头文件的名字。

这三步就足够了。由 uic 生成的代码将会包含 HexSpinBox.h ,而不是 <QSpinBox> ,并且会生成一个 HexSpinBox 的实例。在 Qt 设计师中，将会用 QSpinBox 的图标来代表 HexSpinBox 窗口部件，从而允许我们设置 QSpinBox 的所有属性（例如，它的作用范围和当前值）。


改进法的缺点是：在 Qt 设计师中，无法对自定义窗口部件中的那些特定属性进行访问，并且也无法对这个窗口部件自身进行绘制。所有这两个问题都可以通过插件法得到解决。

插件法需要创建一个插件库，Qt 设计师可以在运行时加载这个库，并且可以利用该库创建窗口部件的实例。在对窗体进行编辑或者用于窗体预览时，Qt 设计师就会用到这个真正的窗口部件，这要归功于 Qt 的元对象系统，Qt 设计师才可以动态获取它的这些属性的列表。

eg： 将 IconEditor 集成为一个插件。

首先，必须对 [QDesignerCustomWidgetInterface](https://doc.qt.io/qt-5/qdesignercustomwidgetinterface.html) 进行子类化，并且需要重新实现一些虚函数。假定插件的源代码放在一个名为 iconEditotPlugin 的目录中，并且 IconEditor 的源代码放在与 iconEditotPlugin 目录同级的 iconEditor 目录中。

IconEditorPlugin.h:
```c++
#ifndef ICONEDITORPLUGIN_H
#define ICONEDITORPLUGIN_H

#include <QDesignerCustomWidgetInterface>

class IconEditorPlugin : public QObject,
                         public QDesignerCustomWidgetInterface
{
    Q_OBJECT
	Q_PLUGIN_METADATA(IID "org.qt-project.Qt.QDesignerCustomWidgetInterface" FILE "iconEditor.json")
    // 通知 moc:这里的第二个基类是一个插件接口
    Q_INTERFACES(QDesignerCustomWidgetInterface)

public:
    IconEditorPlugin(QObject * parent = 0);

    QString name() const;
    QString includeFile() const;
    QString group() const;
    QIcon icon() const;
    QString toolTip() const;
    QString whatsThis() const;
    bool isContainer() const;
    QWidget * createWidget(QWidget * parent);
};

#endif
```

IconEditorPlugin.cpp:
```c++
#include <QtPlugin>

#include "../iconEditor/IconEditor.h"
#include "IconEditorPlugin.h"

IconEditorPlugin::IconEditorPlugin(QObject *parent)
    : QObject(parent)
{
}

// 返回由该插件提供的这个窗口部件的名字
QString IconEditorPlugin::name() const
{
    return "IconEditor";
}

// 返回由该插件封装的特定窗口部件的头文件名称。这个头文件会包含在由 uic 工具所生成的代码中。
QString IconEditorPlugin::includeFile() const
{
    return "IconEditor.h";
}

// 返回自定义窗口部件所应属于的窗口部件工具箱群组的名字。但如果以这个名字命名的窗口部件工具箱群组还没有使用到，那么 Qt 设计师将会为这个窗口部件创建一个新的群组。
QString IconEditorPlugin::group() const
{
    return tr("Image Manipulation Widgets");
}

// 返回一个图标，可以在 Qt 设计师窗口部件工具箱中用它来代表自定义窗口部件。这里，我们假定 IconEditorPlugin 有一个与之关联的 Qt resource file ，其中有适当的一项可作为图标。
QIcon IconEditorPlugin::icon() const
{
    return QIcon(":/images/iconEditor.png");
}

// 返回一个工具提示信息，可在鼠标悬停在 Qt 设计师窗口部件工具箱中的自定义窗口部件上时显示该信息。
QString IconEditorPlugin::toolTip() const
{
    return tr("An icon editor widget");
}

// 返回用于Qt设计师 "what is This?" 中显示的文本
QString IconEditorPlugin::whatsThis() const
{
    return tr("This widget is presented in Chapter 5 of <i>C++ GUI "
              "Programming with Qt 4</i> as an example of a custom Qt "
              "widget.");
}

// 如果窗口部件还可以包含其他窗口部件，就返回 true,否则，返回 false
bool IconEditorPlugin::isContainer() const
{
    return false;
}

// 利用给定的父对象创建该窗口部件类的一个实例
QWidget *IconEditorPlugin::createWidget(QWidget *parent)
{
    return new IconEditor(parent);
}

//Q_EXPORT_PLUGIN2(iconeditorplugin, IconEditorPlugin)

```

用于构建该插件的 .pro 文件看起来如下所示：
```highLight
TEMPLATE = lib
CONFIG += plugin
TARGET = IconEditorPlugin
DEPENDPATH += .
INCLUDEPATH += .

# Input
HEADERS += IconEditorPlugin.h ../iconEditor/IconEditor.h
SOURCES += IconEditorPlugin.cpp ../iconEditor/IconEditor.cpp
RESOURCES += IconEditorPlugin.qrc

QT += widgets designer

OTHER_FIFLES += iconEditor.json
```

当键入 make 或者 nmake 来构建该插件时，它就会自动把自己安装到 Qt 设计师的 plugins/designer 目录中。插件一旦构建完毕，在 Qt 设计师中就可以像其他内置的 Qt 窗口部件一样来使用 IconEditor 窗口部件。

## Link
* [qt5-book-code/chap05/](https://github.com/mutse/qt5-book-code/tree/master/chap05)
[上一级](README.md)
[上一篇](4_SpreadSheet.md)
[下一篇](14_multiThread.md)
