# Effective c++ 改善程序与设计的55个具体做法


<!-- @import "[TOC]" {cmd="toc" depthFrom=1 depthTo=6 orderedList=false} -->
<!-- code_chunk_output -->

* [Effective c++ 改善程序与设计的55个具体做法](#effective-c-改善程序与设计的55个具体做法)
	* [条款 01：视 C++ 为一个语言联邦](#条款-01视-c-为一个语言联邦)
	* [条款 02：尽量以 const,enum,inline 替换 #define](#条款-02尽量以-constenuminline-替换-define)
	* [条款 03：尽可能使用 const](#条款-03尽可能使用-const)
		* [const 成员函数](#const-成员函数)
		* [在 const 和 non-const 成员函数中避免重复](#在-const-和-non-const-成员函数中避免重复)
	* [条款 04：确定对象被使用前已先被初始化](#条款-04确定对象被使用前已先被初始化)
		* [请以 local static 对象替换 non-local static 对象](#请以-local-static-对象替换-non-local-static-对象)

<!-- /code_chunk_output -->

## 条款 01：视 C++ 为一个语言联邦
为了理解 C++，你必须认识其主要的次语言。幸运的是总共只有四个：
* `C`：说到底 C++ 仍是以 C 为基础。
* `Object-Oriented C++` :这部分是面向对象设计之古典守则在 C++ 上的最直接实施。
* `Template C++`:模板
* `STL`

因此，C++ 并不是一个带有一组守则的一体语言；它是从四个次语言组成的联邦政府，每个次语言都有自己的规约。记住这四个次语言你就会发现 C++ 容易了解得多。

请注意：
* C++ 高效编程守则视状况而变化，取决于你使用 C++ 的那一部分。

## 条款 02：尽量以 const,enum,inline 替换 #define

宏只是一个简单的替换，你需要小心的使用它,并且使用宏不好调试

请记住：
* 对于单纯常量，最好以 `const` 对象或 `enum` 替换 `#define`
* 对于形似函数的宏(macros)（注：该方式不会招致函数调用带来的额外开销）,最好改用 `inline` 函数替换 `#define`

## 条款 03：尽可能使用 const
const 的一件奇妙的事情是，它允许你指定一个语义约束（也就是指定一个 “不该被改动”的对象），而编译器会强制实施这项约束。它允许你告诉编译器和其他程序员某值应该保持不变。只要这（某值保持不变）是事实，你就该明确说出来，因为说出来可以获得编译器的相助，确保这条约束不被违反。

关于 const 的使用可以参考之前写的文章 [const 限定符](const.md)

### const 成员函数
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

### 在 const 和 non-const 成员函数中避免重复

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

## 条款 04：确定对象被使用前已先被初始化

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
### 请以 local static 对象替换 non-local static 对象
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

[上一级](base.md)
[上一篇](do_while_false.md)
[下一篇](function_arg_stack.md)
