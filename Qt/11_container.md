# 11. 容器类


<!-- @import "[TOC]" {cmd="toc" depthFrom=1 depthTo=6 orderedList=false} -->
<!-- code_chunk_output -->

- [11. 容器类](#11-容器类)
  - [连续容器](#连续容器)
    - [隐含共享是如何工作的](#隐含共享是如何工作的)

<!-- /code_chunk_output -->


容器通常是用于在内存中存储给定类型的许多项的模板类。STL 中已经提供了很多容器。

Qt 提供了属于自己的容器类，所以在编写 Qt 时，即可以使用 Qt 容器也可以使用 STL 容器。Qt 容器的主要优点是它们在所有的平台上运行时都表现一致，并且它们都是隐含共享的。隐含共享（implicit sharing），或者称为“写时复制”，是一个能够把整个容器作为不需要太多运行成本的值来传递的最优化过程。Qt 容器的另一个主要特征就是易于使用的迭代器类，这是从 Java 中得到的灵感，它们可以利用 QDataStream 变成数据流，而且它们通常可以使可执行文件中的代码量比相应的 STL 类中的要少。最后，在 Qt/Embedded Linux 支持的一些硬件平台上，通常只能使用 Qt 容器。

Qt 既提供了诸如 [QVector<T>](https://doc.qt.io/qt-5/qvector.html)、 [QLinkedList<T>](https://doc.qt.io/qt-5/qlinkedlist.html) 、 [QList<T>](https://doc.qt.io/qt-5/qlist.html) 等的连续容器，也提供了诸如 [QMap<K,T>](https://doc.qt.io/qt-5/qmap.html) 、 [QHash<K,T>](https://doc.qt.io/qt-5/qhash.html) 等的关联容器。

Qt 还提供了在任意容器上执行相关操作的通用算法。

在这一章中，我们还将关注 [QString](https://doc.qt.io/qt-5/qstring.html) 、 [QByteArray](https://doc.qt.io/qt-5/qbytearray.html) 和 [QVariant](https://doc.qt.io/qt-5/qvariant.html) ,因为它们与容器有很多相似之处。QString 是贯穿 Qt 应用编程接口的一个 16 位 Unicode 字符串， QByteArray 是一个用来存储原始二进制数据的 8 位字符数组，QVariant 类则可以存储绝大多数 c++ 和 Qt 值类型。

## 连续容器

QVector<T>:
```c++
QVector<double> vect(3);
vect[0] = 1.0;
vect[1] = 0.540302;
vect[2] = -0.416147;

QVector<double> vect;
vect.append(1.0);
vect.append(0.540302);
vect.append(-0.416147);

//We can also use the << operator instead of append():
vect << 1.0 << 0.540302 << -0.416147;

double sum = 0.0;
for (int i = 0; i < vect.count(); ++i)
sum += vect[i];
```

Qt 还提供了 QLinkedList<T>,这是一种把项存储到内存中不相邻位置的数据结构。与常量不同，链表不支持快速随机访问，但它提供了“常量时间”的插入和删除。

链表并未提供 [] 操作符，所以必须使用迭代器来遍历项。

```c++
QLinkedList<QString> list;
list.append("Clash");
list.append("Ramones");
QLinkedList<QString>::iterator i = list.find("Ramones");
list.insert(i, "Tote Hosen");
```

QList<T> 连续容器是一个“数组列表”，结合了单一类 QVector 和 QLinkedList 的最重要的优点。它支持随机访问，而且它的接口与 QVector 的一样是基于索引的。在 QList 的任意一端插入或者移除项都是非常快速的，并且对含 1000 项以上的列表来说，在中间插入项也是很快的。除非我们想在一个极大的列表中执行插入或者要求列表中的元素都必须占据连续的内存地址，否则 QList 通常是最适合采用的多用途容器类。

[QStringList](https://doc.qt.io/qt-5/qstringlist.html) 类是被广泛用于 Qt 应用编程接口的 QList<QString> 的子类，且提供了一些特别的函数，以使得这种类对于字符串的处理方式更通用。

[QStack<T>](https://doc.qt.io/qt-5/qstack.html) 和 [QQueue<T>](https://doc.qt.io/qt-5/qqueue.html) 是这些方便的子类中的另外两个例子。

Qt 提供的两类迭代器用于遍历存储在容器中的项：
* Jave 风格的迭代器 - 易于使用
* STL 风格的迭代器 - 可以结合 Qt 和 STL 的一般算法而具有更加强大的功能。

对于每个容器类，都有两种 Java 风格的迭代器类型：
* 只读迭代器 - eg :[QVectorIterator<T>](https://doc.qt.io/qt-5/qvectoriterator.html) QLinkedListIterator<T> 和 QListIterator<T>
* 读-写迭代器（在其名字中都含有 “Mutable”字样），eg : [QMutableVectorIterator<T>](https://doc.qt.io/qt-5/qmutablevectoriterator.html)

Java 风格迭代器的有效位置

![](../images/11_container_202004151435_1.png)

当使用 Java 风格的迭代器时，必须首先牢记的是：它们本身并不是直接指向项的，而是能够定位在第一项之前、最后一项之后或者是两项之间。eg：
```c++
QList<double> list;
...
QListIterator<double> i(list);
while (i.hasNext()) {
do_something(i.next());
}
```

向后迭代与此类似，但必须首先调用 toBack():
```c++
QListIterator<double> i(list);
i.toBack();
while (i.hasPrevious()) {
  do_something(i.previous());
}
```

previous() 和 next() 对 Java 风格迭代器的影响：

![](../images/11_container_202004151435_2.png)

Mutable 迭代器（即 java 风格的读写迭代器）在遍历时提供了插入、修改以及删除项的函数。

```c++

// 删除
QMutableListIterator<double> i(list);
while (i.hasNext()) {
  if (i.next() < 0.0)
  i.remove();
}

QMutableListIterator<double> i(list);
i.toBack();
while (i.hasPrevious()) {
  if (i.previous() < 0.0)
  i.remove();
}

// 修改

QMutableListIterator<double> i(list);
while (i.hasNext()) {
  int val = i.next();
  if (val < 0.0)
  i.setValue(-val);
}
```

通过调用 insert() ，可以在当前迭代器位置插入一项。然后，迭代器就被提升到新的项和随后的项之间的位置。

除了 Java 风格的迭代器，每一个连续容器类 C<T> 都有两个 STL 风格的迭代器：
* C<T>::iterator
* C<T>::const_iterator

STL 风格迭代器的有效位置：

![](../images/11_container_202004151435_3.png)

```c++
QList<double>::iterator i = list.begin();
while (i != list.end()) {
  * i = qAbs(*i);
  ++i;
}
```

一些函数放回一个容器。如果想使用 STL 风格的迭代器遍历某个函数的返回值，则必须复制此容器并且遍历这个副本。eg：
```c++
// 正确的方式
QList<int> list = splitter->sizes();
QList<int>::const_iterator i = list.begin();
while (i != list.end()) {
  do_something(*i);
  ++i;
}

// 错误的方式
QList<int>::const_iterator i = splitter->sizes().begin();
while (i != splitter->sizes().end()) {
  do_something(*i);
  ++i;
}

```

这是因为每次调用 splitter->sizes() 都返回一个新的 QList<int> 值。如果不存储这个返回值，则 C++ 在开始迭代之前就自动将其销毁，而只留下一个浮动迭代器。更糟糕的是，每次循环运行的时候，由于调用了 splitter->sizes().end(),splitter->sizes() 都必须生成一个新的列表的副本。总之，当使用 STL 风格的迭代器时，总是在返回的容器副本上进行遍历。

利用只读 Java 风格的迭代器，不必复制容器。这个迭代器在后台自动生成一个副本，以确保总是遍历首先返回的函数的数据。eg：
```c++
QListIterator<int> i(splitter->sizes());
while (i.hasNext()) {
  do_something(i.next());
}
```

像这样复制一个容器看起来似乎耗费比较大，实际上不然，这是由于采用了名为隐含共享的最优化过程。这意味着复制一个 Qt 容器的速度大致就像复制一个简单指针一样快。只有在复制项之一发生改变时，数据才会实际被复制，而且这一操作都可以在后台自动处理。由于这个原因，隐含共享（implicit sharing） 有时候也被称为“写时复制”。

隐含共享的优点在于它是一个我们不必考虑的最优化过程。它工作简单，不需要程序员的任何干预。同时，隐含共享提倡由值返回的对象的整洁的编程风格。eg：

```c++
QVector<double> sineTable()
{
  QVector<double> vect(360);
  for (int i = 0; i < 360; ++i)
  vect[i] = std::sin(i / (2 * M_PI));
  return vect;
}

QVector<double> table = sineTable();
```

比较起来，STL 鼓励我们将 vector 作为一个非常量参数来传递，以避免当函数返回值被存储于变量中时发生复制：
```c++
void sineTable(std::vector<double> &vect)
{
  vect.resize(360);
  for (int i = 0; i < 360; ++i)
  vect[i] = std::sin(i / (2 * M_PI));
}

// 调用就像这样
std::vector<double> table;
sineTable(table);
```

Qt 对所有的容器和许多其他类都使用隐含共享，包括 QByteArray QBrush QFont QImage QPixmap 和 QString。这使得这些类不论是作为函数参数还是函数返回值，都可以非常有效地传递。

隐含共享是 Qt 对不希望修改的数据绝不进行复制的保证。为了使隐含共享的作用发挥得最好，可以采用两个新的编程习惯。
1. 对于一个非常量 vector 或者 list 进行只读存取时，使用 at() 函数而不用 [] 操作符。因为 Qt 的容器类不能辨别 [] 操作符是否将出现在一个赋值的左边还是右边。，它假设最坏的情况出现并且强制执行深层复制，而 at() 函数则不被允许出现在一个赋值的左边。
2. 当使用 STL 风格迭代器遍历容器类的时候，类似的问题也将出现。只要在非常量的容器类上调用 begin() 或 end() 函数，并且如果数据是共享的，Qt 就会强制执行深层赋值。为了防止这种底效操作的发生，一种解决方法就是无论何时都尽可能地使用 const_iterator、 constBegin() 和 constEnd()。

Qt 还提供了最后一种在连续容器中遍历项的方式 - foreach 循环，eg：
```c++
QLinkedList<Movie> list;
...
foreach (Movie movie, list) {
  if (movie.title() == "Citizen Kane") {
    std::cout << "Found Citizen Kane" << std::endl;
    break;
  }
}
```

foreach 循环会在进入循环时自动复制一个容器，因此即使在迭代过程中修改了容器类，也不会影响到循环。break 和 continue 循环声明也是支持的。就如同 for 循环的声明语句一样，可以在循环体外定义迭代变量，eg：
```c++
QLinkedList<Movie> list;
Movie movie;
...
foreach (movie, list) {
  if (movie.title() == "Citizen Kane") {
    std::cout << "Found Citizen Kane" << std::endl;
    break;
  }
}
```

在循环体外定义迭代变量只对那些支持含有一个逗号的数据类型的容器类才适用（eg, QPair<QString,int>）。

### 隐含共享是如何工作的

这里以 QString 为例说明隐含共享的工作原理。

```c++
QString str1 = "Humpty";
QString str2 = str1;
```
我们设置 str1 为 "Humpty"并令 str2 等于 str1。在这一点上，QString 的两个对象都指向内存中相同的内部数据结构。与字符数据一起，数据结构保存一个引用计数，以指出有多少 QString 指向相同的数据结构。因为 str1 和 str2 都指向相同的数据，所以引用计数的值为 2.

```c++
str2[0] = 'D';
```

当修改 str2 时，它首先对数据进行深层复制，以确保 str1 和 st2 指向不同的数据结构，然后才将新数值应用于它所复制的数据。str1 的数据的引用计数变为 1,且把 str2 的数据的引用计数也设为 1.引用计数为 1 表示数据未被共享。

```c++
str2.truncate(4); // str2 = "Dump"
```
如果再次修改 str2 ，则由于 str2 数据的引用计数为 1,将不会发生数据复制。
```c++
str1 = str2;
```
当将 str2 赋给 str1 时， str1 的数据的引用计数将为 0 ，这意味着没有一个 QString 仍在使用 "Humpty" 数据。这样，这些数据就从内存中释放。两个 QString 都指向 "Dump" ,现在它的引用计数就是 2 了。

由于引用计数中的竞争情况，数据共享在多线程程序中通常只是作为一个选项而没有给与关注。使用 Qt，这并不是一个问题。在内部，容器类使用汇编语言指令执行基本的引用计数。通过 [QSharedData](https://doc.qt.io/qt-5/qshareddata.html) 和 [QSharedDataPointer](https://doc.qt.io/qt-5/qshareddatapointer.html) 类，Qt 的用户也可以使用这项技术。




[上一级](README.md)
[上一篇](10_itemViewClass.md)
[下一篇](14_multiThread.md)
