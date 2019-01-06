# Effective c++ 改善程序与设计的55个具体做法


<!-- @import "[TOC]" {cmd="toc" depthFrom=1 depthTo=6 orderedList=false} -->
<!-- code_chunk_output -->

* [Effective c++ 改善程序与设计的55个具体做法](#effective-c-改善程序与设计的55个具体做法)
	* [让自己习惯 c++](#让自己习惯-c)
		* [条款 01：视 C++ 为一个语言联邦](#条款-01视-c-为一个语言联邦)
		* [条款 02：尽量以 const,enum,inline 替换 #define](#条款-02尽量以-constenuminline-替换-define)
		* [条款 03：尽可能使用 const](#条款-03尽可能使用-const)
			* [const 成员函数](#const-成员函数)
			* [在 const 和 non-const 成员函数中避免重复](#在-const-和-non-const-成员函数中避免重复)
		* [条款 04：确定对象被使用前已先被初始化](#条款-04确定对象被使用前已先被初始化)
			* [请以 local static 对象替换 non-local static 对象](#请以-local-static-对象替换-non-local-static-对象)
	* [构造/析构/赋值运算](#构造析构赋值运算)
		* [条款 05： 了解 c++ 默默编写并调用的哪些函数](#条款-05-了解-c-默默编写并调用的哪些函数)
		* [条款 06： 若不想使用编译器自动生成的函数，就该明确拒绝](#条款-06-若不想使用编译器自动生成的函数就该明确拒绝)
		* [条款 07：为多态基类声明 virtual 析构函数](#条款-07为多态基类声明-virtual-析构函数)
		* [条款 08：别让异常逃离析构函数](#条款-08别让异常逃离析构函数)

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

[上一级](base.md)
[上一篇](do_while_false.md)
[下一篇](function_arg_stack.md)
