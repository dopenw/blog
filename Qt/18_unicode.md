# 18. 国际化


<!-- @import "[TOC]" {cmd="toc" depthFrom=1 depthTo=6 orderedList=false} -->
<!-- code_chunk_output -->

- [18. 国际化](#18-国际化)
  - [使用 Unicode](#使用-unicode)

<!-- /code_chunk_output -->

除了采用英语和许多欧洲语系中的拉丁字符之外，Qt 也为世界上其他的文字系统提供了广泛的支持：
* Qt 在整个应用程序编程接口及其内部都使用 Unicode。无论用户用于用户接口的是何种语言，应用程序都可以为所有的用户提供类似的支持。
* Qt 的文本引擎可以处理所有主要的非拉丁文文字系统，其中包括阿拉伯文、中文、西里尔文、希伯来文、日文、韩文、泰文和印度文。
* Qt 的布局引擎可以为从右到左的文本布局提供支持，比如对阿拉伯文和希伯来文的支持。
* 一些特定的语言在输入文本时要求使用特殊的输入法。比如像 QLineEdit 和 QTextEdit 这样的一些编辑器窗口部件，都可以与安装在用户系统中的任意输入法和谐地工作在一起。

通常情况下，只允许用户使用它们自己的本地语言输入文本是不够的，最好要让整个用户界面允许翻译才行。Qt 让这些变得非常简单：只需使用 tr(),并且使用 Qt 的支持工具为所需的语言准备相应的翻译文件即可。Qt 为翻译人员提供了 [Qt Linguist](https://doc.qt.io/qt-5/qtlinguist-index.html) 的图形用户界面工具。配上两个通常由开发人员运行的命令行语句 - lupdate 和 lrelease,Qt Linguist 就可以为翻译提供完美支持了。

对于大多数的应用程序，程序会在一开始启动的时候根据用户的本地设置载入所需的翻译文件。但是在少数情况下，对用户来说，要求程序在运行的时候根据也能切换语言，可能也是很有必要的。而利用 Qt,使这一切变得完全可能起来，虽然这的确还需要再做一些其他工作。在翻译文本的长度比原有文本的长度长的时候，也要多亏了 Qt 的布局系统，使得不同的用户接口部分会自动进行调整，从而可以为翻译文本腾出空间。


## 使用 Unicode
Unicode 是一种支持世界上绝大多数文字系统的字符编码标准。Unicode 最初的思想是在存储 Unicode 字符时使用 16 位而不是 8 位，这样它就可以大约具有 65000 个字符编码，而不是只有 256 个。 Unicode 在同样的编码位置以子集的形式包含了 ASCII 和 ISO 8859-1(Latin-1)。例如，字符 "A" 的 ASCII 、 Latin-1 编码和 Unicode 编码都是 0x41,而 字符 “？” 的 Latin-1 编码 和 Unicode 编码都是 0xD1.


Qt 中的 QString 类用于将字符串存储为 Unicode.在 QString 中的每一个字符都是一个 16 位 的 QChar ,而不是一个 8 位的 char。
```c++
str[0] = 'A';
// 等价于
str[0] = QChar(0x41);
```

如果源文件的编码格式是 Latin-1,那么指定 Latin-1 字符就非常简单：
```c++
str[0] = 'Ñ';
```
如果源文件中还使用了其他的编码格式，那么使用数字赋值的方式会更好些：
```c++
str[0] = QChar(0xD1);
```
我们可以使用任何一个 Unicode 字符的数字值来指定它。
```c++
str[0] = QChar(0x03A3);
str[0] = QChar(0x20AC);
```

[Unicode 所支持的所有字符的数值列表](https://www.unicode.org/standard/standard.html)。如果你很少需要非 Latin-1 的 Unicode 字符，在线查找这些字符就已经足够了。但是，为了在 Qt 程序中输入 Unicode 字符串，Qt 还是提供了很多简便的方法，正如我们在这一节的稍后部分将看到的那样。

使用 QChar 编程和使用 char 编程有一点点不同。为了获得一个 QChar 的数字值，可以对他调用 `unicode()` 。为了获得一个 QChar (作为一个 char)的 ASCII 或者 Latin-1 的值，则要求对他调用 `toLatin1()` 。对于那些非 Latin-1 的字符，`toLatin1()` 将会返回 "\0"。

如果我们知道一个程序中的所有字符都是 ASCII 字符，那么可以在 toLatin1() 的返回值上使用 `isalpha() 、 isdigit() 和 isspace()` 这样的标准 <cctype> 函数。然而，在执行这些操作使使用 QChar 的成员函数会是更好的方法，因为它们可用于任意的 Unicode 字符。QChar 提供的这些函数包括 `isPrint() isPunct() isSpace() isMark() isLetter() isNumber() isLetterOrNumber() isDigit() isSymbol() isLower() isUpper()` 。eg:
```c++
// 这个代码片段可以用于区分任意的大写或者小写格式的字母，包括 拉丁文、希腊文和西里尔文。
if (ch.isDigit() || ch.isUpper())
  ...
```

当读取和写入文本文件时，需要给予特殊的关注。文本文件可以使用很多种编码格式，并且通常很有可能需要从文本文件的内容来猜测它的编码格式。默认情况下， QTextStream 对于读取和写入通常都使用系统的本地 8 位 编码格式[这可以通过 [QTextCodec::codecForLocale()](https://doc.qt.io/qt-5/qtextcodec.html#codecForLocale) 函数获得]。对于美国和西欧的本地设置，这种做法通常获得的就是 Latin-1。

如果要设计自己的文件格式，并且希望能够读取和写入任意的 Unicode 字符，那么在开始写入 QTextStream 之前调用以下代码：
```c++
stream.setCodec("UTF-16");
stream.setGenerateByteOrderMark(true);
```

这些数据被保存为 UTF-16 ，这是一种每个字符都使用两个字节存储的格式，并且它会使用一个特殊的 16 位值（Unicode 字节序掩码，0xFFFE）作为前缀，用于识别该文件采用的是 Unicode 编码，它也用来存储格式是以高字节在前还是高字节在后。 UTF-16 格式和 QString 的内存表示形式一致，所以读取和写入 UTF-16 格式的 Unicode 字符串的速度非常块。另一方面，当我们把纯粹的 ASCII 数据保存为 UTF-16 格式的时候，总是会带来固有的开销，因为它存储每个字符的时候使用的是两个字节，而不是只用一个字节。

通过使用适当的 QTextCodec 来调用 `setCodec()` ，还可以指定其他的编码方法。QTextCodec 是一个可以在 Unicode 和给定编码格式之间进行转换的对象。Qt 在很大范围的上下文关系上都使用了 QTextCodec。在 Qt 内部，把它们用于对字体、输入法、剪贴板、拖放和文件名的支持中。但是当我们编写 Qt 应用程序的时候，也可以使用它们。

当读取文本文件时，如果这个文件是以字节顺序标记(Byte Order Mark,BOM)开始的，那么 QTextStream 就会自动检测到 UTF-16。通过调用 `setAutoDetectUnicode(false)` ，可以关闭这一行为。如果假定数据不是以字节顺序标记开始的，那么在读取开始之前最好能够使用 "UTF-16" 先调用一下 `setCodec()`。

Unicode 完全支持的另外一种编码格式是 UTF-8 。对于 UTF-16,它的主要优点在于：它是 ASCII 的一个超集。位于  0x00 和 0x7F 范围之内的任何字符都被表示为一个单一字节。而其他字符，包括位于 0x7F 以上的 Latin-1 字符，则都使用多字节序列进行表示。对于以 ASCII 为主的文本，UTF-8 占用的空间约为 UTF-16 占用空间的一半。为了在 QTextStream 中使用 UTF-8 ,在读取和写入之前，可以以 "UTF-8" 作为编码格式的名字先调用一下 setCodec()。

如果我们总是希望忽略用户的本地设置而读取或者输出 Latin-1 字符，则可以把 QTextStream 的编码格式设置为 "ISO 8859-1"。例如：
```c++
QTextStream in(&file);
in.setCodec("ISO 8859-1");
```

一些文本格式会在文件头指明它们所使用的编码格式。这个文件头通常是普通的 ASCII 编码，这样可以确保不管用什么样的编码格式（这里我们假设它是 ASCII 的一个超集），都可以正确地读取这个文件。eg: XML 文件格式。XML 文件通常编码为 UTF-8 或者 UTF-16 。读取它们的适当方式是使用带 “UTF-8” 的 setCodec() 调用。如果格式是 UTF-16，QTextStream 将会自动检测到该格式并且进行自我调整。XML 文件的 <?xml?> 样式的文件头有时会包含一个 encoding 参数，eg：
```xml
<?xml version="1.0" encoding="EUC-KR"?>
```

由于 QTextStream 不允许在开始读取字符之后再改变这个编码格式，因此，考虑到显式编码的问题，正确读取文件的方法应当是使用适当的编码格式[可以使用 [QTextCodec::codecForName()](https://doc.qt.io/qt-5/qtextcodec.html#codecForName) 获得] 重新开始读取文件。对于 XML 的这种情况，可使用 Qt 的 XML 类避免自己处理编码格式。


QTextCodec 的另外一种用法是用于指明在源代码中出现的字符串的编码方式。假设有这样一个例子，有一个日本的程序员团队正在编写一个主要面向日本家庭市场的应用程序。这些程序员喜欢使用编码方式为 EUC-JP 或者 Shift-JIS 的文本编辑器编写它们的源代码。这样的编辑器可以让他们方便地输入日文字符，所以，它们就可以输入像下面这样的程序代码：
```c++
QPushButton *button = new QPushButton(tr("こんにちは"));
```
默认情况下，Qt 会把 tr() 的参数当作 Latin-1 。为了改变这一点，可以调用静态函数 [QTextCodec::setCodecForTr()](https://doc.qt.io/archives/qt-4.8/qtextcodec.html#setCodecForTr) [注：qt5 不再支持]

例如：
```c++
QTextCodec::setCodecForTr(QTextCodec::codecForName("EUC-JP"));
```

这个调用必须在第一次对 tr() 调用之前完成。通常，应当在  main() 函数中来完成它，并且是在创建完 QApplication 对象之后就马上来做这项工作。

而在程序中给定的其他字符串仍将会当作 Latin-1 字符串。如果程序员想在以上情况下也输入日文字符，则可以使用 QTextCodec 明确将它们转换成为 Unicode:
```c++

QString text = japaneseCodec->toUnicode("シーフード料理");
```
作为选择，当在 const char * 和 QString 之间进行转换的时候，可以通过调用 [QTextCodec::setCodecForCStrings()](https://doc.qt.io/archives/qt-4.8/qtextcodec.html#setCodecForCStrings) [注：qt5 不再支持] 来让 Qt 使用某种特殊的编解码器：
```c++
QTextCodec::setCodecForCStrings(QTextCodec::codecForName("EUC-JP"));
```

在 Qt 5 中要注意这个：
* [QTextCodec中的setCodecForTr等终于消失了 (Qt5)](https://blog.csdn.net/dbzhang800/article/details/7325698)

[Qt 所支持的编码方式](https://doc.qt.io/qt-5/qtextcodec.html#details)

对于列出的所有这些编码方式，QTextCodec::codecForName() 总是可以返回一个有效的指针。通过对 QTextCodec 的子类化，也可以支持其他类型的编码方式。


[上一级](README.md)
[上一篇](17_onlineHelp.md)
[下一篇](19_cutomStyle.md)
