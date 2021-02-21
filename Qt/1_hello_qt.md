# 1.QT入门

<!-- @import "[TOC]" {cmd="toc" depthFrom=1 depthTo=6 orderedList=false} -->
<!-- code_chunk_output -->

* [1.QT入门](#1qt入门)
	* [fedora install Qt5](#fedora-install-qt5)
		* [Qt creator](#qt-creator)
	* [窗口部件的布局](#窗口部件的布局)
		* [Link](#link)

<!-- /code_chunk_output -->

## fedora install Qt5
```sh
dnf install make gcc-c++ gdb
dnf install qt5*-devel
dnf install qt-creator
```

### Qt creator
* [Download link](https://download.qt.io/archive/qt/)

## 窗口部件的布局
```c++
#include <QHBoxLayout>
#include <QApplication>
#include <QSlider>
#include <QSpinBox>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QWidget * window =new QWidget;
    window->setWindowTitle("enter your age");

    QSpinBox * spinBox=new QSpinBox;
    //微调框
    QSlider * slider=new QSlider(Qt::Horizontal);
    //滑块
    spinBox->setRange(0,130);
    slider->setRange(0,130);

    QObject::connect(spinBox,SIGNAL(valueChanged(int)),slider,SLOT(setValue(int)));
    QObject::connect(slider,SIGNAL(valueChanged(int)),spinBox,SLOT(setValue(int)));

    spinBox->setValue(35);

    QHBoxLayout * layout=new QHBoxLayout;
    layout->addWidget(spinBox);
    layout->addWidget(slider);
    window->setLayout(layout);

    window->show();

    return a.exec();
}

```
fedora 27显示效果：
![](../images/1_hello_qt_201711192138_1.png)

Qt有三个主要的布局管理器类：
* QHBoxLayout：在水平方向上排列窗口部件，从左到右（在某些文化中则是从右到左）
* QVBoxLayout:在竖直方向上排列窗口部件，从上到下
* QGridLayout:把各个窗口部件排列在一个网格中

### Link
* [Qt Creator on Fedora 25: “No valid kits found”](https://stackoverflow.com/questions/40978510/qt-creator-on-fedora-25-no-valid-kits-found)

[上一级](README.md)
[下一篇 -> 2.创建对话框](2_creat_dialog.md)
