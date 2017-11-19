# 1.QT入门

## 窗口部件的布局
```c++
#include <QHBoxLayout>
#include <QApplication>
#include <QSlider>
#include <QSpinBox>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QWidget *window =new QWidget;
    window->setWindowTitle("enter your age");

    QSpinBox *spinBox=new QSpinBox;
    //微调框
    QSlider *slider=new QSlider(Qt::Horizontal);
    //滑块
    spinBox->setRange(0,130);
    slider->setRange(0,130);

    QObject::connect(spinBox,SIGNAL(valueChanged(int)),slider,SLOT(setValue(int)));
    QObject::connect(slider,SIGNAL(valueChanged(int)),spinBox,SLOT(setValue(int)));

    spinBox->setValue(35);

    QHBoxLayout *layout=new QHBoxLayout;
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
[上一级](base.md)
[下一篇](2_creat_dialog.md)
