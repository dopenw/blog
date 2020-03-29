# Effective c++ 改善程序与设计的55个具体做法


<!-- @import "[TOC]" {cmd="toc" depthFrom=1 depthTo=6 orderedList=false} -->
<!-- code_chunk_output -->

- [Effective c++ 改善程序与设计的55个具体做法](#effective-c-改善程序与设计的55个具体做法)
  - [让自己习惯 c++](#让自己习惯-c)
    - [条款 01：视 C++ 为一个语言联邦](#条款-01视-c-为一个语言联邦)
    - [条款 02：尽量以 const,enum,inline 替换 #define](#条款-02尽量以-constenuminline-替换-define)
    - [条款 03：尽可能使用 const](#条款-03尽可能使用-const)
      - [const 成员函数](#const-成员函数)
      - [在 const 和 non-const 成员函数中避免重复](#在-const-和-non-const-成员函数中避免重复)
    - [条款 04：确定对象被使用前已先被初始化](#条款-04确定对象被使用前已先被初始化)
      - [请以 local static 对象替换 non-local static 对象](#请以-local-static-对象替换-non-local-static-对象)
  - [构造/析构/赋值运算](#构造析构赋值运算)
    - [条款 05： 了解 c++ 默默编写并调用的哪些函数](#条款-05-了解-c-默默编写并调用的哪些函数)
    - [条款 06： 若不想使用编译器自动生成的函数，就该明确拒绝](#条款-06-若不想使用编译器自动生成的函数就该明确拒绝)
    - [条款 07：为多态基类声明 virtual 析构函数](#条款-07为多态基类声明-virtual-析构函数)
    - [条款 08：别让异常逃离析构函数](#条款-08别让异常逃离析构函数)
    - [条款 09：绝不在构造和析构过程中调用 virtual 函数](#条款-09绝不在构造和析构过程中调用-virtual-函数)
    - [条款 10：令 operator= 返回一个 reference to * this](#条款-10令-operator-返回一个-reference-to-this)
    - [条款 11：在 operator= 中处理“自我赋值”](#条款-11在-operator-中处理自我赋值)
    - [条款 12：复制对象时务忘其每一个成分](#条款-12复制对象时务忘其每一个成分)
  - [资源管理](#资源管理)
    - [条款 13： 以对象管理资源](#条款-13-以对象管理资源)
    - [条款 14： 在资源管理类中小心 copying 行为](#条款-14-在资源管理类中小心-copying-行为)
    - [条款 15 ：在资源管理类中提供对原始资源的访问](#条款-15-在资源管理类中提供对原始资源的访问)
    - [条款 16： 成对使用 new 和 delete 时要采取相同形式](#条款-16-成对使用-new-和-delete-时要采取相同形式)
    - [条款 17：以独立语句将 newed 对象置入智能指针](#条款-17以独立语句将-newed-对象置入智能指针)

<!-- /code_chunk_output -->

## 让自己习惯 c++
### 条款 01：视 C++ 为一个语言联邦
为了理解 C++，你必须认识其主要的次语言。幸运的是总共只有四个：
* `C`：说到底 C++ 仍是以 C 为基础。
* `Object-Oriented C++` :这部分是面向对象设计之古典守则在 C++ 上的最直接实施。
* `Template C++`:模板
* `STL`

因此，C++ 并不是一个带有一组守则的一体语言；它是从四个次语言组成的联邦政府，每个次语言都有自己的规约。记住这四个次语言你就会发现 C++ 容易了解得多。

请注意：
* C++ 高效编程守则视状况而变化，取决于你使用 C++ 的那一部分。

### 条款 02：尽量以 const,enum,inline 替换 #define

宏只是一个简单的替换，你需要小心的使用它,并且使用宏不好调试

请记住：
* 对于单纯常量，最好以 `const` 对象或 `enum` 替换 `#define`
* 对于形似函数的宏(macros)（注：该方式不会招致函数调用带来的额外开销）,最好改用 `inline` 函数替换 `#define`

### 条款 03：尽可能使用 const
const 的一件奇妙的事情是，它允许你指定一个语义约束（也就是指定一个 “不该被改动”的对象），而编译器会强制实施这项约束。它允许你告诉编译器和其他程序员某值应该保持不变。只要这（某值保持不变）是事实，你就该明确说出来，因为说出来可以获得编译器的相助，确保这条约束不被违反。

关于 const 的使用可以参考之前写的文章 [const 限定符](const.md)

#### const 成员函数
将 const 实施于成员函数的目的，是为了确认该成员函数可作用于 const 对象身上。这一类函数之所以重要，基于两个理由：
* 他们使 class 接口比较容易被理解。
* 他们使 “操作 const 对象”成为可能。这对编写高效率代码是个关键。


两个成员函数如果只是常量性(constness) 不同，可以被重载。考虑以下 class:
```c++
class TextBlock{
public:
	...
	const char & operator [] (std::size_t position) const
	{
		return text[position];
	}
	char & operator[] (std::size_t position)
	{
		return text[position];
	}
private:
	std::string text;
}
```

TextBlock 的 operator[] s 可被这么使用：
```c++
TextBlock tb("Hello");
std::cout<<tb[0]; // 调用 no-const operator []

const TextBlock ctb("World");
std::cout<<ctb[0]; // 调用 const operator []
```

上述的例子太过造作，下面这个比较真实：
```c++
void print(const TextBlock & ctb)
{
	std::cout<<ctb[0]; // 调用 const operator []
	...
}
```

```c++
tb[0]='x'; //没问题
ctb[0]='x'; //错误 ！
```

也清注意，non-const operator [] 的返回类型是个 reference to char,不是 char 。如果 operator [] 只是返回一个 char ,下面这样的句子就无法通过编译：
```c++
tb[0]='x';
```
那是因为，如果函数的返回值是个内置类型，那么改动函数返回值从来就不合法。纵使合法， c++ 以 by value 返回对象这一事实意味被改动的其实是 tb.text[0] 的一个副本，不是 tb.text[0] 自身，那不会是你想要的行为。

成员函数如果是 const 意味着什么 ？ 这又两个流行概念： bitwise constness (又称为  physical constness) 和 logical constness。

bitwise 阵营的人相信，成员函数只有在不更改对象之任何成员变量 （static 除外）时才可以说是 const。

不幸的是许多成员函数虽然不十足具备 const 性质却能通过 bitwise 测试。

看下面的例子：

```c++
class CTextBlock {
private:
	char * pText;

public:
	...
	char & operator [] (std::size_t position) const
	{
		return pText[position];
	}
};
```

这个 class 不适当的将其 operator [] 声明为 const 成员函数，而该函数却返回一个 reference 指向对象内部值。

编译器允许这样：
```c++
const CTextBlock cctb("Hello");
char * pc=&cctb[0];

*pc ='J'; // cctb 现在有了 "Jello" 这样的内容
```

这种情况导出了所谓i的 logical constness。这一派拥护者主张，一个 const 成员函数可以修改它所处理的对象内的某些 bits，但只有在客户端侦测不出的情况下才得如此。

参考下面的例子：
```c++
class CTextBlock {
private:
	char * pText;
	mutable std::size_t textLength;
	mutable bool lengthIsVaild;

public:
	...
	std::size_t length() const;
};

std::size_t CTextBlock::length() const
{
	if(!lengthIsVaild)
	{
		textLength=std::strlen(pText);
		lengthIsVaild=true;
	}
	return textLength;
}
```

#### 在 const 和 non-const 成员函数中避免重复

对于 “bitwise-constness 非我所欲”的问题， mutable 是个解决办法，但它不能解决所有的 const 相关难题。举个例子：

```c++
class TextBlock
{
public:
	...
	const char & operator [] (std::size_t position) const
	{
		... //bounds checking
		... //log access data
		... //verify data integrity
		return text[position];
	}

	char & operator [] (std::size_t position) const
	{
		... //bounds checking
		... //log access data
		... //verify data integrity
		return text[position];
	}
private:
	std::string text;
};
```

可以用如下代码代替：
```c++
class TextBlock
{
	...
	const char & operator [] (std::size_t position) const // 一如既往
	{
		...
		...
		...
		return text[position];
	}

	char & operator [] (std::size_t position) //现在只调用 const op[]
	{
		return
		const_cast<char &>( //将 op[] 返回值的 const 移除
			static_cast<const TextBlock&>(this) // 为i * this 加上 const
			[position] // 调用 const op[]
		);
	}

	...
}
```

值的了解的是，反向做法 ：令 const 版本调用 non-cast 版本以避免重复-并不是你该做的事情。


请记住：
* 将某些东西声明为 const 可帮助编译器侦测出错误用法。
* 编译器强制实施 bitwise constness,但你编写程序应该使用 “概念上的常量性” (conceptual constness)
* 当 const 和 non-const 成员函数有着实质等价的实现时，令 non-const 版本调用 const 版本可避免代码重复。

### 条款 04：确定对象被使用前已先被初始化

读取未初始化的值会导致不明确的行为。

这个规则很容易奉行，重要的是别混淆了赋值(assignment)和初始化(initalization)。考虑一个用来表现通讯簿的 class ,其构造函数如下：
```c++
class PhoneNumber {...};
class ABEntry {
private:
	std::string theName;
	std::string theAddress;
	std::list<PhoneNumber> thePhones;
	int numTimesConsulted;

public:
	ABEntry (const std::string &name,const std::string & address,const std::list<PhoneNumber>&phones);
};

ABEntry::ABEntry(const std::string &name,const std::string & address,const std::list<PhoneNumber>&phones)
{
	theName=name;
	theAddress=address;
	thePhones=phones;
	numTimesConsulted=0;
	//以上都是 赋值 ，而非 初始化
}
```

ABEntry 构造函数的一个较佳的写法是，使用所谓的 member initalization list(成员初值列) 替换赋值动作：
```c++
ABEntry::ABEntry(const std::string &name,const std::string & address,const std::list<PhoneNumber>&phones):theName(name),
theAddress(address),thePhones(phones),numTimesConsulted(0)
{}
```
#### 请以 local static 对象替换 non-local static 对象
`编译单元`是指产出单一目标文件(single object file) 的那些源码。基本上它是单一源码文件加上其所包含入的头文件 (#include files)。

[C++编译器compliler与链接器Linker工作原理](https://www.jianshu.com/p/5b3aa1b55cb4)

现在，我们关心的问题涉及至少两个源码文件，每一个内含至少一个 non-local static 对象。
参考如下示例：
```c++
class FileSystem{
public:
	...
	std::size_t numDisks() const;
	...
};
extern FileSystem tfs; //预备给客户使用的对象
```
在另一个源码文件中：
```c++
class Directory{
public:
	Directory(params);
	...
};
Directory::Directory(params)
{
	...
	std::size_t disks=tfs.numDisks(); //使用 tfs 对象
	...
}
```

进一步假设，这些客户决定创建一个 Directory 对象，用来放置临时文件；
```c++
Directory tempDir(params); //为临时文件而做出的目录
```
现在，初始化次序的重要性显现出来了：除非tfs在 tempDir 之前先被初始化，否则 tempDir 的构造函数可能会用到尚未初始化的 tfs。但 tfs 和 tempDir 是不同的人在不同的时间于不同的源码文件建立起来的，它们是定义于不同编译单元内的 non-local static 对象。

如何确定 tfs 会在 tempDir 之前先被初始化？
```highlight
喔，你无法确定。再说一次，C++ 对 “定义于不同编译单元内的 non-local static 对象” 的初始化相对次序并无明确的定义。
```

幸运的是一个小小的设计便可完全消除这个问题。就像 Singleton 模式的一种实现那样。这种手法的基础在于:c++ 保证，函数内的 local static 对象会在 “该函数被调用期间” “首次遇上该对象之定义式”时会被初始化。
```c++
class FileSystem{...};
FileSystem &tfs()
{
	static FileSystem fs;
	return fs;
}
class Directory {...};
Directory::Directory(params)
{
	...
	std::size_t disks=tfs.numDisks();
	...
}
Directory &tempDir()
{
	static Directory td;
	return td;
}
```

请注意：
* 为内置型对象进行手工初始化，因为 C++ 不保证初始化它们。
* 构造函数最好使用成员初值列(member initialization list),而不要在构造函数本体内使用赋值操作 (assignment)。初值列列出的成员变量，其排列次序应该和它们在 class 中的声明次序相同。
* 为免除 “跨编译单元之初始化次序”问题，请以 local static 对象替换 non-local static 对象。

## 构造/析构/赋值运算
### 条款 05： 了解 c++ 默默编写并调用的哪些函数

什么时候 empty class （空类） 不再是个 empty class 呢？ 当 c++ 处理过它之后。是的，如果你自己没声明，编译器就会为它声明（编译器版本的）一个 copy 构造函数、一个 copy assignment 操作符和一个析构函数。此外如果你没有声明任何构造函数，编译器也会为你声明一个 default 构造函数。所有这些函数都是 Public 且 inline(见条款 30)。因此，如果你写下：
```c++
class Empty{};
```
这就好像你写下：
```c++
class Empty
{
public:
	Empty() {...}
	Empty(const Empty & rhs) {...}
	~Empty() {...}

	Empty & operator=(const Empty& rhs){...}
};
```

至于 copy 构造函数和 copy assignment 操作符，编译器创建的版本只是单纯地将来源对象的每一个 non-static 成员拷贝到目标对象。

### 条款 06： 若不想使用编译器自动生成的函数，就该明确拒绝

为驳回编译器自动（暗自）提供的机能，这又两种方法：
1. 可将相应的成员函数声明为 private并且不予实现；
```c++
class Sample{
public:
	...
private:
	...
	//只有声明
	Sample(const Sample &);
	Sample & operator=(const Sample &);
};
```
2. 使用 Uncopyable 这样的 base class；
```c++
class Uncopyable
{
protected: //允许 derived 对象构造和析构
	Uncopyable(){}
	~Uncopyable{}
private: //但组织 copying
	Uncopyable(const Uncopyable&);
	Uncopyable& operator=(const Uncopyable&);
};
```
为求阻止 Sample 对象被拷贝，我们唯一需要做的就是继承 Uncopyable:
```c++
class Sample:private Uncopyable
{
	...
};
```
这行的通，因为只要任何人-甚至是 member 函数 或 friend 函数 -- 尝试拷贝 Sample 对象，编译器便试着生成一个 copy 构造函数和 copy assignment操作符，而正如条款 12 所说，这些函数的“编译器生成版”会尝试调用其 base class 的对应的兄弟，那些调用会被编译器拒绝，因为其 base class 的拷贝函数是 private。
也可以使用 boost 提供的版本，那个 class 名为 [noncopyable](https://www.boost.org/doc/libs/1_63_0/libs/core/doc/html/core/noncopyable.html).

### 条款 07：为多态基类声明 virtual 析构函数

参见如下示例：
```c++
#include <iostream>
#include <string>

using namespace std;

class A {
public:
  A() { cout << "A create" << endl; }
  ~A() { cout << "A delete" << endl; }
  void show() { cout << "A show" << endl; }
};

class B : public A {
public:
  B() { cout << "B create" << endl; }
  ~B() { cout << "B delete" << endl; }

  void show() { cout << "B show" << endl; }
};

int main(int argc, char const *argv[]) {
  A * newB = new B();
  newB->show();
  delete newB;

  return 0;
}

```

output:
```
A create
B create
A show
A delete
```

添加virtual

```c++
#include <iostream>
#include <string>

using namespace std;

class A {
public:
  A() { cout << "A create" << endl; }
  virtual ~A() { cout << "A delete" << endl; }
  void show() { cout << "A show" << endl; }
};

class B : public A {
public:
  B() { cout << "B create" << endl; }
  ~B() { cout << "B delete" << endl; }

  void show() { cout << "B show" << endl; }
};

int main(int argc, char const *argv[]) {
  A * newB = new B();
  newB->show();
  delete newB;

  return 0;
}
```

output:
```
A create
B create
A show
B delete
A delete
```

请记住：
* polymorphic(带多态性质的) base classes 应该声明一个 virtual 析构函数。如果 class 带有任何 virtual 函数，他就应该拥有一个 virtual 析构函数。
* classes 的设计目的如果不是作为 base classes 使用，或不是为了具备多态性(polymorphic),就不应该声明 virtual 析构函数。

### 条款 08：别让异常逃离析构函数
c++ 并不禁止析构函数吐出异常，但它不鼓励你这样做。这是由理由的，考虑以下代码：
```c++
class Widget {
public:
	 ~Widget (){...} // 假设这个可能吐出一个异常
};

void doSomething()
{
	std::vector<Widget> v;
	...
}
```

当 vector v 被销毁，它有责任销毁其内含的所有 Widgets. 假设 v 内含10个 Widget,而在析构第一个元素期间，有个异常被抛出。其他 9 个 Widget 还是应该被销毁，因此 v 应该调用它们各个析构函数。

若析构函数吐出异常，程序可能过早结束或出现不明确行为。是的，c++ 不喜欢析构函数吐出异常！

假设你使用一个 class 负责数据库连接：
```c++
class DBConnection{
public:
	...
	static DBConnection create();

	void close(); //关闭联机；失败则抛出异常
};
```
为确保客户不忘记在 DBConnection 对象上调用 close(),一个合理的想法是创建一个用来管理 DBConnection 资源的 class,并在其析构函数中调用 close.
```c++
class DBConn{
public:
	...
	~DBConn()
	{
		db.close();
	}
private:
	DBConnection db;
};
```
这便允许客户写出这样的代码：
```c++
{
	DBConn dbc(DBConnection::create());

	...
}
```
只要调用 close 成功，一切都美好。但如果该调用导致异常, DBConn 析构函数会传播该异常，也就是允许它离开这个析构函数。那会造成问题，因为那就是抛出了难以驾驭的麻烦。
两个办法可以避免这一问题。DBConn 的析构函数可以：
1. 如果 close 抛出异常就结束程序。通常通过调用 abort 完成。
```c++
DBConn::~DBConn()
{
	try
	{
		db.close();
	}
	catch(...)
	{
		writeErrInfo2Log(); //制作运转记录，记下对 close 的调用失败；
		std::abort();
	}
}
```
如果程序遭遇一个“于析构期间发生的错误”后无法继续执行，“强迫结束程序”是个合理选项。毕竟它可以阻止异常从析构函数传播出去（那会导致不明确的行为）。也就是说调用 abort 可以抢先制“不明确行为”于死地。

2. 吞下因调用 close 而发生的异常：
```c++
DBConn::~DBConn()
{
	try
	{
		db.close();
	}
	catch(...)
	{
		writeErrInfo2Log(); //制作运转记录，记下对 close 的调用失败；
	}
}
```
一般而言，将异常吞掉是个坏主意，因为它压制了“某些动作失败”的重要信息！然而有时候吞下异常也比负担“草率结束程序”或“不明确行为带来的风险”好。为了让这成为一个可行方案，程序必须能够继续可靠地执行，即使在遭遇并忽略一个错误之后。

上述的办法没有什么吸引力。问题在于两者都无法对“导致 close 抛出异常”的情况做出反应。

一个较佳策略是重新设计 DBConn 接口，使其客户有机会对可能出现的问题做出反应。就像这样：
```c++
class DBConn
{
public:
	...
	void close() // 供客户使用的新函数
	{
		db.close();
		closed=true;
	}

	~DBConn()
	{
		if(!closed)
		{
			try
			{
				db.close();
			}
			catch(...)
			{
				writeErrInfo2Log(); //制作运转记录，记下对 close 的调用失败；
				...
			}
		}
	}

private:
	DBConnection db;
	bool closed;
};
```
把调用 close 的责任从 DBConn 析构函数手上移到 DBConn 客户手上。如果真有错误发生--如果 close 的确抛出异常-- 而且 DBConn 吞下该异常或结束程序，客户没有立场抱怨，毕竟它们曾有机会第一手处理问题，而他们选择了放弃。

请记住：
* 析构函数绝不要吐出异常。如果一个被析构函数调用的函数可能抛出异常，析构函数应该捕捉任何异常，然后吞下它们（不传播）或结束程序；
* 如果客户需要对某个操作函数运行期间抛出的异常做出反应，那么 class 应该提供一个普通函数（而非在析构函数中）执行该操作。

### 条款 09：绝不在构造和析构过程中调用 virtual 函数
本条款开始之前先阐述重点：你不应该在构造函数和析构函数期间调用 virtual 函数，因为这样的调用不会带来你预想的结果，就算有你也不会高兴。如果你同时也是一位 java 或 c# 程序uan，请更加注意本条款，因为这是 C++ 与它们不相同的一个地方。

请看下面的一个例子：
```c++
#include <iostream>

class Transaction{
public:
	Transaction();
	virtual void logTransaction() const =0;
};

Transaction::Transaction()
{
	logTransaction();
}

class BuyTransaction:public Transaction{
public:
	virtual void logTransaction() const
  {}
};

class SellTransaction:public Transaction
{
public:
	virtual void logTransaction() const
  {}
};

int main(int argc, char const *argv[]) {
  BuyTransaction test;
  return 0;
}

```

生成执行文件:
```sh
# 使用g++ 8.2.1 ，生成执行文件失败；
# 编译出现警告
... : In constructor ‘Transaction::Transaction()’:
... :warning: pure virtual ‘virtual void Transaction::logTransaction() const’ called from constru
ctor
  logTransaction();
                 ^
# 链接错误
/usr/bin/ld: /tmp/ccE9rBeB.o: in function `Transaction::Transaction()`:
... :(.text+0x20): undefined reference to `Transaction::logTransaction() const \`
collect2: error: ld returned 1 exit status
```

base class 构造期间 virtual 函数绝不会下降到 derived classes 阶层。取而代之的是，对象的作为就像隶属 base 类型一样。非正式的说法或许比较传神：在 base class 构造期间，virtual 函数不是 virtual 函数。

这一似乎反直觉的行为有个好理由。由于 base class 构造函数的执行更早于 derived class 构造函数，当 base class 构造函数执行时 derived class 的成员变量尚未初始化。如果此期间调用 virtual 函数下降至 derived classes 阶层，要知道 derived class 的函数几乎必然取用 local 成员变量，而这些成员变量尚未初始化。这将是一张通往不明确行为和彻夜调试大会串的直达车票。“要求使用对象内部尚未初始化的成分”是危险的代名词，所以 c++ 不会让你走这条路。

对象在 derived class 构造函数开始执行之前不会成为一个 derived class 对象。

相同道理也适用于析构函数。一旦 derived class 析构函数开始执行，对象内的 derived class成员变量便呈现未定义值，所以 c++ 视他们仿佛不再存在。进入 base class 析构函数后对象就成为了一个 base class 对象，而 c++ 的任何部分包括 virtual 函数、 dynamic_casts 等等也就这么看待它。

上述示例中的问题，某些编译器会为此发出一些警告信息。

再看下面的例子：
```c++
#include <iostream>

class Transaction{
public:
	Transaction();
	virtual void logTransaction() const =0;
private:
  void init()
  {
    logTransaction(); //这里调用 virtual
  }
};

Transaction::Transaction()
{
	init(); // 调用 non-virtual
}

class BuyTransaction:public Transaction{
public:
	virtual void logTransaction() const
  {}
};

class SellTransaction:public Transaction
{
public:
	virtual void logTransaction() const
  {}
};

int main(int argc, char const *argv[]) {
  BuyTransaction test;
  return 0;
}
```
使用 g++ 生成执行文件，并不会有警告及错误信息；但是运行
```sh
pure virtual method called
terminate called without an active exception
Aborted (core dumped)
```

这段代码概念上和稍早版本相同，但它比较潜藏并且暗中为害，因为它通常不会引发编译器和连接器的抱怨。此时由于 logTransaction 是 Transaction 内的一个 pure virtual 函数，当 pure virtual 函数被调用，大多执行系统会中止程序（通常对此结果发出一个信息）。

很显然，在构造函数里面调用 virtual 函数是一个错误的错法。

其他方案可以解决这个问题。像这样：
```c++
class Transaction
{
	public:
	explicit Transaction(const std::string& logInfo);
	void logTransaction(const std::string& logInfo) const ; // non-virtual function
	...
};

Transaction::Transaction(const std::string& logInfo)
{
	...
	logTransaction(logInfo); // non-virtual 调用
}

class BuyTransaction:public Transaction
{
	public：
	BuyTransaction(params)
	:Transaction(createLogString(params))
	{
		...
	}
	...
	private:
	static std::string createLogString(params);
}
```

### 条款 10：令 operator= 返回一个 reference to * this
关于赋值，有趣的是你可以把它们写成连锁形式：
```c++
int x,y,z;
x=y=z=15; //赋值连锁形式
```
同样有趣的是，赋值采用右结合律，所以上述连锁赋值被解析为：
```c++
x=(y=(z=15));
```
为了实现“连锁赋值”，赋值操作符必须返回一个 reference 指向操作符的左侧实参。这是你为 classes 实现赋值操作符时应该遵循的协议：
```c++
class Widgets{
public:
	...

	Widget& operator=(const Widget &rhs)
	{
		...
		return * this;
	}
	...
};
```

这个协议不仅适用于以上标准赋值形式，也适用于所有赋值相关运算，例如：
```c++
class Widget
{
public:
	...
	Widget& operator+=(const Widget &rhs)
	{
		...
		return * this;
	}

	Widget& operator=(int rhs)
	{
		...
		return * this;
	}
	...
};
```
注意，这只是个协议，并无强制性。如果不遵循它，代码一样可以通过编译。然而这份协议被 STL 提供的诸如 string,vector,complex,tr1::shared_ptr 等类型共同遵守。因此除非你有一个非常标新立异的好理由，不然还是随众吧。

### 条款 11：在 operator= 中处理“自我赋值”
”自我赋值” 发生在对象被赋值给自己时：
```c++
class Widget{...};
Widget w;
...
w=w;
```
这看起来有点愚蠢，但他合法，所以不要认定客户决不会这么做。此外赋值操作并不总是那么可被一眼辨识出来，例如：
```c++
// 潜在的自我赋值
a[i]=a[j]; // if i==j
*px=*py; // 如果 px 和 py 恰巧指向同一个东西
```
```c++
class Base{...};
class Derived:public Base{...};
void doSomething(const Base& rb,Derived* pd);
// rb 和 *pd 有可能其实是同一对象
```

示例：
```c++
class Bitmap{...};
class Widget{
	...
private:
	Bitmap * pb; // pointer to object by new
};
```

让我们来看一份不安全的 operator= 实现版本：
```c++
Widget& Widget::operator=(const Widget& rhs)
{
	delete pb;
	pb=new Bitmap(* rhs.pb);
	return * this;
}
```
这里自我赋值的问题时，operator 函数内的 * this 和 rhs 有可能时同一个对象。果真如此 delete 就不只是销毁当前对象的 bitmap,它也销毁 rhs 的 bitmap。在函数末尾， Widget -- 它原本不该被自我赋值动作改变的--发现自己持有一个指针指向一个已被删除的对象！
阻止这种错误，可以这样：
```c++
Widget& Widget::operator=(const Widget& rhs)
{
	if (this == &rhs) {
		return * this;
	}
	delete pb;
	pb=new Bitmap(* rhs.pb);
	return * this;
}
```
但这个版本仍然存在异常方面的麻烦。更明确地说，如果“new BitMap”导致异常（不论时因为分配时内存不足或因为 Bitmap 的 copy 构造函数抛出异常），Widget 最终持有一个指针指向一块被删除的 bitmap。这样的指针有害。
你可以这样处理：
```c++
class Widget
{
	...
	void swap(Widget & rhs); //交换 * this 和 rhs 数据；详见条款 29
	...
};
```
```c++
Widget& Widget::operator=(const Widget &rhs)
{
	Widget tmp(rhs);
	swap(temp);
	return * this;
}
```
or
```c++
Widget& Widget::operator=(Widget rhs)
{
	swap(rhs);
	return * this;
}
```
注：作者比较忧虑第二种做法，认为它为了伶俐巧妙的修补而牺牲了清晰性，然而将"copy动作"从函数体内移至“函数参数构造阶段”却可令编译器有时生成更高效的代码。

请记住：
* 确保当对象自我赋值时 operator= 有良好的行为。其中技术包括比较“来源对象”和“目标对象”的地址、精心周到的语句顺序、以及 copy-and-swap;
* 确定任何函数如果操作一个以上的对象，而其中多个对象时同一个对象时，其行为仍然正确。

### 条款 12：复制对象时务忘其每一个成分

设计良好之面向对象系统会将对象的内部封装起来，只留两个函数负责对象拷贝（复制），那便是带着适切名称的 copy 构造函数和 copy assignment 操作符，作者称其为 copying 函数。

如果你声明自己的 copying 函数，便拒绝了编译器生成的缺省 copying 函数。编译器仿佛被冒犯似的，会以一种奇怪的方式回敬：当你的实现代码几乎必然出错却不告诉你。
考虑如下示例：
```c++
void logCall(const std::string& funcName);
class Customer{
public:
	...
	Customer (const Customer& rhs);
	Customer& operator=(const Customer& rhs);
	...
private:
	std::string name;
};

Customer::Customer(const Customer& rhs):name(rhs.name)
{
	logCall("Customer copy constructor");
}

Customer& Customer::operator=(const Customer& rhs)
{
	logCall("Customer copy assignment operator");
	name=rhs.name;
	return * this;
}
```
这里的每一件事情看起来都很好，而实际上每件事情也的确都好，直到另一个成员变量加入战局：
```c++
class Date {...};
class Customer
{
public:
	...
private:
	std::string name;
	Date lastTransaction; // other val
}
```

这时的既有 copying 函数执行的是局部拷贝:它们的确复制了name,但是没有复制新添加的 lastTransaction。大多数编译器对此不出任何怨言-即使在最高等警告级别中。

结论很明显：如果你为 calss 添加一个成员变量，你必须同时修改 copying 函数。（你也需要修改 class 的所有构造函数(见条款 4 和 45)以及任何非标准形式的 operator= 。如果你忘记，编译器不太可能提醒你）。

另外一旦发生继承，可能会造成此一主题最暗中肆虐的一个潜藏危机。请考虑：
```c++
class PriorityCustomer:public Customer
{
public:
	...
	PriorityCustomer(const PriorityCustomer& rhs);
	PriorityCustomer& operator=(const PriorityCustomer& rhs);
	...
private:
	int priority;
}
PriorityCustomer::PriorityCustomer(const PriorityCustomer& rhs):priority(rhs.priority)
{
	logCall("PriorityCustomer copy constructor");
}

PriorityCustomer& PriorityCustomer::operator=(const PriorityCustomer& rhs)
{
	logCall("PriorityCustomer copy assignment operator");
	priority=rhs.priority;
	return * this;
}
```
PriorityCustomer 的 copying 函数看起来好像复制了 PriorityCustomer 内的每一样东西，但是请再看一眼。是的，它们复制了 PriorityCustomer 声明的成员变量，但每个 PriorityCustomer 还内含它所继承的 Customer 成员变量副本，而那些成员变量却未被复制。

以上事态在 PriorityCustomer 的 copy assignment 操作符身上只是轻微不同。它不曾企图修改其 base class 的成员变量，所以那些成员变量保持不变。
任何时候只要你承担起"为 derived class 撰写 copying 函数"的重责大任，必须很小心地也复制其 base class 成分。那些成分往往是 private,所以你无法直接访问它们，你应该让 derived class 的 copying 函数调用相应的 base class 函数：
```c++
PriorityCustomer::PriorityCustomer(const PriorityCustomer& rhs):
	Customer(rhs), // 调用 base class copy 构造函数
	priority(rhs.priority)
{
	logCall("PriorityCustomer copy constructor");
}

PriorityCustomer& PriorityCustomer::operator=(const PriorityCustomer& rhs)
{
	logCall("PriorityCustomer copy assignment operator");
	Customer::operator=(rhs); // 对 base class 成分进行赋值操作
	priority=rhs.priority;
	return * this;
}
```

请记住：
* copying 函数应该确保复制“对象内的所有成员变量”及“所有 base class 成分”。
* 你不该令 copy assignment 操作符调用 copy 构造函数；同样不该令 copy 构造函数调用 copy assignment 操作符。
* 不要尝试以某个 copying 函数实现另一个 copying 函数。应该将共同机能放进第三个函数中，并由两个 copying 函数共同调用。

## 资源管理

所谓资源就是，一旦用了它，将来必须还给系统。如果不这样，糟糕的事情就会发生。C++ 程序最常使用的资源就是动态分配内存。其他常见的资源还包括 文件描述器(file descriptors)、 互斥锁(mutex locks) 、图形界面中的字型和笔刷、数据库连接、以及网络 sockets 。不论那一种资源，重要的是，当你不再使用它时，必须将它还给系统。

尝试在任何运用情况下都确保以上所言，是件困难的事，但当你考虑到异常、函数内多重回传路径、程序维护员改动软件却没能充分理解随之而来的冲击，态势就很明显了：资源管理的特殊手段还不很充分够用。

本章一开始是一个直接而易懂且基于对像(object-based) 的资源管理办法，建立在 c++ 对构造函数、析构函数、copying 函数的基础上。经验显示，经过训练后严守这些做法，可以几乎消除资源管理问题。

### 条款 13： 以对象管理资源

```c++
class Investment {...}; // “投资类型” 继承体系中的 root class
```
进一步假设，这个程序库系通过一个工厂函数供应我们某特定的 Investment 对象：
```c++
Investment * createInvestment();
// 返回指针，指向 Investment 集成体系内动态分配的对象。调用者有责任删除它。
// 这里为了简化，刻意不写参数。
```

```c++
void f()
{
	Investment * pInv = createInvestment();

	...

	delete pInv;
}
```
这看起来妥当，但若干情况下 f 可能无法删除它得自 createInvestment 的投资对象(...)。无论 delete 如何被略过去，我们泄漏的不只是内含投资对象的那块内存，还包括那些投资对象所保存的任何资源。

当然了，谨慎的编写程序可以防止这一类错误，但你必须想想，代码可能会在时间渐渐过去后被修改。单纯依赖“f总是会执行其 delete 语句”是行不通的。

为确保 createInvestment 返回的资源总是被释放，我们需要将资源放进对象内，当控制流离开 f ，该对象的析构函数会自动释放那些资源。实际上这正是隐身于本条款背后的半边想法：把资源放进对象内，我们便可依赖 c++ 的 “析构函数自动调用机制”确保资源被释放。（稍后讨论另半边想法。）

许多资源被动态分配于 heap 内而后被用于单一区块或者函数内。它们应该在控制流离开那个区块或函数时被释放。 [auto_ptr](https://en.cppreference.com/w/cpp/memory/auto_ptr) 正是针对这种形式而设计的特制产品。

eg:
```c++
void f()
{
	std::auto_ptr<Investment> pInv(createInvestment());

	...
}
```

这个简单的例子示范“以对象管理资源”的两个关键想法：
* 获得资源后立刻放进管理对象内。
* 管理对象运用析构函数确保资源被释放。

由于 auto_ptr 被销毁时会自动删除它所指之物，所以一定要注意别让多个 auto_ptr 同时指向同一对象。如果真是那样，对象会被删除一次以上，而那会使你的程序搭上驶向“未定义行为”的快速列车上。为了预防这个问题，auto_ptrs 有一个不寻常的特性：若通过 copy 构造函数或 copy assignment 操作符复制它们，它们就会变成 null ,而复制所得的指针将取得资源的唯一拥有权！

这一诡异的复制行为，副加上其底层条件： “受 auto_ptrs 管理的资源必须绝对没有一个以上的 auto_ptr 同时指向它”，意味着 auto_ptrs 并非管理动态分配资源的神兵利器。举个例子，STL 容器要求其元素发挥 “正常的”复制行为，因此这些容器容不得 auto_ptr。

auto_ptr 的替代方案是 "引用计数型智慧指针 (reference-counting smart pointer ,RCSP)"。所谓 RCSP 也是个智能指针，持续追踪共有多少对象指向某笔资源，并在无人指向它时自动删除该资源。 RCSPs 提供的行为类似垃圾回收，不同的是 RCSPs 无法打破环状引用，例如两个其是已经没被使用的对象彼此互指，因而好像还处在 “被使用” 状态。

[shared_ptr](https://en.cppreference.com/w/cpp/memory/shared_ptr) 就是个 RCSP.

```c++
void f()
{
	std::shared_ptr<Investment> pInv(createInvestment());
	...
}
```

这段代码看起来几乎和使用 auto_ptr 的那个版本相同，但 shared_ptr 的复制行为正常多了。

由于 shared_ptr 的复制行为 “一如预期”，它们可被用于 STL 容器以及其他 “auto_ptr 之非正统复制行为并不适用” 的语境上。

auto_ptr 和 shared_ptr 两者都在其析构函数内做 delete 而不是 delete[] 动作。那意味着在动态分配而得到的 array 身上使用 auto_ptr 或 shared_ptr 是个馊主意。尽管如此，可叹的是，那么做仍能够通过编译：
```c++
// 馊主意，oops
std::auto_ptr<std::string> aps(new std::string[10]);

std::shared_ptr<int> spi(new int[1024]);
```
如果你还是认为拥有针对数组而设计，类似 auto_ptr 和 shared_ptr 那样的 classes 较好，看看 boost 吧， [boost::scoped_array](https://www.boost.org/doc/libs/1_61_0/libs/smart_ptr/scoped_array.htm)
和 [boost::shared_array](https://www.boost.org/doc/libs/1_61_0/libs/smart_ptr/shared_array.htm)

本条款也建议：如果你打算手工释放资源，容易发生错误。罐装式的资源管理类如 auto_ptr 和 shared_ptr 往往是比较能够轻松遵循本条款忠告，但有时候你所使用的资源是目前这些预制式 classes 无法妥善管理的。既然如此就需要精巧制作你自己的资源管理类。那并不是非常困难，但的确设计若干你需要考虑的细节。

请记住：
* 为防止资源泄漏，请使用 RAII 对象，它们在构造函数中获得资源并在析构函数中释放资源。
* 两个常被使用的 RAII classes 分别是 shared_ptr 和 auto_ptr(在 c++ 11 中被标记为弃用)。前者通常是较佳的选择，因为其 copy 行为比较直观。若选择 auto_ptr ，复制动作会使它（被复制物） 指向 null。

### 条款 14： 在资源管理类中小心 copying 行为

假如建立一个自己的资源管理类 Lock:
```c++
class Lock{
public:
	explicit Lock(Mutex* pm):mutexPtr(pm)
	{
		lock(mutexPtr);
	}
	~Lock()
	{
		unlock(mutexPtr);
	}
private:
	Mutex * mutexPtr；
};
```

客户对 Lock 的用法符合 RAII 方式：
```c++
Mutex m;
...

{
	Lock m1(&m);
	...
}
```
这很好，但如果 Lock 对象被复制，会发生什么事？

```c++
Lock ml1(&m); // 锁定 m
Lock ml2(ml1); // oops
```

这是一个一般化问题的特定例子，那个一般问题是每一位 RAII class 作者一定需要面对的： “当一个 RAII 对象被复制，会发生什么事？” 大多数你会选择以下两种可能：

* 禁止复制。如果复制动作对 RAII class 并不合理，你便应该禁止之。eg：
```c++
class Lock:private Uncopyable
{
	...
};
```
* 对底层资源祭出 “引用计数法”
比如这样：
```c++
class Lock{
public:
	// 以某个 Mutex 初始化 shared_ptr 并以 unlock 函数为删除器
	explicit Lock(Mutex * pm):mutexPtr(pm,unlock)
	{
		lock(mutexPtr.get());
	}
private:
	std::shared_ptr<Mutex> mutexPtr;
};
```

* 复制底部资源（复制资源管理对象时，进行的是 “深度拷贝”。就像字符串深度拷贝那样）
* 转移底部资源的拥有权。某些罕见场合下你可能希望确保永远只有一个 RAII 对象指向一个未加工资源，即使 RAII 对象被复制依然如此。此时资源的拥有权会从被复制物转移到目标物。这是 auto_ptr 奉行的复制的意义。

请记住：
* 复制 RAII 对象必须一并复制它所管理的资源，所以资源的 copying 行为决定 RAII 对象的 copying 行为。
* 普遍而常见的 RAII class copying 行为是：抑制 copying、施行引用计数法。不过其他行为也可能被实现。

### 条款 15 ：在资源管理类中提供对原始资源的访问

```c++
std::shared_ptr<Investment> pInv(createInvestment());
```
假设你希望以某个函数处理 Investment 对象，像这样：
```c++
int daysHeld(const Investment* p); //返回投资天数
```

这时候你需要一个函数可以将 RAII class 对象转换为其所内含之原始资源。有两个做法可以达成目标：
* 显式转换
* 隐式转换

std::shared_ptr 和 auto_ptr 都提供一个 get 成员函数，用来执行显示转换，也就是它返回智能指针内部的原始指针：
```c++
int days = daysHeld(pInv.get());
```
就像（几乎）所有智能指针一样，shared_ptr 和 auto_ptr 也重载了指针取值(pointer defeferencing) 操作符 (operator-> 和 operator*) ,它们允许隐式转换至底部原始指针。

由于有时候还是必须取得 RAII 对象内的原始资源，某些 RAII class 设计者于是联想到 "将油脂涂在滑轨上"，做法是提供一个隐式转换函数。考虑下面：
```c++
FontHandle getFont(); // 这是个 c API

void releaseFont(FontHandle fh);

class Font{
public:
  explicit Font(FontHandle fh):f(fh)
  {

  }
  ~Font()
  {
    releaseFont(f);
  }

private:
  FontHandle f; // 原始字体资源
};
```

假设有大量的与字体相关的 c API ，它们处理的是 FontHandle,那么 “将 Font 对象转换为 FontHandle”会是一种很频繁的需求。Font class 可为此提供一个显式转换函数：
```c++
class Font{
public:
  ...
  FontHandle get() const
  {
    return f;
  }

  ...
};
```

不幸的是这使得客户每当想要使用 API 时就必须调用 get:
```c++
void changeFontSize(FontHandle f,int newSize); // C API

Font f(getFont());
int newFontSize =0;
...
changeFontSize(f.get(),newFontSize);
```

某些程序员可能会认为，如此这般地到处要求显式转换，足以使人们倒尽胃口，不再愿意使用这个 class ，从而增加了泄漏字体地可能性，而 Font class 的主要设计目的就是为了防止资源泄漏。

另一种方法是令 Font 提供隐式转换函数，转型为 FontHandle:
```c++
class Font{
public:
  ...
  operator FontHandle() const //  隐式转换函数
  {
    return f;
  }
};
```
这使得客户调用 C API 时比较轻松且自然：
```c++
Font f(getFont());
int newFontSize =0;
...
changeFontSize(f,newFontSize);
```

但这个隐式转换会增加错误发生机会。假如各户可能会在需要 Font 时意外创建一个 FontHandle:
```c++
Font f1(getFont());
...
FontHandle f2 = f1; // oops!原意是要拷贝一个 Font 对象
//，却反而将 f1 隐式转换为其底部的 FontHandle 然后再复制它。
```

这几乎不会有好下场。

是否该提供一个显式转换函数将 RAII class 隐式转换为其底部的资源，或是应该提供隐式转换，答案主要取决于 RAII class 被设计执行的特定工作，以及它被使用的情况。通常显式转换函数是比较受欢迎的路子，因为它将“非故意之类型转换”的可能性最小化了。然而有时候，隐式转换所带来的“自然用法”也会引发天平倾斜。

请记住：
* APIs 往往要求访问原始资源，所以每一个 RAII class 应该提供一个 “取得其所管理之资源” 的办法。
* 对原始资源的访问可能经由显式转换或隐式转换。一般而言显式转换比较安全，但隐式转换对客户比较方便。

### 条款 16： 成对使用 new 和 delete 时要采取相同形式

以下动作有什么错？
```c++
std::string * stringArray = new std::string[100];

...

delete stringArray;
```

delete 的最大问题在于：即将被删除的内存之内究竟存有多少个对象？

实际上这个问题可以更简单些：即将被删除的那个指针，所指的是单一对象或者对象数组？这是个比不可缺的问题，因为单一对象的内存布局一般而言不同于数组的内存布局。

```c++
std::string * sp1 = new std::string;
std::string * sp2 = new std::string[100];

...
delete sp1;
delete [] sp2;
```

游戏规则很简单：如果你调用 new 时使用 [],你必须在对应调用 delete 时也使用 []。如果你调用 new 时没有使用 [],那么也不该在对应调用 delete 时使用 [].

这个规则对于希望使用 typedef 的人也很重要，比如这个：
```c++
typedef std::string AddressLines[4];

std::string * pa1 = new AddressLines;
```

那就必须匹配“数组形式”的 delete:
```c++
delete pa1; // oops,行为未定义
delete [] pa1; // good
```
为避免诸如此类问题，最好尽量不要对数组形式做 typedef 动作。这很容易达成，因为 c++ STL 含有 string,vector等 template，可将数组的需求降至几乎为0。例如可以：
```c++
typedef std::vector<string> AddressLines;
```
请记住：
* 如果你调用 new 时使用 [],必须在对应调用 delete 时也使用 []。如果你调用 new 时没有使用 [],一定不要在相应的 delete 表达式中使用 []

### 条款 17：以独立语句将 newed 对象置入智能指针

```c++
int priority(); // 处理优先级
void processWidget(std::shared_ptr<Widget> pw,int priority); // 在动态分配所得的 Widget 上进行某些带优先级的处理
```
现在考虑调用 processWidget
```c++
processWidget(new Widget,priority()); // 等等，这不能通过编译。
```
如果写成这样就可以通过编译：
```c++
processWidget(std::shared_ptr<Widget>(new Widget),priority());
```
令人惊讶的是，虽然我们在此使用“对象管理式资源”，上述调用取可能泄漏资源。

编译器产出一个 processWidget 调用码之前，必须首先核算即将被传递的各个实参。上述第二个参数只是一个单纯的对 priority 函数的调用，但第一个实参 shared_ptr<Widget>(new Widget) 有两部分组成：
* 执行 "new Widget" 表达式
* 调用 shared_ptr 构造函数

于时在调用 processWidget 之前，编译器必须创建代码，做以下三件事：
* 调用 priority
* 执行 "new Widget" 表达式
* 调用 shared_ptr 构造函数

c++ 编译器以什么样的次序完成这些事情呢？弹性很大。这和其他语言如 Jave 和 c# 不同，那两种语言总是以特定次序完成函数参数的核算。可以确定的是 “ new Widget” 一定执行于 shared_ptr 构造函数被调用之前，因为这个表达式的结果还要传递作为 shared_ptr 构造函数的一个实参，但对 priority 的调用可以排在第一或第二或第三执行。如果编译器选择以第二顺位执行它（说不定可因此生成更高效的代码，谁知道！），最终获得这样的操作序列：
* 执行 "new Widget" 表达式
* 调用 priority
* 调用 shared_ptr 构造函数
现在请你想想，万一对 priority 调用导致异常，会发生什么事情？（oops，将会发生内存泄漏）

避免这类问题的方法很简单：
```c++
// 在单独语句内以智能指针存储 newed 所得对象。
std::shared_ptr<Widget> pw(new Widget);

// 这个调用绝不至于造成泄漏
processWidget(pw,priority());
```
以上代码之所以行得通，因为编译器对于 “跨越语句的各项操作” 没有重新排列的自由（只有在语句内它才拥有那个自由度）。

请记住：
* 以独立语句将 newed 对象存储于智能指针内。如果不这样做，一旦异常被抛出，有可能导致难以察觉的资源泄漏。




[上一级](README.md)
[上一篇](do_while_false.md)
[下一篇](function_arg_stack.md)
