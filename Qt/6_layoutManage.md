# 6. 布局管理


<!-- @import "[TOC]" {cmd="toc" depthFrom=1 depthTo=6 orderedList=false} -->
<!-- code_chunk_output -->

- [6. 布局管理](#6-布局管理)
  - [在窗体中摆放窗口部件](#在窗体中摆放窗口部件)

<!-- /code_chunk_output -->


放置在窗体的每一个部件都必须给定一个合适的大小和位置。Qt 提供了多个用于在窗体中摆放窗口部件的类： [QHBoxLayout](https://doc.qt.io/qt-5/qhboxlayout.html)、 [QVBoxLayout](https://doc.qt.io/qt-5/qvboxlayout.html)、 [QGridLayout](https://doc.qt.io/qt-5/qgridlayout.html)和 [QStackedLayout](https://doc.qt.io/qt-5/qstackedlayout.html)。

使用 Qt 布局类的另外一个原因是：它们可以确保窗体能够自动适应于不同的字体、语言和系统平台。

可以执行布局管理功能的类还有 [QSplitter](https://doc.qt.io/qt-5/qsplitter.html) 、 [QScrollArea](https://doc.qt.io/qt-5/qscrollarea.html) 、 [QMainWindow](https://doc.qt.io/qt-5/qmainwindow.html) 、 [QMdiArea](https://doc.qt.io/qt-5/qmdiarea.html)。 这些类所拥有的共同点在于它们提供了一种用户可以灵活掌控的布局方式。

## 在窗体中摆放窗口部件

管理窗体上子窗口部件的布局：
1. 绝对位置法(最原始的)：这可以通过对窗体的各个子窗口部分分配固定的大小和位置以及对窗体分配固定的大小实现。
```c++
FindFileDialog::FindFileDialog(QWidget *parent)
    : QDialog(parent)
{
    ...
    namedLabel->setGeometry(9, 9, 50, 25);
    namedLineEdit->setGeometry(65, 9, 200, 25);
    lookInLabel->setGeometry(9, 40, 50, 25);
    lookInLineEdit->setGeometry(65, 40, 200, 25);
    subfoldersCheckBox->setGeometry(9, 71, 256, 23);
    tableWidget->setGeometry(9, 100, 256, 100);
    messageLabel->setGeometry(9, 206, 256, 25);
    findButton->setGeometry(271, 9, 85, 32);
    stopButton->setGeometry(271, 47, 85, 32);
    closeButton->setGeometry(271, 84, 85, 32);
    helpButton->setGeometry(271, 199, 85, 32);

    setWindowTitle(tr("Find Files or Folders"));
    setFixedSize(365, 240);
}

```
2. 人工布局法（通过重新实现窗体的 resizeEvent() 函数）：将子窗口的大小尺寸设置为与窗口的大小成比例
```c++
void FindFileDialog::resizeEvent(QResizeEvent * /* event */)
{
    int extraWidth = width() - minimumWidth();
    int extraHeight = height() - minimumHeight();

    namedLabel->setGeometry(9, 9, 50, 25);
    namedLineEdit->setGeometry(65, 9, 100 + extraWidth, 25);
    lookInLabel->setGeometry(9, 40, 50, 25);
    lookInLineEdit->setGeometry(65, 40, 100 + extraWidth, 25);
    subfoldersCheckBox->setGeometry(9, 71, 156 + extraWidth, 23);
    tableWidget->setGeometry(9, 100, 156 + extraWidth,
                             50 + extraHeight);
    messageLabel->setGeometry(9, 156 + extraHeight, 156 + extraWidth,
                              25);
    findButton->setGeometry(171 + extraWidth, 9, 85, 32);
    stopButton->setGeometry(171 + extraWidth, 47, 85, 32);
    closeButton->setGeometry(171 + extraWidth, 84, 85, 32);
    helpButton->setGeometry(171 + extraWidth, 149 + extraHeight, 85,
                            32);
}
```
3. 布局管理器法（最简便易行）

![](../images/6_layoutManage_202003262212_2.png)

```c++
FindFileDialog::FindFileDialog(QWidget *parent)
    : QDialog(parent)
{
    ...


    // QGridLayout::addWidget() 调用遵循如下语法：
    // layout->addWidget(widget,row,column,rowSpan,columnSpan);
    // widget 要插入到布局中的子窗口部件
    // (row,column) 是由该窗口部件部件所占用的左上角单元格
    // rowSpan 是该窗口部件要占用的行数
    // columnSpan 是该窗口部件要占用的列数
    // 如果省略了这些参数，则参数 rowSpan 和 columnSpan 将会取默认值 1。
    QGridLayout * leftLayout = new QGridLayout;
    leftLayout->addWidget(namedLabel, 0, 0);
    leftLayout->addWidget(namedLineEdit, 0, 1);
    leftLayout->addWidget(lookInLabel, 1, 0);
    leftLayout->addWidget(lookInLineEdit, 1, 1);
    leftLayout->addWidget(subfoldersCheckBox, 2, 0, 1, 2);
    leftLayout->addWidget(tableWidget, 3, 0, 1, 2);
    leftLayout->addWidget(messageLabel, 4, 0, 1, 2);

    QVBoxLayout * rightLayout = new QVBoxLayout;
    rightLayout->addWidget(findButton);
    rightLayout->addWidget(stopButton);
    rightLayout->addWidget(closeButton);
    // 告诉垂直布局管理器，它会占满布局中这一处空间
    // 在 Qt 设计师中，可以通过插入一个分隔符(spacer)来达到同样的效果。
    rightLayout->addStretch();
    rightLayout->addWidget(helpButton);

    QHBoxLayout * mainLayout = new QHBoxLayout;
    mainLayout->addLayout(leftLayout);
    mainLayout->addLayout(rightLayout);
    setLayout(mainLayout);

    setWindowTitle(tr("Find Files or Folders"));
}
```

通过一个 QHBoxLayout 一个 QGridLayout 和一个 QVBoxLayout ，该布局就得到处理。

![](../images/6_layoutManage_202003262212_1.png)

对话框周围的边白和两个子窗口之间的间隔均被设置为默认值，该值取决于当前窗口部件的风格，但通过 [QLayout](https://doc.qt.io/qt-5/qlayout.html) 提供的 setContentsMargins() 和 setSpacing() ，可以对他们的值进行修改。

对于目前所探讨的问题，使用布局管理器的确为我们提供了很多额外的好处。如果往布局中添加一个窗口部件或者从布局中调用了hide() 或者 show() ，也同样能够做到自适应。如果一个子窗口部件的大小提示发生了变化，布局将会自动进行调整，从而把新的大小提示考虑进去。还有，布局管理器也会自动根据窗体中子窗口部件的最小大小提示和大小提示，从总体上为这个窗体设置一个最小尺寸。

在迄今为止所给出的例子中，我们只是简单地把窗口部件放置到布局中，并且使用一定地分隔符元素（拉伸因子）来占用任何多余的空间。但在某些情况下，由此形成的布局看起来可能还不是我们最想要的形式。在这些情形中，可以通过改变要摆放的窗口部件的大小尺寸策略和大小提示来调整布局。

一个窗口部件的大小策略会告诉布局系统应该如何对它进行拉伸或者压缩。Qt 为它所有的内置窗口部件都提供了合理的默认大小策略值，但是由于不可能为每一种可能产生的布局都提供唯一的默认值，所以在一个窗体中，开发人员改变它上面的一个或两个窗口部件的大小策略是非常普遍的现象。一个 [QSizePolicy](https://doc.qt.io/qt-5/qsizepolicy.html) 既包含一个水平分量又包含一个垂直分量。以下是一些最为常用的取值：
* Fixed - 该窗口部件不能被拉伸或者压缩
* Minimum - 该窗口部件的大小提示就是它的最小大小
* Maximum - 该窗口部件的大小提示就是它的最大大小
* Preferred - 该窗口部件的大小提示就是它比较合适的大小。但是如果需要，还是可以对该窗口部件进行拉伸或者压缩。
* Expanding - 可以拉伸或者压缩该窗口部件，并且它特别希望能够变高变长。

使用一个 显示“Some Text”文本的 QLabel 为例：

![](../images/6_layoutManage_202003262212_3.png)

在图中，Preferred 和 Expanding 描述成了同样的效果。但是，它们之间到底有何不同呢？在重新改变一个既包含 Preferred 又包含 Expanding 窗口部件的尺寸大小时，多出来的空间就会分配给 Expanding 窗口部件，而 Preferred 窗口部件仍旧会按照原有大小提示而保持不变。

这里还有另外两种大小规则： MinimumExpanding 和 Ignored. 前者仅仅用于 Qt 老版本的极少数情况中，但是如今它已经不再会被用到了。一种比较好的方式是使用 Expanding ，并且再适当地对 minimumSizeHint() 进行重新实现即可。后者与 Expanding 相似，只是它可以忽略窗口部件的大小提示和最小大小提示。

除了大小规则中包含的水平方向和垂直方向两个分量之外，QSizePolicy 类还保存了水平方向和垂直方向的一个拉伸因子。这些拉伸因子可以用来说明在增大窗体的时候，对不同的子窗体部件应使用的不同放大比例。例如，假定在 QTextEdit 的上面还有一个 QTreeWidget ，并且希望这个 QTextEdit 的高度能够是 QTreeWidget 高度的两倍，那么就可以把这个 QTextEdit 在垂直方向上的拉伸因子设置为 2 ，而把 QTreeWidget 在垂直方向上的拉伸因子设置为 1。

影响布局方式的另一种方法是设置它的子窗口部件的最小大小、最大大小或固定大小。当布局管理器在摆放这些窗口部件的时候，它就会考虑这些约束条件。并且如果这样还不够的话，还可以对子窗口部件的类进行派生并且重新实现 sizeHint() 函数，由此获取所需的大小提示。


[上一级](README.md)
[上一篇](5_createCustomWidget.md)
[下一篇](14_multiThread.md)
