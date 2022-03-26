# 《C++ Qt 设计模式（第二版）》 读书笔记

<!-- @import "[TOC]" {cmd="toc" depthFrom=1 depthTo=6 orderedList=false} -->

<!-- code_chunk_output -->

- [《C++ Qt 设计模式（第二版）》 读书笔记](#c-qt-设计模式第二版-读书笔记)
  - [第 8 章 QObject，QApplication ，信号和槽](#第-8-章-qobjectqapplication-信号和槽)
    - [QObject 的生命周期](#qobject-的生命周期)
    - [Qt Test 框架](#qt-test-框架)
  - [第 10 章 主窗口和动作](#第-10-章-主窗口和动作)
    - [命令模式](#命令模式)
  - [第 12 章 元对象，属性和反射编程](#第-12-章-元对象属性和反射编程)
    - [QMetaObject - 元对象模式](#qmetaobject---元对象模式)
    - [类型识别和 qobject_cast](#类型识别和-qobject_cast)
    - [属性系统](#属性系统)
      - [Q_PROPERTY 宏 - 描述 QObject 属性](#q_property-宏---描述-qobject-属性)
      - [QVariant 类：属性访问](#qvariant-类属性访问)
      - [动态属性](#动态属性)
    - [元类型，声明和注册](#元类型声明和注册)
      - [qRegisterMetaType()](#qregistermetatype)
    - [invokeMethod()](#invokemethod)
  - [第 14 章 验证和正则表达式](#第-14-章-验证和正则表达式)
    - [输入掩码](#输入掩码)
    - [验证器](#验证器)
    - [正则表达式](#正则表达式)
    - [正则表达式验证](#正则表达式验证)
    - [子类化 QValidator](#子类化-qvalidator)
  - [第 17 章 并发](#第-17-章-并发)
    - [QThread 和 QtConcurrent](#qthread-和-qtconcurrent)
      - [线程安全和 QObject](#线程安全和-qobject)
  - [附录](#附录)
    - [UML 建模工具](#uml-建模工具)

<!-- /code_chunk_output -->

---

## 第 8 章 QObject，QApplication ，信号和槽

### QObject 的生命周期

- 警告
  - 确保每一个 [QObject](https://doc.qt.io/qt-5/qobject.html) 在 QApplication 之后创建，在 QApplication 销毁前销毁，这一点至关重要。静态存储区创建的对象将在 main() 返回后才被销毁，这就太迟了。意味着绝不应该定义静态存储类的 QObject。
- 栈还是堆
  - 一般情况下，没有父对象的 QObject 应当在栈上面创建，或者定义成另一个类的子对象。有父对象的 QObject 不应当在栈上面创建。因为那样的话，它有可能被删除两次。在堆上创建的所有 QObject 都应当或者是有父对象的，或者是由其他对象进行管理的。
- 提示
  - 不推荐直接删除 QObject。在带有事件循环的程序中，最好是利用[QObject::deleteLater()](https://doc.qt.io/qt-5/qobject.html#deleteLater) 来删除 QObject。这样做，可以在应用程序处理事件并在当前槽返回之后就安排该对象的销毁。

### Qt Test 框架

[Qt Test Overview](https://doc.qt.io/qt-5/qtest-overview.html)

Qt Test is a framework for unit testing Qt based applications and libraries. Qt Test provides all the functionality commonly found in unit testing frameworks as well as extensions for testing graphical user interfaces.

Qt Test is designed to ease the writing of unit tests for Qt based applications and libraries:

|       Feature       |                                            Details                                           |
|:-------------------:|:--------------------------------------------------------------------------------------------:|
| Lightweight         | Qt Test consists of about 6000 lines of code and 60 exported symbols.                        |
| Self-contained      | Qt Test requires only a few symbols from the Qt Core module for non-gui testing.             |
| Rapid testing       | Qt Test needs no special test-runners; no special registration for tests.                    |
| Data-driven testing | A test can be executed multiple times with different test data.                              |
| Basic GUI testing   | Qt Test offers functionality for mouse and keyboard simulation.                              |
| Benchmarking        | Qt Test supports benchmarking and provides several measurement back-ends.                    |
| IDE friendly        | Qt Test outputs messages that can be interpreted by Qt Creator, Visual Studio, and KDevelop. |
| Thread-safety       | The error reporting is thread safe and atomic.                                               |
| Type-safety         | Extensive use of templates prevent errors introduced by implicit type casting.               |
| Easily extendable   | Custom types can easily be added to the test data and test output.                           |

[Qt Test](https://doc.qt.io/qt-5/qttest-index.html)

---

## 第 10 章 主窗口和动作

### 命令模式

在软件中，可能会提供不同的撤销动作(undo) 来降低用户的焦虑感并鼓励其尝试新功能。与确认对话框相比，这可能是一种更好的选择。Qt 提供了一些基本的 undo 功能，以使编写带 undo 功能的应用程序更为简单。

Qt 类中的 [QUndoCommand](https://doc.qt.io/qt-5/qundocommand.html) ,[QRunnable](https://doc.qt.io/qt-5/qrunnable.html) 和 QAction ,可以认为在一定程度上实现了这一模式。

利用 Qt 实现命令模式非常简单：

- 可以创建一个命令并在适当的容器（例如，QQueue）内对它们进行排队
- 通过把 QUndoCommand 放到 [QUndoStack](https://doc.qt.io/qt-5/qundostack.html) 上即可得到执行。
- 如果需要同时执行各个命令，则可以从 QRunnable 派生它们，并使用 QtConcurrent::Run() 在线程池中对它们进行调度。
- 或许会把一些命令序列化到文件并在随后再次对它们进行调用（也有可能是在另一台机器上），以用来实现批处理或者分布式执行。

---

## 第 12 章 元对象，属性和反射编程

本章将引入反射(reflection)的基本思想。所谓反射，就是指对象成员的自我检查。使用发射编程，就可以编写出通用的操作，可对具有不同结构的类进行操作。使用通用的值存储器 QVariant ，就可以按照一种统一的方式来对基本类型和其他普通类型进行操作。

### QMetaObject - 元对象模式

所谓元对象(meta object)，就是描述另一个对象结构的对象。

- 元对象模式
  - [QMetaObject](https://doc.qt.io/qt-5/qmetaobject.html) 是元对象模式(MetaObject Pattern)的一个 Qt 实现，它提供了一个 QObject 对象所拥有的属性和方法的信息。元对象模式有时也被称为反射模式(Reflection Pattern)。

一个拥有元对象的类就可以支持反射。这是一个许多面向对象语言都所具有的性质。虽然 C++ 中不存在反射，但 Qt 的元对象编译器(MetaObject compiler,moc) 可以为 QObject 生成支持这种机制的代码。

只要满足一定的条件，每个派生自 QObject 的类都会拥有一个由 moc 为其生成的 QMetaObject。QObject 拥有一个成员函数[QObject::metaObject()](https://doc.qt.io/qt-5/qobject.html#metaObject)，它能够返回指向对象的 QMetaObject 的指针。

The functions you are most likely to find useful are these ([details Link](https://doc.qt.io/qt-5/qmetaobject.html#details)):

- className() returns the name of a class.
- superClass() returns the superclass's meta-object.
- method() and methodCount() provide information about a class's meta-methods (signals, slots and other invokable member functions).
- enumerator() and enumeratorCount() and provide information about a class's enumerators.
- propertyCount() and property() provide information about a class's properties.
- constructor() and constructorCount() provide information about a class's meta-constructors.

信号和槽机制也同样需要依赖于 QMetaObject 。
通过使用 QMetaObject 和 [QMetaProperty](https://doc.qt.io/qt-5/qmetaproperty.html),就可以编写足够通用的代码来处理所有具有自我描述能力的类。

### 类型识别和 qobject_cast

除了 C++ 的 RTTI 运算符 dynamic_cast 和 typeid 之外，Qt 还提供了两种运行时的类型识别机制：

- [qobject_cast](https://doc.qt.io/qt-5/qobject.html#qobject_cast)
- [QObject::inherits()](https://doc.qt.io/qt-5/qobject.html#inherits)(不建议使用)

qobject_cast 实际是一个向下转换运算符，类似于 dynamic_cast。取决于所使用的编译器，你或许可以发现，qobject_cast 的运行速度要比 dynamic_cast 快 5 到 10 倍。

- 注意
  - qobject_cast 的实现没有使用 C++ RTTI ，该运算符的代码时由元对象编译器生成的。
  - 把 qobject_cast 用于非 QObject 的基类时，需要把每个基类都放到一个形如 [Q_INTERFACES](https://doc.qt.io/qt-5/qobject.html#Q_INTERFACES)(BaseClass1 BaseClass2)的代码行内，并把它放到类定义中 Q_OBJECT 宏的后面。

### 属性系统

[The Property System](https://doc.qt.io/qt-5/properties.html)

```c++
class MyClass : public QObject
{
    Q_OBJECT
    Q_PROPERTY(Priority priority READ priority WRITE setPriority NOTIFY priorityChanged)

public:
    MyClass(QObject *parent = 0);
    ~MyClass();

    enum Priority { High, Low, VeryHigh, VeryLow };
    Q_ENUM(Priority)

    void setPriority(Priority priority)
    {
        m_priority = priority;
        emit priorityChanged(priority);
    }
    Priority priority() const
    { return m_priority; }

signals:
    void priorityChanged(Priority);

private:
    Priority m_priority;
};

MyClass *myinstance = new MyClass;
QObject *object = myinstance;

myinstance->setPriority(MyClass::VeryHigh);
object->setProperty("priority", "VeryHigh");
```

#### Q_PROPERTY 宏 - 描述 QObject 属性

属性功能使得我们可以选择访问数据成员的方式：

- 直接访问，通过经典的获取函数和设置函数(getter/setter)。速度更快，更为有效。
- 间接访问，通过 QObject/QMetaObject 接口（可让代码复用性更好）。

[Q_PROPERTY(...)](https://doc.qt.io/qt-5/qobject.html#Q_PROPERTY)

#### QVariant 类：属性访问

可以通过[QVariant QObject::property(const char *name) const](https://doc.qt.io/qt-5/qobject.html#property) 函数来获得任意属性的值。

#### 动态属性

即使未用 Q_PROPERTY 在类中定义，在 QObject 中加载和存储一些属性也是可能的。

固定属性的清单来自 QMetaObject。使用 QMetaObject::read() 或者 QObject::property() 可以查询属性的值。propertyCount() 函数会设置对 QMetaProperty 清单进行遍历的条件。

QMetaObject 无法知道动态属性。相反，要知道动态属性必须得使用 QObject 的一些方法。可以对利用 [QObject::dynamicPropertyNames()](https://doc.qt.io/qt-5/qobject.html#dynamicPropertyNames) 返回的 QList 清单中的名称进行遍历，并可用 [QVariant QObject::property(const char *name) const](https://doc.qt.io/qt-5/qobject.html#property) 获得其值。

### 元类型，声明和注册

[QMetaType](https://doc.qt.io/qt-5/qmetatype.html)  是一个用于值类型的辅助类(helper class)。对于 60 多种内置类型，QMetaType 为每一个类型ID关联了一个类型名，从而使构造和析构可以在运行时动态发生。有一个名称为[QMetaType::Type](https://doc.qt.io/qt-5/qmetatype.html#Type-enum) 的公共枚举，它有所有 QVariant 兼容类型的值。在 QMetaType::Type 中的枚举值和 [QVariant::Type](https://doc.qt.io/qt-5/qvariant-obsolete.html#Type-enum) 枚举值一样。(注：官方更建议使用QMetaType::Type)。

通过使用 [Q_ENUMS](https://doc.qt.io/qt-5/qobject-obsolete.html#Q_ENUMS) 宏，我们已在 QVariant 系统中加入了一些自定义的枚举类型。使用 [Q_DECLARE_METATYPE(MyType)](https://doc.qt.io/qt-5/qmetatype.html#Q_DECLARE_METATYPE) 宏也有可能把自己的值类型加到 QMetaType 列表中。如果 MyType 有公共的默认复制构造函数和公共的复制构造函数以及一个公共的析构函数，Q_DECLARE_METATYPE 宏使得它可用作 QVariant 中的自定义类型。

#### qRegisterMetaType()

要注册的元类型必须已经用 Q_DECLARE_METATYPE 声明过。模板函数[qRegisterMetaType<T>()](https://doc.qt.io/qt-5/qmetatype.html#qRegisterMetaType-1) 会注册类型 T 并返回由 QMetaType 使用的内部 ID。这个函数有一个重载版本 [template <typename T> int qRegisterMetaType(const char *typeName)](https://doc.qt.io/qt-5/qmetatype.html#qRegisterMetaType),它可以让你注册一个名称作为类型 T 的名称。对于这个函数的调用必须早早地出现在主程序中，一定要在任何试图以一种动态方式尝试使用该注册地类型之前。

声明了元类型之后，存储一个值到 QVariant 中是可能的。

[Custom Type Example](https://doc.qt.io/qt-5/qtcore-tools-customtype-example.html)

### invokeMethod()

Qt 把信号连接到槽需要一种机制：通过名称以类型安全的方式来间接调用这些槽。当调用槽时，实际是由 [invokeMethod()](https://doc.qt.io/qt-5/qmetaobject.html#invokeMethod) 完成的。

```c++
QMetaObject::invokeMethod(thread, "quit",
                          Qt::QueuedConnection);

```

向函数传递参数，需要使用 [Q_ARG](https://doc.qt.io/qt-5/qmetaobject.html#Q_ARG) 宏创建一些值,这样会返回一个 [QGenericArgument](https://doc.qt.io/qt-5/qgenericargument.html),它封装了单个参数的类型和值信息。

```c++
QString retVal;
QMetaObject::invokeMethod(obj, "compute", Qt::DirectConnection,
                          Q_RETURN_ARG(QString, retVal),
                          Q_ARG(QString, "sqrt"),
                          Q_ARG(int, 42),
                          Q_ARG(double, 9.7));
```

---

## 第 14 章 验证和正则表达式

输入数据通常需要进行过滤或者验证以确保输入是否合适。
验证器(validator)和输入掩码(input mask)让开发人员可以精细地控制 QLineEdit 的行为，可用于对特定的输入类型进行限制。

### 输入掩码

所谓输入掩码，是一种控制用户在输入窗口中可键入的内容的主动模式。它有助于防止输入某些类型不正确的数据。每个 QLineEdit 都有一个 QString 属性用来存储掩码字符串中哪个位置处的何种字符是允许的。该字符串由一些特殊的、预定义的掩码字符和一些占据输入字符串相应位置的（可选的）普通字符构成。

详见 [QLineEdit inputMask](https://doc.qt.io/qt-5/qlineedit.html#inputMask-prop)

### 验证器

验证器(validator) 是可附加到输入窗口(例如 QLineEdit,QSpinBox和 QComboBox)的不可见对象，能够提供一个检查用户输入的通用框架。Qt 有一个名称为 [QValidator](https://doc.qt.io/qt-5/qvalidator.html) 的抽象类，它为所有内置的和自定义的验证器定义了接口。

[void QLineEdit::setValidator(const QValidator *v)](https://doc.qt.io/qt-5/qlineedit.html#setValidator)

### 正则表达式

Qt 提供了一个 [QRegExp](https://doc.qt.io/qt-5/qregexp.html) 类，它实现了 Perl 风格的扩展正则表达式语言的大部分功能。

### 正则表达式验证

[QRegExpValidator](https://doc.qt.io/qt-5/qregexpvalidator.html) 类使用了 QRegExp 来验证输入字符串。

eg:

```c++
// regexp: optional '-' followed by between 1 and 3 digits
QRegExp rx("-?\\d{1,3}");
QValidator *validator = new QRegExpValidator(rx, this);

QLineEdit *edit = new QLineEdit(this);
edit->setValidator(validator);
```

### 子类化 QValidator

当验证用户输入的需求超出了简单的数字范围或者正则表达式验证的能力时，可以通过对 QValidator 派生来定义自己的验证器类。

## 第 17 章 并发

[QProcess](https://doc.qt.io/qt-5/qprocess.html) 和 [QThread](https://doc.qt.io/qt-5/qthread.html) 提供了两种并发方式。

### QThread 和 QtConcurrent

Qt 的线程模型允许线程的优先次序和控制。QThread 是一个低级(low-level)类，适合用于显示地构建长期运行的线程。

[QtConcurrent](https://doc.qt.io/qt-5/qtconcurrent-index.html) 是一个命名空间，提供了用于编写并发软件的更高层次的类和算法。该命名空间中有一个重要的类， [QThreadPool](https://doc.qt.io/qt-5/qthreadpool.html),这是一个管理线程池的类。每个 Qt 应用程序都有一个 QThreadPool::globalInstance() 函数，它的带有一个推荐的最大线程数，在大多数系统上，处理核的数量就是该值的默认值。

QThreadPool example:

```c++
class HelloWorldTask : public QRunnable
{
    void run() override
    {
        qDebug() << "Hello world from thread" << QThread::currentThread();
    }
};

HelloWorldTask *hello = new HelloWorldTask();
// QThreadPool takes ownership and deletes 'hello' automatically
QThreadPool::globalInstance()->start(hello);
```

借助于 [QtConcurrent](https://doc.qt.io/qt-5/qtconcurrent-index.html) 中函数式的 map/fiter/reduce 算法（它们可将函数并行用到容器中的每个项），通过将进程分布在由线程池管理的多个线程上，可编写一个能够自动利用系统多核的程序。另外，在命令模式和利用 QtConcurrent::run() 工作时可把 QRunnable 用作基类。在这些情况下，无须显式地创建线程或直接管理它们，只需简单地把工作片段描述为具有正确接口的对象即可。

- 线程指南
  - 一般情况下，要尽可能避免使用线程，而是用 Qt 事件循环与 QTimer 、非阻塞 I/O操作、信号以及短持续事件槽相结合的方法来代替。此外，可以在主线程中长期运行的循环调用 QApplication::processEvents() ，以使执行工作时图形界面可以保持响应。
  - 要驱动动画，建议使用 QTimer,QTimeLine 或者动画框架。这些 API 并不需要额外创建其他线程。它们允许访问动画代码中的 GUI 对象而且不会妨碍图形用户界面的响应。
  - 如果要完成 CPU 密集工作并希望将其分配给多个处理核，可以把工作分散到 QRunnable 并通过以下这些推荐做法来实现线程安全：
    - 无论何时，都尽可能使用 QtConcurrent 算法把 CPU 密集型计算工作分散给多线程，而不是自己编写 QThread 代码。
    - 除了主线程之外，不要从其他任何线程访问图形用户界面（这也包括那些 QWidget 派生的类、QPixmap 和其他与显卡相关的类）。这包括读取操作，比如查询 QLineEdit 中输入的文本。
    - 要其他线程中处理图像，使用 QImage 而不是 QPixmap。
    - 不要调用 QDialog::exec() 或者从除主线程之外的任何线程创建 QWidget 或 QIODevices 的子类。
    - 使用 QMutex，QReadWriteLock 或者 QSemaphore 以禁止多个线程同时访问临界变量。
    - 在一个拥有多个 return 语句的函数中使用 QMutexLocker(或 QReadLocker,QWriteLocker)，以确保函数从任意可能的执行路径均可释放锁。
    - 创建 QObject 的线程，也称线程关联（thread affinity）,负责执行那个 QObject 的槽。
    - 如果各 QObject 具有不同的线程关联，那么就不能以父 - 子关系来连接它们。
    - 通过从 run() 函数直接或者间接地调用 QThread::exec() ，可以让线程进行事件循环。
    - 利用 [QApplication::postEvent()](https://doc.qt.io/qt-5/qcoreapplication.html#postEvent) 分发事件，或使用队列式地信号/槽连接，都是用于线程间通信地安全机制 - 但需要接收线程处于事件循环中。
    - 确保每个跨线程连接的参数类型都用 qRegisterMetaType() 注册过。

#### 线程安全和 QObject

可重入函数就是一个可以由多个线程同时调用的函数，其中任意的两次调用都不会试图访问相同的数据。线程安全的方法在任何时间都可以同时由多个线程调用，因为任何共享数据都会在某种程度上（例如，通过 QMutex ）避免被同时访问。如果一个类的所有非静态函数都是可重入的或者线程安全的，那么它就是可重入的或者式线程安全的。
一个 QObject 在它所 “属于” 或者有关联的线程中被创建。其各子对象也必须属于同一个线程。Qt 禁止存在跨线程的父-子关系。

- [QObject::thread()](https://doc.qt.io/qt-5/qobject.html#thread) 可返回它的所有者线程，或者是其关联线程。
- [QObject::moveToThread()](https://doc.qt.io/qt-5/qobject.html#moveToThread) 可将其移动到另一个线程。

- moveToThread(this)
  - 每一个 QThread 的本质都是一个 QObject，这决定了它与其创建的线程存在关联，而不是与它启动的线程存在关联。
  - QThread 并不是线程本身，而是该线程的管理器。实际上，QThread 是一个底层线程 API 的封装器。
  - 这就意味着，当信号连接到这个 QThread 的槽上时，槽函数的执行是在其创建线程，而不是在其管理的线程进行的。
  - 一些程序通过改变 QThread 的定义使它可表示其管理的线程并在该线程内执行它的槽。这些程序使用一种变通方法：在 QThread 的构造函数中使用 moveToThread(this)。这一变通方法的主要问题是，在线程退出后，通过 post 方式派发给该对象的事件如何处理留下不确定性。
  - 要说明的是，尽管 Qt 自身文档和一些旧示例程序都使用了这种做法，它仍是不再被推荐的。一方面，在托管的线程终止时，对于事件和信号会发生什么并不确定。另一方面，它混淆了 QRunnable 和 QThread 的目的，并将太多的职责加到单一的类中。

[QtConcurrent 更多示例](https://github.com/qt/qtbase/tree/5.15.2/examples/qtconcurrent)

## 附录

### UML 建模工具

两个开源的 UML 工具(都使用了某种 XML 方言作为其原生文件格式)：

- [Umbrello](https://umbrello.kde.org/) 是 KDE 中的 UML 建模器 - 它可以直接导入 C++ 代码，这使得拖拽导入类到图表变得非常容易。也可以通过 UML 图生成 C++ 代码
- [Dia](http://dia-installer.de/) 是一个 Gnome 实用工具，它是一个带有某些  UML 特性的通用性更强的图标工具。有很多插件和实用工具可用于往/从 Dia 中导入代码、导出图形到其他语言和格式。

---

- [上一级](README.md)
- 上一篇 -> [Qt Creator 使用](QtCreatorTips.md)
- 下一篇 -> [解决 qt 程序的依赖项](deployqt.md)
