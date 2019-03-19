# 再探 c++ primer


<!-- @import "[TOC]" {cmd="toc" depthFrom=1 depthTo=6 orderedList=false} -->
<!-- code_chunk_output -->

* [再探 c++ primer](#再探-c-primer)
	* [变量和基本类型](#变量和基本类型)
		* [基本内置类型](#基本内置类型)
		* [列表初始化](#列表初始化)
		* [默认初始化](#默认初始化)
		* [变量声明和定义的关系](#变量声明和定义的关系)
		* [引用](#引用)
		* [指针](#指针)
		* [const限定符](#const限定符)
			* [顶层 const](#顶层-const)
			* [constexpr 和常量表达式](#constexpr-和常量表达式)
	* [字符串、向量、和数组](#字符串-向量-和数组)
		* [标准库类型 vector](#标准库类型-vector)
			* [值初始化](#值初始化)
		* [数组](#数组)
			* [定义和初始化内置数组](#定义和初始化内置数组)
		* [访问数组元素](#访问数组元素)
			* [检查下标的值](#检查下标的值)
		* [指针和数组](#指针和数组)
			* [指针也是迭代器](#指针也是迭代器)
			* [标准库函数 begin 和 end](#标准库函数-begin-和-end)
			* [指针运算](#指针运算)
		* [与旧代码的接口](#与旧代码的接口)
			* [混用 std::string 对象和 c 风格字符串](#混用-stdstring-对象和-c-风格字符串)
			* [使用数组初始化 vector 对象](#使用数组初始化-vector-对象)
		* [多维数组](#多维数组)
			* [多维数组初始化](#多维数组初始化)
			* [使用范围 for 语句处理多维数组](#使用范围-for-语句处理多维数组)
			* [类型别名简化多维数组的指针](#类型别名简化多维数组的指针)
	* [表达式](#表达式)
		* [基础](#基础)
			* [基本概念](#基本概念)
				* [左值和右值](#左值和右值)
		* [算术运算符](#算术运算符)
			* [警惕溢出和其他算术运算异常](#警惕溢出和其他算术运算异常)
		* [逻辑和关系运算符](#逻辑和关系运算符)
		* [赋值运算符](#赋值运算符)
			* [赋值运算符优先级较低](#赋值运算符优先级较低)
			* [复合赋值运算符](#复合赋值运算符)
		* [递增和递减运算符](#递增和递减运算符)
		* [逗号运算符](#逗号运算符)
		* [类型转换](#类型转换)
			* [其他隐式类型](#其他隐式类型)
			* [显式转换](#显式转换)
				* [旧式的强制类型转换](#旧式的强制类型转换)
		* [C++ 运算符优先级表](#c-运算符优先级表)
		* [注意](#注意)
	* [语句](#语句)
		* [范围 for 语句](#范围-for-语句)
		* [标准异常](#标准异常)
	* [函数](#函数)
		* [函数基础](#函数基础)
			* [形参和实参](#形参和实参)
		* [参数传递](#参数传递)
			* [含有可变形参的函数](#含有可变形参的函数)
		* [返回类型和 return 语句](#返回类型和-return-语句)
			* [不要返回局部对象的引用或指针](#不要返回局部对象的引用或指针)
			* [返回数组指针](#返回数组指针)
		* [函数重载](#函数重载)
			* [重载和 const 形参](#重载和-const-形参)
			* [重载与作用域](#重载与作用域)
		* [特殊用途语言特性](#特殊用途语言特性)
			* [默认实参](#默认实参)
			* [内联函数和 constexpr 函数](#内联函数和-constexpr-函数)
				* [内联函数可避免函数调用的开销](#内联函数可避免函数调用的开销)
				* [constexpr函数](#constexpr函数)
				* [把内联函数和 constexpr函数放在头文件内](#把内联函数和-constexpr函数放在头文件内)
			* [调试帮助](#调试帮助)
				* [NDEBUG 预处理变量](#ndebug-预处理变量)
		* [函数匹配](#函数匹配)
			* [候选函数和可行函数](#候选函数和可行函数)
			* [实参类型转换](#实参类型转换)
				* [函数匹配和 const 实参](#函数匹配和-const-实参)
		* [函数指针](#函数指针)
	* [类](#类)
		* [定义抽象数据类型](#定义抽象数据类型)
			* [构造函数](#构造函数)
				* [委托构造函数](#委托构造函数)
				* [聚合类](#聚合类)
		* [访问与封装](#访问与封装)
			* [使用 class 或 struct 关键字](#使用-class-或-struct-关键字)
			* [友元](#友元)
			* [返回 * this 的成员函数](#返回-this-的成员函数)
				* [从 const 成员函数返回 * this](#从-const-成员函数返回-this)
			* [类的声明](#类的声明)
		* [类的作用域](#类的作用域)
		* [类的静态成员](#类的静态成员)
			* [静态成员能用于某些场景，而普通成员不能](#静态成员能用于某些场景而普通成员不能)
		* [小结](#小结)
	* [IO 类](#io-类)
		* [IO 类](#io-类-1)
			* [IO 对象无拷贝或赋值](#io-对象无拷贝或赋值)
			* [条件状态](#条件状态)
			* [管理输出缓冲](#管理输出缓冲)
		* [文件输入输出](#文件输入输出)
			* [文件模式](#文件模式)
		* [string 流](#string-流)
	* [顺序容器](#顺序容器)
		* [顺序容器概述](#顺序容器概述)
		* [容器库概览](#容器库概览)
			* [使用 assign (仅顺序容器)](#使用-assign-仅顺序容器)
			* [使用 swap](#使用-swap)
			* [关系运算符](#关系运算符)
		* [顺序容器操作](#顺序容器操作)
			* [使用 emplace 操作](#使用-emplace-操作)
			* [访问元素](#访问元素)
			* [删除元素](#删除元素)
			* [特殊的 forward_list 操作](#特殊的-forward_list-操作)
			* [改变容器大小](#改变容器大小)
			* [容器操作可能使迭代器失效](#容器操作可能使迭代器失效)
				* [编写改变容器的循环程序](#编写改变容器的循环程序)
				* [不要保存 end 返回的迭代器](#不要保存-end-返回的迭代器)
		* [vector 对象是如何增长的](#vector-对象是如何增长的)
		* [额外的 string 操作](#额外的-string-操作)
			* [构造函数的其他方法](#构造函数的其他方法)
				* [substr 操作](#substr-操作)
			* [改变 string 的其他方法](#改变-string-的其他方法)
			* [string 搜索操作](#string-搜索操作)
			* [compare 函数](#compare-函数)
			* [数值转换](#数值转换)
		* [容器适配器](#容器适配器)
			* [栈适配器](#栈适配器)
			* [deque](#deque)
			* [priority_queue](#priority_queue)
	* [关联容器](#关联容器)
		* [关联容器概述](#关联容器概述)
			* [关键字类型的要求](#关键字类型的要求)
			* [pair 类型](#pair-类型)
		* [关联容器操作](#关联容器操作)
			* [关联容器迭代器](#关联容器迭代器)
			* [添加元素](#添加元素)
			* [删除元素](#删除元素-1)
			* [map 的下标操作](#map-的下标操作)
			* [访问元素](#访问元素-1)
		* [无序容器](#无序容器)
	* [动态内存](#动态内存)
		* [动态内存与智能指针](#动态内存与智能指针)
			* [shared_ptr 类](#shared_ptr-类)
			* [直接管理内存](#直接管理内存)
			* [shared_ptr 和 new 结合使用](#shared_ptr-和-new-结合使用)
			* [智能指针和异常](#智能指针和异常)
			* [unique_ptr](#unique_ptr)
			* [weak_ptr](#weak_ptr)
		* [动态数组](#动态数组)
			* [new 和数组](#new-和数组)
			* [allocator 类](#allocator-类)
	* [Link](#link)

<!-- /code_chunk_output -->

## 变量和基本类型

### 基本内置类型
c++ 定义了一套包括算术类型([arithmetic type](https://en.cppreference.com/w/c/language/arithmetic_types))和 空类型(void) 在内的基本数据类型。
* 其中算术类型包含了字符、整型数、布尔值和浮点数。
* 空类型不对应具体的值，仅用于一些特殊的场合。

### 列表初始化

c++ 语言定义了初始化的好几种不同的形式，例如：
```c++
int units_sold=0;
int units_sold={0};
int units_sold{0};
int units_sold(0);
```

作为 c++ 11 新标准的一部分，用花括号来初始化变量（列表初始化）得到了全面应用，而在此之前，这种初始化的形式仅在某些受限的场合下才使用。现在，无论是初始化对象还是某些时候为对象赋新值，都可以使用这样一组由花括号括起来起来的初始值了。


```highLight
当用于内置类型的变量时，这种初始化形式有一个重要的特点：如果我们使用列表初始化且初始值存在丢失信息的风险，则编译器将报错。
```

```c++
long double ld=3.1415926536;
int a{ld},b={ld}; //错误：转换未执行，因为存在丢失信息的危险
int c(ld),d=ld; //正确：转换执行，且丢失了部分值
```

### 默认初始化
如果定义变量时没有指定初值，则变量被默认初始化（ default initialized),此时变量被赋予了 "默认值"。默认值到底是什么由变量类型决定，同时定义变量的位置也会对此有影响。

```highLight
如果是内置类型的变量未被显式初始化，它的值由定义的位置决定。
* 定义于任何函数体之外的变量被初始化为 0。
* 定义在函数体内部的内置类型变量将 不被初始化(uninitialized)。一个未被初始化的内置变量的值是未定义的。
```

每个类各自决定其初始化对象的方式。而且，是否允许不经初始化就定义对象也由类自己决定。如果类允许这种行为，他将决定对象的初始值到底是什么。

绝大多数类都支持无须显式初始化而定义对象，这样的类提供了一个合适的默认值。

### 变量声明和定义的关系
为了支持分离式编译， c++ 语言将声明和定义区分开来。声明使得名字为程序所知，一个文件如果想使用别处定义的名字则必须包含对这个名字的声明。而定义负责创建与名字关联的实体。

如果想声明一个变量而非定义它，就在变量名前添加关键字 extern ，而且不要显式地初始化变量：
```c++
extern int  i; //声明 i而非定义 i  
int j; //声明并定义 j
extern int k=5; //定义
```

变量能且只能被定义一次，但是可以被多次声明。


### 引用

```highLight
c++ 11 新增了一种引用：所谓的右值引用 (rvalue refefence),这种引用主要用于内置类。严格来说，当我们使用术语 "引用 (refefence)" 时，指的其实是“左值引用(lvalue refefence)”。
```

引用 (refefence) 为对象起了另外一个名字，引用类型引用 (refers to) 另外一种类型。

一般在初始化变量时，初始值会被拷贝到新建的对象中。然而定义引用时，程序把引用和它的初始值 ```绑定(bind)``` 在一起，而不是将初始值拷贝给引用。一旦初始化完成，引用将和他的初始值对象一直绑定在一起。因为无法令引用重新绑定到另外一个对象，因此引用必须初始化。

```c++
int val=1024;
int &refVal=val; //refVal 指向 val
int &refVal2; //error，引用必须被初始化
int &refVal3=10; //error，引用类型的初始值必须是一个对象

double val1=3.14;
int &refVal4=val1; //error,此处引用类型的初始值必须是 int 型对象
```

引用即别名，只是定义需要特别注意，后面使用就跟直接使用相应的对象是一样的。

### 指针
过去的程序还会用到一个名为 NULL 的预处理变量来给指针赋值，这个变量在头文件 cstdlib 中定义，他的值就是 0。

在新标准下，现在的 c++ 程序最好使用 nullptr,同时尽量避免使用 NULL 。

把 int 变量直接赋给指针是错误的操作，即使 int 变量的值恰好等于 0 也不行。

```c++
int zero=0;
int *ptr=zero; //error，不能把 int 变量直接赋给指针
```

```sh
#g++
error: invalid conversion from ‘int’ to ‘int*’ [-fpermissive]
 int *ptr=zero;
```

建议：
* 初始化所有指针
* 在试图使用一个指针之前代码可以首先检查它是否为空
* 释放指针所指向的对象后应将指针赋为 nullptr

给定指针 P，你能知道它是否指向了一个合法的对象吗？ (不能)
[给定一个指针，如何判断这个指针是否已经指向一个合法的对象？](https://www.zhihu.com/question/24331527)

### const限定符
[const限定符](const.md)

#### 顶层 const
对于 const 和指针的组合：
* 顶层 const (top-level const) 表示指针本身是个常量。
* 底层 const (low-level const) 表示指针所指的对象是一个常量。

#### constexpr 和常量表达式
常量表达式 ([const expresion](https://en.cppreference.com/w/cpp/language/constant_expression))是指不会改变并且在编译过程就能得到计算结果的表达式。
一个对象（或表达式）是不是常量表达式由它的数据类型和初始值共同决定的，例如：
```c++
const int maxFiles=20 ; // const expresion
const int limit=maxFiles + 1; //const expresion
int staffSize=27; //not const expresion
const int size=getSize(); //not const expresion
```

c++11 新标准规定，允许将变量声明为 constexpr 类型以便由编译器来验证变量的值是否是一个常量表达式。声明为 constexpr 的变量一定是一个常量，而且必须用常量表达式初始化.
```c++
constexpr int mf=20; //20 是常量表达式
constexpr int limit=mf+1; //const expresion
constexpr int size=size(); //只有当 size 是一个 constexpr 函数时才是一条正确的声明语句
```
```highLight
一般来说，如果你认定变量是一个常量表达式，那就把它声明为 constexpr 类型
```

* 字面值类型
	字面值类型：这些类型一般比较简单，值也显而易见、容易得到。比如，算术类型、引用和指针都属于字面值类型。自定义类、IO库、string类型则不属于字面值类型。

	尽管指针和引用都能定义成 constexpr,但他们的初始值却受到严格限制。一个 constexpr 指针的初始值必须时 nullptr 或者 0，或者是存储在某个固定地址中的对象。

* 指针和 constexpr
	必须明确一点，在 constexpr 声明中如果定义了一个指针，限定符 constexpr 仅对指针有效，与指针所指的对象无关：
	```c++
	const int * p=nullptr; //p 是一个指向整型常量的指针
	constexpr int * q=nullptr; //q 是一个指向整数的常量指针
	```

注意，下面代码是不合法的：
```c++
#include <iostream>

int main(int argc, char const *argv[]) {
  int null=0,* p=null;
  return 0;
}
```
Run it:
```sh
error: invalid conversion from ‘int’ to ‘int*’ [-fpermissive]
   int null=0,*p=null;
```

## 字符串、向量、和数组

### 标准库类型 vector
#### 值初始化
通常情况下，可以只提供 vector 对象容纳的元素数量而略去初始值。此时库会创建一个 值初始化(value-initialized) 元素初值，并把它赋给容器中的所有元素。这个初值由 vector 对象中元素的类型决定。

### 数组
#### 定义和初始化内置数组
数组是一种复合类型。数组的声明形如 a[d],其中 a 是数组的名字， d 是数组的维度。维度说明了数组中元素的个数，因此必须大于0。数组中元素的个数也属于数组类型的一部分，编译的时候应该是已知的。也就是说，维度必须是一个常量表达式：
```c++
const int arrSize=10; // 常量表达式
int arr[arrSize];
int arr1[10];
```

* 显式初始化数组元素
```c++
const unsigned size=3;
int a1[size]={0,1,2};
int a2[]={0,1,2};
int a3[5]={0,1,2}; //等价于 a3[]={0,1,2,0,0}
std::string a4[3]={"hi","bye"}; //等价于 a4[]={"hi","bye",""}
int a5[2]={0,1,2}; //error
```

* 字符数组的特殊性
当使用这种方式时，一定要注意字符串字面值的结尾处还有一个空字符，这个空字符也会像字符串的其他字符一样被拷贝到字符数组中去：
```c++
char a1[]={'c','+','+'}; // 没有空字符,维度为3
char a2[]={'c','+','+','\0'}; //含有显式的空字符
char a3[]="c++"; // 自动添加表示字符串结束的空字符
const char a4[6]="Daniel"; //error
```
* 不允许拷贝和赋值
```c++
int a[]={0,1,2};
int a2[]=a; //error
a2=a; //error
```
注意，一些编译器支持数组的赋值，这就是所谓的编译器扩展。但一般来说，最好避免使用非标准特性，因为含有非标准特性的程序很可能在其他编译器上无法正常工作。

* 理解复杂的数组声明
```c++
int * ptrs[10]; //ptrs 是含有10个整型指针的数组
int &ref[10]=/* ?*/; //error ，不存在引用的数组
int (* parray)[10]=&arr; // parray 指向一个含有 10 个整数的数组
int (&arrRef)[10]=arr; //arrRef 引用一个含有 10 个整数的数组
```
默认情况下，类型修饰符从右到左依次绑定。对于 ptrs 来说，从右向左理解其含义比较简单：首先知道我们定义的是一个大小为 10 的数组，它的名字是 ptrs,然后知道数组中存放的是指向 int 的指针。

但是对于 parray 来说，从右向左理解就不太合理了。因为数组的维度是紧跟着被声明的名字，所以就数组而言，由内向外阅读要比从右向左好多了。
* 首先是圆括号括起来的部分，* parray 意味着 parray 是个指针
* 接下来观察右边，可知道 parray 是个指向大小为 10 的数组的指针
* 最后观察左边，知道数组中的元素是 int。
这样最终的含义就明白无误了，parray 是一个指针，它指向一个 int 数组，数组中包含 10 个元素。同理，(&arrRef) 表示 arrRef 是一个引用，它引用的对象是一个大小为 10 的数组，数组中元素的类型是 int。
```c++
int *(&arry)[10]; //arry 是数组的引用，该数组含有 10 个指针
```
* Tips：
要想理解数组声明的含义，最好的办法是从数组的名字开始按照由内向外的顺序阅读。

sample:
```c++
#include <iostream>
#include <string>

std::string arr1[10];
int arr2[10];

int main(int argc, char const *argv[]) {
  std::string arr3[10];
  int arr4[10];

  std::cout << "arr1: ";
  for(auto i:arr1)
  {
    std::cout << i<<",";
  }
  std::cout << '\n';

  std::cout << "arr2: ";
  for(auto i:arr2)
  {
    std::cout << i<<",";
  }
  std::cout << '\n';

  std::cout << "arr3: ";
  for(auto i:arr3)
  {
    std::cout << i<<",";
  }
  std::cout << '\n';

  std::cout << "arr4: ";
  for(auto i:arr4)
  {
    std::cout << i<<",";
  }
  std::cout << '\n';
  return 0;
}
```

Run it:
```sh
arr1: ,,,,,,,,,,
arr2: 0,0,0,0,0,0,0,0,0,0,
arr3: ,,,,,,,,,,
arr4: -678078112,32527,-678074624,32527,-678079712,32527,1635824384,1054802016,-678078496,32527,
```

### 访问数组元素
#### 检查下标的值
数组的下标是否在合理范围之内由程序员负责检查，所谓合理就是说下标应该 >=0 and < arr.size();

* 大多数常见的安全问题都源于缓冲区溢出错误。当数组或其他类似数据结构的下标越界并试图访问非法内存区域时，就会产生此类错误。

### 指针和数组
#### 指针也是迭代器
你可以这样：
```c++
int arr[10]={0,1,2,3,4,5,6,7,8,9};
int * end=&arr[10];

for(int * b=arr;b!=end;++b)
{
	std::cout<<* b<<std::endl;
}
```

#### 标准库函数 begin 和 end
尽管能计算得到尾后指针，但这种用法极易出错。为了让指针的使用更简单、更安全，c++ 11 新标准引入了两个名为 [std::begin(std::valarray)](https://en.cppreference.com/w/cpp/iterator/begin) 和 [std::end(std::valarray)](https://en.cppreference.com/w/cpp/iterator/end) 的函数。
就像这样：
```c++
int arr[]={0,1,2,3,4,5};
int * begin=std::begin(arr);
int * end=std::end(arr);
```

#### 指针运算
将两个指针相加不但是非法的，而且也没什么意义。
```c++
int arr[5]={0,1,2,3,4};
auto n=std::end(arr)-std::begin(arr); //n is 5
```
两个指针相减的结果的类型是一种名为 [ptrdiff_t](https://en.cppreference.com/w/cpp/types/ptrdiff_t) 的标准库类型，ptrdiff_t 是一种带符号类型。

* 练习 3.34:假定 p1 和 p2 指向同一个数组中的元素，则下面程序的功能是什么？什么情况下该程序是非法的？
```c++
p1+=p2-p1;
```
答：功能是将 p1 指向的位置变更到 p2 指向的位置；该程序非法的情况见下程序：
```c++
#include <iostream>
int main(int argc, char const *argv[]) {
  int arr[3]={0};
  int * const p1=arr;
  int * p2=arr+1;
  p1+=p2-p1;
  return 0;
}
```
Compile it:
```sh
arr3_34.cpp: In function ‘int main(int, const char**)’:
arr3_34.cpp:7:10: error: assignment of read-only variable ‘p1’
   p1+=p2-p1;
```

### 与旧代码的接口
#### 混用 std::string 对象和 c 风格字符串
std::string 专门提供了一个名为 c_str 的成员函数：
```c++
std::string s("Hello world");
char * str=s; //error
const char *str=s.c_str();
```
不过请注意：
```c++
#include <string>
#include <iostream>

int main(int argc, char const *argv[]) {
  const char * str=nullptr;
  std::string s("test");
  str=s.c_str();
  std::cout << "str val :"<<str << '\n';
  s="test1";
  std::cout << "str val :"<<str << '\n';
  return 0;
}
```
Run it:
```sh
str val :test
str val :test1
```
#### 使用数组初始化 vector 对象
你可以这样：
```c++
int arr[]={0,1,2,3,4,5};
std::vector<int> v(std::begin(arr),std::end(arr));
```

### 多维数组
#### 多维数组初始化
```c++
int arr[3][4]={
	{0,1,2,3},
	{4,5,6,7},
	{8,9,10,11}
};
```
例如下面的初始化语句，形式上更为简洁，完成的功能和上面这段代码完全一样：
```c++
int arr[3][4]={0,1,2,3,4,5,6,7,8,9,10,11};
```

```c++
int (&row)[4]=arr[1]; //把 row 绑定到 arr 的第二个  4 元素数组上
```
#### 使用范围 for 语句处理多维数组
```c++
size_t cnt=0;
for(auto &row:arr)
	for (auto &col:row)
	{
		col=cnt;
		++cnt;
	}
```
看下面的这个例子：
```c++
for(auto &row:arr)
	for (auto col:row)
	{
	std::cout << col << '\n';
	}
```
这个循环中并没有任何写操作，可是我们还是将外层循环的控制变量声明成了引用类型，这是为了避免数组被自动转换为指针。假设采用下面形式：
```c++
// error，无法通过编译
for(auto row:arr)
	for (auto col:row)
```
程序将无法通过编译。这是因为，像之前一样第一个循环遍历 arr 的所有元素，注意这些元素实际上是大小为 4 的数组。因为 row 不是引用类型，所以编译器初始化row时会自动将这些数组形式的元素转换成指向该数组首元素的指针。这样得到的 row 的类型就是 int * ,显然内层的循环就不合法了，编译器将试图在一个 int * 内遍历，这显然和程序的初衷相去甚远。

* Note
要使用范围 for 语句处理多维数组，除了最内层的循环外，其他所有循环的控制变量都应该时引用类型。

#### 类型别名简化多维数组的指针
```c++
using int_array=int[4]; //新标准下类型别名的声明
typdef int int_array[4]; //等价的 typedef 声明
```

## 表达式

### 基础
#### 基本概念
c++ 定义了 一元运算符(unary operator)和 二元运算符(binary operator)。除此之外，还有一个作用于三个运算对象的三元运算符 `？：`。函数调用也是一种特殊的运算符，它对运算对象的数量没有限制。
* 作用于一个运算对象的运算符是一元运算符，如 `取地址(&) 解引用(*)`;
* 作用于两个运算对象的运算符是二元运算符，如 `相等运算符(==) 乘法运算符(*)`

当然一些符号既能作为一元运算符也能作为二元运算符。比如 `*`。一个符号到底是一元运算符还是二元运算符由它的上下文决定的。

##### 左值和右值
c++ 的表达式要不然是右值(rvalue)，要不然是左值(lvalue)。这两个名词是从c语言继承过来的，原本是为了帮助记忆；左值可以位于赋值语句的左侧，右值则不能。

在 C++ 中，两者的区别就没那么简单了。一个左值表达式的求值结果是一个对象或者一个函数，然而以常量对象为代表的某些左值实际上不能作为赋值语句的左侧运算对象。 此外，虽然某些表达式的求值结果是对象，但他们是右值而非左值。可以做一个简单的归纳：`当一个对象被用作右值的时候，用的是对象的值（内容）；当对象被用作左值的时候，用的是对象的身份（在内存中的位置）。`

不同的运算符对运算对象的要求各不相同，有的需要左值运算对象、有的需要右值运算对象；返回值也有差异，有的得到左值结果、有的得到右值结果。一个重要的原则是在需要右值的地方可以用左值来代替，但是不能把右值当作左值（也就是位置）使用。当一个左值被当作右值使用时，实际使用的是它的内容（值）。到目前为止，已经有几种我们熟悉的运算符是要用到左值的。
* 赋值运算符需要一个（非常量）左值为其左侧运算对象，得到的结果也仍然是一个左值。
* 取地址符作用于一个左值运算对象，返回一个指向该运算对象的指针，这个指针是一个右值。
* 内置解引用运算符、下标运算符、迭代器解引用运算符、string和 vector 的下标运算符的求值结果都是左值。
* 内置类型和迭代器的递增递减运算符作用于左值运算对象，其前置版本所得的结果也是左值。

使用关键字 decltype 的时候，左值和右值也有所不同。如果表达式的求值结果是左值， decltype 作用于该表达式得到一个引用类型。
```c++
int * p;
decltype(* p); //因为解引用运算符生成左值，所以结果为 int &
decltype(& p); //因为取地址运算符生成右值，所以结果为 int **
```

### 算术运算符
算术运算符（左结合律）。
运算符 `%`，参与取余运算的运算对象必须是整数类型；

#### 警惕溢出和其他算术运算异常
```c++
#include <iostream>
#include <limits>

int main(int argc, char const *argv[]) {
  short val=std::numeric_limits<short>::max();
  std::cout << "val : "<<val << '\n';
  ++val;
  std::cout << "val : "<<val << '\n';
  ++val;
  std::cout << "val : "<<val << '\n';
  return 0;
}
```
Run it:
```sh
val : 32767
val : -32768
val : -32767
```

### 逻辑和关系运算符
逻辑与和逻辑或运算符，左结合律；
逻辑非，右结合律；
关系运算符，左结合律；

### 赋值运算符
赋值运算符满足右结合律，这一点和其他二元运算符不太一样。
#### 赋值运算符优先级较低
考虑如下代码：
```c++
int i=get_value();
while (i !=42) {
	...
}
```
但如果这样写，显然不是我们想要的：
```c++
//error
int i=0;
while (i = get_value() != 42) {
	...
}
```

* Note:
因为赋值运算符的优先级低于关系运算符的优先级，所以在条件语句中，赋值部分通常应该加上括号。

#### 复合赋值运算符
```c++
+= 	 -=    *=	/=  %= //算术运算符
<<=	 >>=   &=	^=  |= // 位运算符
```

### 递增和递减运算符
建议：
1. `除非必须，否则不用递增递减运算符的后置版本。`

	前置版本的递增运算符避免了不必要的工作，它把值加 1 后直接返回改变了的运算对象。与之相比，后置版本需要将原始值存储下来以便于返回这个未修改的内容。如果我们不需要修改前的值，那么后置版本的的操作就是一种浪费。
	对于整数和指针类型来说，编译器可能对这种额外的工作进行一定的优化；但是对于相对复杂的迭代器类型，这种额外的工作就消耗巨大了。建议养成使用前置版本的习惯，这样不仅不需要担心性能的问题，而且更重要的是写出的代码会更符合编程的初衷。

2. `简洁可以成为一种美德`

	比如这样：
	```c++
	std::cout << * iter << '\n';
	++iter;
	```
	可以写成这样，更为简洁，也更少出错
	```c++
	std::cout << * iter++ << '\n';
	```

### 逗号运算符
逗号运算符含有两个运算对象，按照从左向右的顺序依次求值。
让我们看下面的例子:
```c++
#include <iostream>

int main(int argc, char const *argv[]) {
  int x=3;
  int y=5;

  int val= (++x,++y);
  std::cout << "x: "<<x << '\n';
  int val1=(--x,--y);
  std::cout << "x: "<<x << '\n';

  std::cout << "val: "<<val << '\n';
  std::cout << "val1: "<<val1 << '\n';
  return 0;
}
```
Run it:
```sh
x: 4
x: 3
val: 6
val1: 5
```

### 类型转换
```c++
int val=3.541+3; // 编译器可能会警告该运算损失了精度
```
上述的类型转换是自动执行的，无须程序员的介入，有时甚至不需要程序员了解。因此，它们被称作隐式转换。

* 何时发生隐式转换
	在下面这些情况下，编译器会自动的转换运算对象的类型：
	* 在大多数表达式中，比 int 类型小的整型值首先提升为较大的整数类型。
	* 在条件中，非布尔值转换成布尔类型。
	* 初始化过程中，初始值转换成变量的类型；在赋值语句中，右侧运算对象转换成左侧运算对象的类型。
	* 如果算术运算或关系运算的运算对象有多种类型，需要转换成同一种类型。
	* 函数调用时也会发生类型转换。

#### 其他隐式类型
除了算术转换之外还有几种隐式类型转换：
* 数组转换成指针
* 指针的转换： nullptr/0 -> 任意指针类型；任意指针类型与 void * ，const void * 的转换。
* 转换成布尔类型
* 转换成常量
* 类类型定义的类型
```c++
std::string s,t="a value"; //字符串字面值转换成 string 类型
while(cin>>s) // while 的条件部分把 cin 转换成布尔值
```

#### 显式转换
有时我们希望显式地将对象强制转换成另外一种类型。例如，如果想在下面的代码中执行浮点数除法：
```c++
int i=0,j=0;
double slope=i/j;
```
就要使用某种方法将 i 和 j 显式地转换成 double，这种方法称为 强制类型转换(cast)。

* 命名的强制类型转换
	* [static_cast](https://en.cppreference.com/w/cpp/language/static_cast) 任何具有明确定义的类型转换，只要不包含底层 const ，都可以使用 static_cast。
	* [dynamic_cast](https://en.cppreference.com/w/cpp/language/dynamic_cast) ,支持运行时类型时别。
	* [const_cast](https://en.cppreference.com/w/cpp/language/const_cast) 只能改变运算对象的底层 const 。
	* [reinterpret_cast](https://en.cppreference.com/w/cpp/language/reinterpret_cast) 通常为运算对象的位模式提供较低层次上的重新解释。
		使用 reinterpret_cast 是非常危险的，比如这样
		```c++
		#include <iostream>
		#include <string>

		int main(int argc, char const *argv[]) {
			int * p=nullptr;
			char * p1=reinterpret_cast<char * >(p);

			std::string str(p1);
			std::cout << str << '\n';
			return 0;
		}
		```
		Run it:

		```sh
		terminate called after throwing an instance of \'std::logic_error\'
		what():  basic_string::_M_construct null not valid
		(core dumped)
		```
		reinterpret_cast 本质上依赖于机器。要想安全的使用 reinterpret_cast ，必须对涉及的类型和编译器实现转换的过程都非常了解。

建议：`避免强制类型转换`
强制类型转换干扰了正常的类型检查，因此我们强烈建议程序员避免使用强制类型转换。这个建议对于 reinterpret_cast 尤其适用，因为此类类型转换总是充满了风险。在有重载函数的上下文中使用 const_cast 无可厚非；但是在其他情况下使用 const_cast 也就意味着程序存在某种设计缺陷。其他强制类型转换，比如 static_cast 和 dynamic_cast,都不应该频繁使用。每次书写一次强制类型转换语句，都应该反复斟酌能否以其他方式实现相同目标。就算实在无法避免，也应该尽量限制类型转换值的作用域，并且记录对相关类型的所有假定，这样可以减少错误发生的机会 。

##### 旧式的强制类型转换
在早期版本中的 c++ 语言中，显式地进行强制类型转换包含两种形式：
```highLight
type (expr); // 函数形式的强制类型转换
(type) expr; // c 语言风格的强制类型转换
```
根据所涉及的类型不同，旧式的强制类型转换分别具有 const_cast、static_cast、reinterpret_cast 相似的行为。当我们在某处执行旧式的强制类型转换时，如果 const_cast 和 static_cast 也合法，则其行为与对应的命名转换一致。如果替换后不合法，则旧式强制类型转换执行 reinterpret_cast 类似的功能：
```c++
char * p2=(char * )p; //同 reinterpret_cast 效果一样
```
建议：使用命名的强制类型转换，不要用旧式的强制类型转换

### C++ 运算符优先级表
[C++ 运算符优先级表](https://zh.cppreference.com/w/cpp/language/operator_precedence)

### 注意
大多数运算符并不明确规定运算对象的求值顺序：编译器有权自由选择先对左侧运算对象求值还是先对右侧对象求值。一般来说，运算对象的求值顺序对表达式的最终结果没有影响。但是，如果两个运算对象指向同一个对象而且其中一个改变了对象的值，就会导致程序出现不易发现的严重缺陷。

## 语句

### 范围 for 语句
c++ 11 新标准引入了一种更简单的 for 语句，这种语句可以遍历容器或其他序列的所有元素。范围 for 语句 (range for statement) 的语法形式是：
```c++
for (declaration: expression)
	statement
```
比如这个：
```c++
std::vector<int> v{0,1,2,3,4,5};

for(auto &r:v)
{
	r*=2;
}
```
范围 for 语句的定义来源于与之等价的传统 for 语句：
```c++
for(auto beg=v.bein(),end=v.end();beg!=end();++beg)
{
	auto &r=* beg;
	r*=2;
}
```

### 标准异常
c++ STL 定义了一组类，用于报告标准库函数遇到的问题。这些异常类也可以在用户编写的程序中使用，他们分别定义在 4 个头文件中：
* exception 头文件定义了最通用的异常类 exception。它只报告异常的发生，不提供任何额外信息。
* stdexcept 头文件定义了几种常用的异常类，详细信息见下表
* new 头文件中定义了 bad_alloc 异常类型。
* type_info 头文件中定义了 bad_cast 异常类型。

\<stdexcept\> 定义的异常类

| exception        | 最常见的问题                                   |
|------------------|------------------------------------------------|
| runtime_error    | 只有在运行时才能检测的问题                     |
| range_error      | 运行时错误：生成的结果超出了有意义的值域范围   |
| overflow_errror  | 运行时错误：计算上溢                           |
| underflow_error  | 运算错误：计算下溢                             |
| logic_error      | 程序逻辑错误                                   |
| domain_error     | 逻辑错误：参数对应的结果值不存在               |
| invalid_argument | 逻辑错误：无效参数                             |
| length_error     | 逻辑错误：试图创建一个超出该类型最大长度的对象 |
| out_of_range     | 逻辑错误：使用一个超出有效范围的值             |

我们只能以默认初始化的方式初始化 exception bad_cast bad_alloc 对象，不允许为这些对象提供初始值。
其他异常类型的行为则恰好相反：应该使用 string 对象或者 C 风格字符串初始化这些类型的对象，但是不允许使用默认初始化的方式。当创建此类对象时，必须提供初始值，该初始值含有错误相关的信息。

## 函数

### 函数基础

#### 形参和实参
实参是形参的初时值。第一个实参初始化第一个形参，第二个实参初始化第二个形参，以此类推。尽管实参和形参存在对应关系，但是并没有规定实参的求值顺序。

### 参数传递
* 传递多维数组：
```c++
void print(int (*matrix)[10],int rowSize)
{}
//or
void print(int matrix[][10],int rowSize)
{}
```

#### 含有可变形参的函数
为了编写能处理不同数量实参的函数，c++ 11 新标准提供了两种主要的方法：
* 如果所有的实参类型相同，可以传递一个名为 [std::initializer_list](https://en.cppreference.com/w/cpp/utility/initializer_list)
* 如果实参类型不同，我们可以编写一种特殊的函数，也就是所谓的可变参数模板 [Parameter pack](https://en.cppreference.com/w/cpp/language/parameter_pack)

C++ 还有一种特殊的形参类型（即省略符），可以用它传递可变数量的实参。不过请注意，这种功能一般只用于与 c 函数交互的接口程序：
* `省略符形参应该仅仅用于 c 和 c++ 通用的类型。特别应该注意的是，大多数类类型的对象在传递给省略符形参时都无法正确拷贝。`

一个简单的 initializer_list 示例：
```c++
#include <iostream>
#include <initializer_list>
#include <string>

void errorMsg(int errorCode,std::initializer_list<std::string> par)
{
  std::cout << "errorCode:"<<errorCode<<", ";
  for(auto i:par)
  std::cout << i << " ";
  std::cout << '\n';
}

int main(int argc, char const *argv[]) {
  errorMsg(-1,{"functionX","is","error"});
  return 0;
}
```

### 返回类型和 return 语句

* 主函数 main 的返回值
如果函数的返回类型不是 void ,那么它必须返回一个值。但是这条规则有个例外：我们允许 main 函数没有 return 语句直接结束。如果控制到达了 main 函数的结尾而且没有 return 语句，编译器将隐式地插入一条返回 0 的 return 语句。（返回 0 表示执行成功，返回其他值表示执行失败，其中非 0 值的具体含义依机器而定）

#### 不要返回局部对象的引用或指针
```c++
//严重错误

const std::string &manip()
{
	std::string ret;
	if(!ret.empty())
	  return ret;
	else
	  return "Empty";
}
```

#### 返回数组指针
因为数组不能被拷贝，所以函数不能返回数组。不过，函数可以返回数组的指针或引用。

* 声明一个返回数组指针的函数
	举个例子：
	```c++
	int (* func(int i))[10];
	```

* 使用尾置返回类型
在 c++ 11 新标准中还有一种可以简化上述 func 声明的方法，就是使用尾置返回类型([trailing return type](https://arne-mertz.de/2016/11/trailing-return-types-everywhere/))
上面的函数，你可以这么写：
	```c++
	auto func(int i)->int( * )[10];
	```
* 使用 decltype
	eg:
	```c++
	int odd[]={1,3,5,7,9};
	int even[]={0,2,4,6,8};
	// 返回一个指针，该指针指向含有 5 个 整数的数组
	decltype(odd) *arrPtr(int i)
	{
		return (i%2) ? &odd : &even;
	}
	```

### 函数重载
如果同一作用域内的几个函数名字相同但形参列表不同，我们称之为重载([overloaded](https://www.geeksforgeeks.org/function-overloading-c/)) 函数。
eg:
```c++
#include <iostream>
using namespace std;

void print(int i) {
	cout << " Here is int " << i << endl;
}
void print(double f) {
	cout << " Here is float " << f << endl;
}
void print(char const *c) {
	cout << " Here is char* " << c << endl;
}

int main() {
print(10);
print(10.10);
print("ten");
return 0;
}
```
不过请注意：
```highLight
main 函数不能重载。
```

#### 重载和 const 形参
如 6.2.3 介绍的，顶层 const 不能影响传入函数的对象。一个拥有顶层 const 形参无法和另一个没有顶层 const 的形参区分开来：
```c++
Record lookup(Phone);
Record lookup(const Phone); //重复声明了 Record lookup(Phone);

Record lookup(Phone*);
Record lookup(Phone* const);//重复声明了 Record lookup(Phone*);
```

另一方面，如果形参时某种类型的指针或引用，则通过区分其指向的是常量对象还是非常量对象可以实现函数重载，此时 const 是底层：
```c++
Record lookup(Account &);
Record lookup(const Account&); //两个不同的函数

Record lookup(Account *);
Record lookup(const Account*); //两个不同的函数
```

#### 重载与作用域
```highLight
一般来说，将函数声明置于局部作用域内不是一个明智的选择。但是为了说明作用域和重载的相互关系，我们暂时违反这一原则而使用局部函数声明。
```
在不同的作用域中无法重载函数名：
```c++
std::string read();
void print(const std::string &);
void print(double);
void fooBar(int val)
{
	bool read=false; // 新作用域：隐藏了外层 read
	std::string s=read(); //错误： read 是一个布尔值，而非函数
	//不好的习惯：通常来说，在局部作用域中声明函数不是一个好的选择
	void print(int); //新作用域：隐藏了之前的 print
	print("Value: ");//错误，print(const std::string&)被隐藏掉了
	print(val); //正确，当前 作用域内的 print(int) 可见
	print(3.14); //正确，调用 作用域内的 print(int); 外面的print(double) 被隐藏掉了
}
```
```highLight
在 c++ 中，名字查找发生在类型检查之前
```
假设我们这样，将其放在同一作用域中，则它将成为另一种重载形式
:
```c++
void print(const std::string &);
void print(double);
void print(int);

void fooBar2(int val)
{
	print("Value ");
	print(val);
	print(3.14);
}
```

### 特殊用途语言特性
本节主要介绍三种函数相关的语言特性：
* 默认实参
* 内联函数和 constexpr函数
* 程序调试过程中常用的一些功能

#### 默认实参
[default argument](https://en.cppreference.com/w/cpp/language/default_arguments)

#### 内联函数和 constexpr 函数
在大多数机器上，一次函数调用其实包含着一系列工作：
* 调用前要先保存寄存器，并在返回时恢复;
* 可能需要拷贝实参；
* 程序转向一个新的位置继续执行；

##### 内联函数可避免函数调用的开销
将函数指定为 内联函数 （inline）,通常就是将它每个调用点上 “内联地”展开。
```c++
const std::string &shorterString(const std::string &s1,const std::string &s2)
{
	return s1.size() <= s2.size() ? s1:s2;
}
```
假设我们把 shorterString 函数定义成内联函数，
```c++
inline const std::string &shorterString(const std::string &s1,const std::string &s2)
{
	return s1.size() <= s2.size() ? s1:s2;
}
```
则如下调用
```c++
std::cout << shorterString(s1,s2) << '\n';
```
将在编译过程中展开成类似于下面的形式：
```c++
std::cout << (s1.size() <= s2.size() ? s1:s2) << '\n';
```
从而消除了 shorterString 函数的运行时开销。

Note:
* 内联说明只是向编译器发出一个请求，编译器可以选择忽略这个请求。

一般来说，内联机制用于优化规模较小、流程直接、频繁调用的函数。很多编译器都不支持内联递归函数，而且一个 75 行的函数也不大可能在调用点内联的展开。

##### constexpr函数
constexpr函数（constexpr function）是指用于常量表达式的函数。
定义 constexpr函数 与其他函数类似，不过要遵循几项约定：
* 函数的返回类型及所有形参的类型都得是字面值类型
* 函数体中必须有且只有一条 return 语句

为了能在编译过程中随时展开，constexpr函数被隐式地指定为内联函数。

constexpr函数体内也可以包含其他语句，只要这些语句在运行时不执行任何操作就行。例如，constexpr函数中可以由空语句、类型别名以及 using 声明。
```c++
constexpr size_t scale(size_t cnt)
{
	return 3*cnt;
}
```
当 scale 的实参是常量表达式时，它的返回值也是常量表达式；反之则不然：
```c++
int arr[scale(2)]; //正确
int i=2;
int a2[scale(i)]; //error，scale(i) 不是常量表达式
```

Note:
* constexpr函数不一定返回常量表达式。

##### 把内联函数和 constexpr函数放在头文件内
对于某个给定的内联函数或者 constexpr函数来说，它的多个定义必须完全一致。基于这个原因，内联函数和 constexpr 函数通常定义在头文件中。

#### 调试帮助

##### NDEBUG 预处理变量
assert 的行为依赖于一个名为 NDEBUG 的预处理变量。如果定义了 NDEBUG ,则 assert 什么也不做。默认状态下没有定义 NDEBUG,此时 assert 将执行运行时检查。

除了用于 assert 外，也可以使用 NDEBUG 编写自己的条件调试代码。
```c++
void print(const int arr[],size_t size)
{
	#ifndef NDEBUG
		std::cerr << __func__ <<": array size is "<<size<< '\n';
	#endif
}
```

预处理器还定义了另外 4 个对于程序调试很有用的名字：
* \_\_FILE\_\_ 存放文件名的字符串字面值
* \_\_LINE\_\_ 存放当前行号的整型字面值
* \_\_TIME\_\_ 存放文件编译时间的字符串字面值
* \_\_DATE\_\_ 存放文件编译日期的字符串字面值

### 函数匹配
eg：
```c++
void f();
void f(int);
void f(int,int);
void f(double,double=3.14);
f(5.6); //use f(double,double);
```

#### 候选函数和可行函数
函数匹配的第一步是选定本次调用对应的重载函数集，集合中的函数称为候选函数(candidate function)。候选函数具有的特征：
* 与被调用的函数同名
* 其声明在调用点可见

第二步考察本次调用提供的实参，然后从候选函数中选出能被这组实参调用的函数，这些新选出来的函数称为可行函数 (viable function)。可行函数的特征：
* 其形参数量与本次调用提供的实参数量相等
* 每个实参的类型与对应的形参类型相同，或者能够转换成形参的类型。

要注意：
```c++
f(42,2.56);
//error,
//对第一个实参 42 来说，f(int,int)是更好的匹配
//对第二个实参 2.56 来说，f(double,double)是更好的匹配
```
use gcc:
```sh
error: call of overloaded ‘f(int, double)’ is ambiguous
 f(42,2.56);
```
同理：
```c++
f(2.56,42); //error
```

编译器最终将因为这个调用具有二义性而拒绝其请求。

Note:
```highLight
调用重载函数时应尽量避免强制类型转换。如果在实际应用中确实需要强制类型转换，则说明我们设计的形参集合不合理。
```

Test code:
```c++
#include <iostream>

void f()
{
  std::cout << "f()" << '\n';
}

void f(int i)
{
  std::cout << "f(int)" << '\n';
}

void f(int i,int j)
{
  std::cout << "f(int,int)" << '\n';
}

void f(double i,double j)
{
  std::cout << "f(double,double)" << '\n';
}

int main(int argc, char const *argv[]) {
 //f(42,2.56);
 //f(2.56,42);
 f(42);
 f(42,0);
 f(2.56,3.14);
 return 0;
}
```

#### 实参类型转换
为了确定最佳匹配，编译器将实参类型到形参类型的转换划分为几个等级，具体排序如下所示：
1. 精确匹配：
	* 实参类型和形参类型相同
	* 实参从数组类型或函数类型转换成对应的指针类型
	* 向实参添加顶层 const 或者从实参中删除顶层 const
2. 通过 const 转换实现的匹配
3. 通过类型提升实现的匹配
4. 通过算术类型转换实现的匹配
5. 通过类类型转换实现的匹配

Warning:
```highLight
内置类型的提升和转换可能在函数匹配时产生意想不到的结果，但幸运的是，在设计良好的系统中函数很少会含有与下面例子类似的形参。
```
分析函数调用前，我们应该知道小整型一般会被提升到 int 类型或更大的整数类型。

有时候即使实参是一个很小的整数值，也会直接将它提升成 int 类型；比如
```c++
void ff(int);
void ff(short);
ff('a'); //char 提升成 int ；调用 f(int)
```
所有算术类型转换的级别都一样。例如，从 int 向 unsigned int 的转换并不比从 int 向 double 的转换级别高。比如
```c++
void manip(long);
void manip(float);
manip(3.14); //错误，二义性调用
```

##### 函数匹配和 const 实参
如果重载函数的区别在与它们的引用类型的形参是否引用了 const，或者指针类型的形参是否指向 const,则当调用发生时编译器通过实参是否是常量来决定选择哪个函数：
eg:
```c++
Record lookup(Account&);
Record lookup(const Account&);

const Account a;
Account b;

lookup(a); //use Record lookup(const Account&);
lookup(b); //use Record lookup(Account&);
```

### 函数指针
eg:
```c++
#include <iostream>
#include <string>

using namespace std;
bool lengthCompare(const string & str1,const string & str2)
{
  if (str1.size() == str2.size()) {
    return true;
  }
  else
    return false;
}

bool (* pf)(const string &,const string &);

int main(int argc, char const * argv[]) {
  pf=lengthCompare; //pf 指向 lengthCompare 函数,等价与 pf=&lengthCompare;取地址符是可选的
  bool sizeIsEqual=pf("hello","goodbye");
  std::cout << sizeIsEqual << '\n';
  return 0;
}
```

* 返回指向函数的指针
eg：
使用类型别名：
```c++
using F=int(int * ,int); //F 是函数类型，不是指针
using PF=int(* )(int * ,int); //PF 是指针类型

PF f1(int); //PF 是指向函数的指针，f1 返回指向函数的指针
F f1(int); //error,F 是函数类型
F *f1(int); //正确，显式地指定返回类型是指向函数的指针
```
当然，我们也可以这样：
```c++
int (* f1(int))（int *,int);
```
还可以这样：
```c++
auto f1(int)->int ( * )(int * ,int);
```

## 类
类的基本思想是数据抽象(data abstraction) 和 封装(encapsulation)。 数据抽象是一种依赖于接口(interface) 和 实现(implementation) 分离的编程(以及设计)技术。

封装实现了类的接口和实现的分离。封装后的类隐藏了它的实现细节。

类要想实现数据抽象和封装，需要首先定义一个抽象数据类型 (abstract data type，封装了实现细节的数据结构)。在抽象数据类型中，由类的设计者负责考虑类的实现过程；使用该类的程序员只需要抽象地思考类型做了什么，而无须了解类型的工作细节。

### 定义抽象数据类型

```highLight
定义在类内部的函数是隐式的 inline 函数。
```

```highLight
常量对象，以及常量对象的引用或指针都只能调用常量成员函数。
```

#### 构造函数
编译器创建的构造函数又被称为 合成的默认构造函数 (synthesized default constructor)。对于大多数类来说，这个合成的默认构造函数将按照如下规则初始化类的数据成员：
* 如果存在类内的初始值，用它来初始化成员。
* 否则，[默认初始化](#默认初始化) 该成员。

```highLight
只有当类没有声明任何构造函数时，编译器才会自动的生成默认构造函数。
```

```highLight
如果类包含有内置类型或者复合类型的成员，则只有当这些成员全都被赋予了类内的初始值时，这个类才适合于使用合成的默认构造函数。
```

* `=default` 的含义
我们定义这构造函数的目的仅仅是因为我们既需要其他形式的构造函数，也需要默认的构造函数。我们希望这个函数的作用完全等同于之前使用的合成默认构造函数。

[Default constructors](https://en.cppreference.com/w/cpp/language/default_constructor)

```highLight
如果你的编译器支持类内初始值,当我们提供一个类内初始值时，必须以符号 = 或者 花括号 表示。
```

```highLight
如果你的编译器不支持类内初始值，那么你的默认构造函数就应该使用构造函数初始值列表来初始化类的每个成员。
```

构造函数初始值列表([constructor initialized list](https://en.cppreference.com/w/cpp/language/initializer_list))

c++11 新标准规定，可以为数据成员提供一个类内初始值 ([in-class initializer](https://stackoverflow.com/questions/13662441/c11-allows-in-class-initialization-of-non-static-and-non-const-members-what-c)).

Sample:
```c++
#include <iostream>
#include <string>

using namespace std;

class test {
private:
 const std::string a="hello";
public:
  void print()
  {
    std::cout << a << '\n';
  }
};

int main(int argc, char const *argv[]) {
  test tmp;
  tmp.print();
  return 0;
}
```

Run it:
```sh
hello
```

```highLight
如果成员是 const、引用，或者属于某种未提供默认构造函数的类类型，我们必须通过构造函数初始值列表为这些成员提供初值。
```

```highLight
最好令构造函数初始值的顺序与成员声明的顺序保持一致。而且如果可能的话，尽量避免使用某些成员初始化其他成员。
```

##### 委托构造函数
c++ 11 新标准扩展了构造函数初始值的功能，使得我们可以定义所谓的 委托构造函数 ([delegating constructors](https://www.geeksforgeeks.org/constructor-delegation-c/))。一个委托构造函数使用它所属类的其他构造函数执行它自己的初始化过程，或者说它把它自己的一些（或者全部）职责委托给其他构造函数。

Sample:
```c++
// Program to demonstrate constructor delegation
// in C++
#include <iostream>
using namespace std;
class A {
	int x, y, z;

public:
	A()
	{
		x = 0;
		y = 0;
		z = 0;
	}

	// Constructor delegation
	A(int z) : A()
	{
		this->z = z; // Only update z
	}

	void show()
	{
		cout << x << '\n'
			<< y << '\n'
			<< z;
	}
};
int main()
{
	A obj(3);
	obj.show();
	return 0;
}
```
Run it:
```sh
0
0
3
```

* 对于 c++ 新手程序员来说有一种常犯的错误，它们试图以如下的形式声明一个用默认构造函数初始化的对象。
```c++
Sales_data obj(); // 错误，声明了一个函数而非对象
Sales_data obj2; //正确，obj2 是一个对象而非函数
```

* 抑制构造函数定义的隐式转换

	* 我们可以通过将构造函数声明为 [explicit](https://en.cppreference.com/w/cpp/language/explicit) 来阻止类类型的隐式转换。
	* explicit 构造函数只能用于直接初始化，不能将 explicit 构造函数用于拷贝形式的初始化过程

##### 聚合类
聚合类 ([aggregate class](http://www.enseignement.polytechnique.fr/informatique/INF478/docs/Cpp/en/cpp/language/aggregate_initialization.html))使得用户可以直接访问其成员，并且具有特殊的初始化语法形式。当一个类满足如下条件时，我们说它是聚合的：
* 所有成员都是 public
* 没有定义任何构造函数
* 没有类内初始值
* 没有基类，也没有 virtual 函数

Sample:
```c++
struct Data{
	int val;
	string s;
};
```
我们可以提供一个花括号括起来的成员初始值列表，并用它初始化聚合类的数据成员：
```c++
Data val1={0,"hello"};
```
当然，初始值的顺序必须与声明的顺序一致。与初始化数组元素的规则一样，如果初始值列表中的元素个数少于类的成员数量，则靠后的成员被值初始化。初始值列表的元素个数绝对不能超过类的成员数量。

### 访问与封装

在 c++ 语言中，我们使用 访问说明符 ([access specifiers](https://stackoverflow.com/questions/5447498/what-are-access-specifiers-should-i-inherit-with-private-protected-or-public)) 加强类的封装性；

封装有两个重要的优点：
* 确保用户代码不会无意间破坏封装对象的状态。
* 被封装的类的具体实现细节可以随时改变，而无须调整用户级别的代码

#### 使用 class 或 struct 关键字
使用 `class` 和 `struct` 定义类唯一的区别就是默认的访问权限。

#### 友元
[Friend class and function in C++](https://www.geeksforgeeks.org/friend-class-function-cpp/)

```highLight
一般来说，最好在类定义开始或结束前的位置集中声明友元。
```

```c++
class Screen{
	friend class Window_mgr;
};
```
必须要注意的一点是，友元关系并不存在传递性。也就是说，如果 Window_mgr 有它自己的友元，则这些友元并不能理所当然的具有访问 Screen 的特权。

```highLight
每个类控制自己的友元类或友元函数。
```

[When should you use 'friend' in C++?](https://stackoverflow.com/questions/17434/when-should-you-use-friend-in-c)

#### 返回 * this 的成员函数

##### 从 const 成员函数返回 * this
`一个 const 成员函数如果以引用的形式返回 * this ，那么它的返回类型将是常量引用。`

#### 类的声明
就像可以把函数的声明和定义分离开来一样，我们也能仅仅声明类而暂时不定义它：
```c++
class Screen; // Screen 类的声明
```
这种声明有时被称作前向声明 (forward declaration)，它向程序中引入名字 Screen 并且指明 Screen 是一种类类型。对于类型 Screen 来说，在它声明之后定义之前是一个不完全类型 (incomplete type)，也就是说，此时我们已知 Screen 是一个类类型，但是不清楚它到底包含哪些成员。

一旦一个类的名字出现后，他就被认为是声明过了（但尚未定义），因此类允许包含指向它自身类型的引用或指针:
```c++
class Link_screen{
	Screen window;
	Link_screen * next;
	Link_screen * prev;
};
```

### 类的作用域
```highLight
编译器处理完类中的全部声明后才会处理成员函数的定义。
```

```c++
// 不建议这样写
void Screen::dummy_fcn(pos height)
{
	cursor=width * this->height; // 成员 height
	// 另一种表示该成员的方式
	cursor=width * Screen::height; //成员 height
}
```
```highLight
尽管类的成员被隐藏了，但我们仍然可以通过加上类的名字或显式地使用 this 指针来强制访问成员。
```

如果我们需要的是外层作用域中的名字，可以显式地通过作用域运算符来进行请求:
```c++
//不建议的写法
void Screen::dummy_fcn(pos height)
{
	cursor=width * ::height; //那个 height? 不是成员 height,而是全局的 height
}
```
```highLight
尽管外层的对象被隐藏掉了，但我们仍然可以用作用域运算符访问它。
```

### 类的静态成员
```c++
class Account{
public:
	void calculate() {amount += amount * interestRate;}
	static double rate() {return interestRate;}
	static void rate(double);
private:
	std::string owner;
	double amount;
	static double interestRate;
	static double initRate();
};
```
类的静态成员存在于任何对象之外，对象中不包含任何与静态数据有关的数据。类似的，静态成员函数也不与任何对象绑定在一起，它们不包含 this 指针。作为结果，静态成员函数不能声明成 const ，而且我们也不能再 static 函数体内使用 this 指针。这一限制适用于 this 的显式使用，也对调用非静态成员的隐式使用有效。

使用类的静态成员
```c++
double r = Account::rate(); // 使用作用域运算符访问静态成员
```
虽然静态成员不属于类的某个对象，但是我们仍然可以使用类的对象、引用或者指针来访问静态成员：
```c++
Account ac1;
Account * ac2=&ac1;
// 调用静态成员函数 rate 的等价形式
r = ac1.rate();
r = ac2->rate();
```
成员函数不用通过作用域运算符就能直接使用静态成员：
```c++
class Account{
public:
	void calculate() {amount += amount * interestRate;}
private:
	static double interestRate;
	//其他成员与之前的版本一致
};

// 当在类的外部定义静态成员时，不能重复 static 关键字，该关键字只出现在类内部的声明语句：
void Account::rate(double newRate)
{
	interestRate=newRate;
}
```

Note:
* `和类的所有成员一样，当我们指向类外部的静态成员时，必须指明成员所属的类名。staic 关键字则只出现在类内部的声明语句中`。

```c++
double Account::interestRate=initRate();
//和其他成员的定义一样， interestRate 的定义也可以访问类的私有成员
```

Tips:
* `要想确保对象只定义一次，最好的办法是把静态数据成员的定义与其他非内联函数的定义放在同一个文件中。`

静态成员的类内初始化
通常情况下，类的静态成员不应该在类的内部初始化。然而，我们可以为静态成员提供 const 整数类型的类内初始值，不过要求静态成员必须是字面值常量类型的 constexpr。
```c++
class Account{
public:
	static double rate(){return interestRate;}
	static void rate(double);
private:
	static constexpr int period = 30; // period 是常量表达式
	double daily_tbl[period];
};

// 即使一个常量静态数据成员在类内部被初始化了，通常情况下也应该在类的外部定义一下该成员。
constexpr int Account::period;
```

如果某个静态成员的应用场景仅限于编译器可以替换它的值的情况，则一个初始化的 const 或 constexpr static 不需要分别定义。相反，如果我们将它用于值不能替换的场景中，则该成员必须有一条定义语句。

#### 静态成员能用于某些场景，而普通成员不能
```c++
class Bar{
public:
	// ...
private:
	static Bar mem1; // 正确，静态成员可以是不完全类型
	Bar * mem2; //正确，指针成员可以是不完全类型
	Bar mem3; //错误，数据成员必须是完全类型
};
```

静态成员和普通成员的另外一个区别就是我们可以使用静态成员作为默认实参
```c++
class Screen{
public:
	// bkground 表示一个在类中稍后定义的静态成员
	Screen& clear(char = bkground);
private:
	static const char bkground;
};
```
非静态数据成员不能作为默认实参，因为它的值本身属于对象的一部分，这么做的结果是无法真正提供一个对象以便从中获取成员的值，最终将引发错误。

Exercise 7.58
```c++
// example.h
class Example {
public:
    static double rate = 6.5;
    static const int vecSize = 20;
    static vector<double> vec(vecSize);
};

// example.C
#include "example.h"
double Example::rate;
vector<double> Example::vec;
```

```c++
static double rate = 6.5;
                ^
            rate should be a constant expression.

static vector<double> vec(vecSize);
                            ^
            we may not specify an in-class initializer inside parentheses.
```

Fixed:
```c++
// example.h
class Example {
public:
    static constexpr double rate = 6.5;
    static const int vecSize = 20;
    static vector<double> vec;
};

// example.C
#include "example.h"
constexpr double Example::rate;
vector<double> Example::vec(Example::vecSize);
```

### 小结
类有两项基本能力：
1. 数据抽象，即定义数据成员和函数成员的能力
2. 封装，即保护类的成员不被随意访问的能力。通过将类的实现细节设为 private ，我们就能完成类的封装。

## IO 类
[cplusplus/Input/output library](http://www.cplusplus.com/reference/iolibrary/)
[cppreference/Input/output library](https://en.cppreference.com/w/cpp/io)

### IO 类

#### IO 对象无拷贝或赋值
```c++
ofstream out1,out2;
out1=out2; //error
ofstream print(ofstream); //error
out2=print(out2); //error
```
由于不能拷贝 IO 对象，因此我们也不能将形参或返回类型设置为流类型。进行 IO 操作的函数通常以引用方式传递和返回流。读写一个 IO 对象会改变其状态，因此传递和返回的引用不能是 const 的。

#### 条件状态
[std::basic_ios](https://en.cppreference.com/w/cpp/io/basic_ios)
[std::ios_base::iostate](https://en.cppreference.com/w/cpp/io/ios_base/iostate)

下面是一个 IO 错误的例子：
```c++
int val;
cin >> val;
```
如果我们在标准输入上键入 Boo,读操作就会失败。代码中的输入运算符期待读取一个 int ，但却得到了一个字符 B。这样，cin 会进入错误状态。类似的，如果我们输入一个文件结束标识，cin 也会进入错误状态。

一个流一旦发生错误，其上后续的 IO 操作都会失败。确定一个流对象的最简单的方法将它当作一个条件来使用：
```c++
while (cin >> word) {
	// ok: 读操作成功 ...
}
```
我们可以使用 good() 或 fail() 来确定流的总体状态。

管理条件状态,eg:
```c++
auto oldState=cin.rdstate(); // 记住 cin 的当前状态
cin.clear(); //使 cin 有效
processInput(cin); //使用 cin
cin.setstate(oldState); // 将 cin 置为原有状态
```

#### 管理输出缓冲
每个输出流都管理一个缓冲区，用来保存程序读写的数据。例如
```c++
os << "please enter a value: ";
```
文本串可能立即打印出来，但也有可能被操作系统保存在缓冲区中，随后再打印。`有了缓冲机制，操作系统就可以将程序的多个输出操作组合成单一的系统级写操作。由于设备的写操作可能很耗时，允许操作系统将多个输出操作组合为单一的设备写操作可以带来很大的性能提升。`

一个输出流可能被关联到另一个流。在这种情况下，当读写被关联的流时，关联到的流的缓冲区会被刷新。例如，默认情况下，cin 和 cerr 都被关联到 cout。 因此，读 cin 或 写 cerr 都会导致 cout 的缓冲区被刷新。

[std::flush](https://en.cppreference.com/w/cpp/io/manip/flush)

[std::unitbuf, std::nounitbuf](https://en.cppreference.com/w/cpp/io/manip/unitbuf),如果想在每次输出操作后都刷新缓冲区，我们可以使用 unitbuf 操作符，eg:
```c++
cout<<unitbuf; // 所有输出操作后都会立即刷新缓冲区
//任何输出都立即刷新，无缓冲
cout<< nounitbuf; //回到正常的缓冲方式
```

警告：`如果程序崩溃，输出缓冲区不会被刷新`

关联输入和输出流
当一个输入流被关联到一个输出流时，任何试图从输入流读取数据的操作都会先刷新关联的输出流。标准库将 cout 和 cin 关联在一起，因此下面语句：
```c++
cin >> val;
```
导致 cout 的缓冲区被刷新。

Note：`交互式系统通常应该关联输入流和输出流。这意味着所有输出，包括用户提示信息，都会在读操作之前被打印出来。`

将一个给定的流关联到一个新的流，我们可以使用[std::basic_ios::tie](https://en.cppreference.com/w/cpp/io/basic_ios/tie)

### 文件输入输出

#### 文件模式
[openmode](https://en.cppreference.com/w/cpp/io/ios_base/openmode)

以 out 模式打开文件会丢弃已有数据

```c++
ofstream out("file1"); //隐含以输出模式打开文件并截断文件
// 为了保留文件内容，我们必须显式指定 app 模式
ofstream app("file2",ofstream::app);
```

Warning:`保留被 ofstream 打开的文件中已有数据的唯一方法是显式指定 app 或 in 模式`

### string 流
[stringstream in C++ and its applications](https://www.geeksforgeeks.org/stringstream-c-applications/)

## 顺序容器
一个容器就是一些特定类型对象的集合。顺序容器(sequential container) 为程序员提供了控制元素存储和访问顺序的能力。这种顺序不依赖于元素的值，而是与元素加入容器时的位置相对应。

* [std::vector](https://en.cppreference.com/w/cpp/container/vector)
* [std::array](https://en.cppreference.com/w/cpp/container/array)
* [std::deque](https://en.cppreference.com/w/cpp/container/deque)
* [std::forward_list](https://en.cppreference.com/w/cpp/container/forward_list)

### 顺序容器概述
下表列出了标准库中的顺序容器。但是，这些容器在以下方面都有不同的性能折中：
* 向容器添加或从容器中删除元素的代价
* 非顺序访问容器中元素的代价

|    vector    |    可变大小数组。支持快速随机访问。在尾部之外的位置插入或删除元素可能很慢   |
|:------------:|:---------------------------------------------------------------------------:|
|     deque    |           双端队列。支持快速随机访问。在头尾位置插入/删除速度很快           |
|     list     | 双向链表。只支持双向顺序访问。在 list 中任何位置进行插入/删除操作速度都很快 |
| forward_list |   单向链表。只支持单向顺序访问。在链表任何位置进行插入/删除操作速度都很快   |
|     array    |             固定大小数组。支持快速随机访问。不能添加或删除元素。            |
|    string    | 与 vector 相似的容器，但专门用于保存字符。随机访问快。在尾部插入/删除速度快 |

除了固定大小的 array 外，其他容器都提供高效、灵活的内存管理。我们可以添加和删除元素，扩张和收缩容器的大小。容器保存元素的策略对容器操作的效率有着固有的，有时是重大的影响。在某些情况下，存储策略还会影响特定容器是否支持特定操作。

例如，string 和 vector 将元素保存在连续的内存空间中。由于元素是连续存储的，由元素的下标来计算其地址是非常快速的。但是，在这两种容器的中间位置添加或删除元素就会非常耗时：在一次插入或删除操作后，需要移动插入/删除位置之后的所有元素，来保持连续存储。而且，添加一个元素有时可能还需要分配额外的存储空间。在这种情况下，每个元素都必须移动到新的存储空间中。

list 和 forward_list 两个容器的设计目的是令容器任何位置的添加和删除操作都很快速。作为代价，这两个容器不支持元素的随机访问：为了访问一个元素，我们只能遍历整个容器。而且，与 vector 、 deque 和 array 相比，这两个容器的额外内存开销也很大。

deque 是一个更为复杂的数据结构。与 string 和 vector 一样，在 deque 的中间位置添加或删除元素的代价（可能）很高。但是，在 deque 的两端添加或删除元素都是很快的，与 list 或 forward_list 添加删除元素的速度相当。

forward_list 和 array 是 c++11 增加的类型。与内置数组相比， array 是一种更安全、更容易使用的数组类型。与内置数组类似， array 对象的大小是固定的。forward_list 的设计目标是达到与最好的手写的单向链表数据结构相当的性能。因此， forward_list 没有 size 操作，因为保存或计算其大小就会比手写链表多出额外的开销。对其他容器而言，size 保证是一个快速的常量时间的操作。

Tips：`通常，使用 vector 是最好的选择，除非你有很好的理由选择其他容器`

[Choosing the Right Container: Sequential Containers](https://embeddedartistry.com/blog/2017/9/11/choosing-the-right-stl-container-sequential-containers)

### 容器库概览

```c++
// 假定 noDefault 是一个没有默认构造函数的类型
std::vector<noDefault> v1(10,init); //正确，提供了元素初始化器
std::vector<noDefault> v2(10); // 错误，必须提供一个元素初始化器
```

迭代器范围：`[begin,end)`

Note:`只有顺序容器的构造函数才接受大小参数，关联容器并不支持`

#### 使用 assign (仅顺序容器)
[std::vector::assign](https://en.cppreference.com/w/cpp/container/vector/assign)

赋值运算符要求左边和右边的运算对象具有相同的类型。它将右边运算符对象中所有元素拷贝到左边运算对象中。顺序容器（array 除外）还定义了一个名为 assign 的成员，允许我们从一个不同但相容的类型赋值，或者从容器的一个子序列赋值。例如：
```c++
list<string> names;
std::vector<const char *> oldstyle;
names = oldstyle; //错误，容器类型不匹配
// 正确，可以将 const char * 转换为 string
names.assign(oldstyle.cbegin(),oldstyle.cend());
```

Warning:`由于其旧元素被替换，因此传递给 assign 的迭代器不能指向调用 assign 的容器`。

#### 使用 swap
swap 操作交换两个相同类型容器的内容。调用 swap 之后，两个容器中的元素将会交换：
```c++
std::vector<string> v1(10);
std::vector<string> v2(24);
std::swap(v1,v2);
```
除 array 外，交换两个容器内容的操作保证会很快`（元素本身并未交换，swap 只是交换了两个容器的内部数据结构）`

Note：`除 array 外，swap 不对任何元素进行拷贝、删除或插入操作，因此可以保证在常数时间内完成.`

除 string 外，指向容器的迭代器、引用和指针在 swap 操作之后都不会失效。它们仍指向 swap 操作之前所指向的那些元素。但是，在 swap 之后，这些元素已经属于不同的容器了。例如，假定 iter 在 swap 之前指向  v1[3] 的 string ,那么在 swap 之后它指向  v2[3] 的元素。

与其他容器不同，swap 两个 array 会真正交换它们的元素。因此，交换两个 array 所需的时间与 array 中元素的数目成正比。

#### 关系运算符

容器的关系运算符使用元素的关系运算符完成比较
Note：`只有当其元素类型也定义了相应的比较运算符时，我们才可以使用关系运算符来比较两个容器。`


### 顺序容器操作

Warning:`向一个 vector,string 或 deque 插入元素会使所有指向容器的迭代器、引用和指针失效`

当我们使用这些操作时，必须记得不同容器使用不同的策略来分配元素空间，而这些策略直接影响性能。在一个 vector 或 string 的尾部之外的任何位置，或是一个 deque 的首尾之外的任何位置添加元素，都需要移动元素。而且，像一个 vector 或 string 添加元素可能引起整个对象存储空间的重新分配。重新分配一个对象的存储空间需要分配新的内存，并将元素从旧的空间移动到新的空间中。

Warning:`将元素插入到 vector、deque 和 string 中的任何位置都是合法的。然而，这样做可能很耗时。`

插入范围内元素，eg:
```c++
svec.insert(svec.end(),10,"Anna");

std::vector<string> v={"quasi","simba","frollo","scar"};
slist.insert(slist.end(),v.end()-2,v.end());
slist.insert(slist.end(),{"these","words","will",
	"go","at","the","end"});
```

```c++
slist.insert(slist.begin(),slist.begin(),slist.end());
//运行时错误，迭代器表示要拷贝的范围，不能指向与目的位置相同的容器
```
如果我们传递给 insert 一对迭代器，它们不能指向添加元素的目标容器。

使用 insert 的返回值,eg:
```c++
list<string> lst;
auto iter=lst.begin();
while(cin>>word)
	iter=lst.insert(iter,word); // 等价于调用push_front
```

#### 使用 emplace 操作
[std::vector::emplace](https://en.cppreference.com/w/cpp/container/vector/emplace)

新标准引入了三个新成员 - emplace_front, emplace 和 emplace_back ,这些操作构造而不是拷贝元素。这些操作分别对应 push_front,insert 和 push_back,允许我们将元素放置在容器头部、一个指定的位置之前或容器尾部。
当调用 push 或 insert 成员函数时，我们将元素类型的对象传递给它们，这些对象被拷贝到容器中。而当我们调用一个 emplace 成员函数时，则是将参数传递给元素类型的构造函数。 emplace 成员使用这些参数在容器管理的内存空间中直接构造元素。

[vector emplace() function in C++ STL](https://www.geeksforgeeks.org/vector-emplace-function-in-c-stl/)

Note:`emplace 函数在容器中直接构造元素。传递给 emplace 函数的参数必须与元素类型的构造函数相匹配。`

练习 9.22:假定 iv 是一个 int 的 vector ，下面的程序存在什么错误？你将如何修改？
```c++
vector<int>::iterator iter = iv.begin(), mid = iv.begin() + iv.size()/2;
while (iter != mid)
    if (* iter == some_val)
        iv.insert(iter, 2 * some_val);
```
Fixed:
```c++
#include <iostream>
#include <vector>

void double_and_insert(std::vector<int>& v, int some_val)
{
    auto mid = [&]{ return v.begin() + v.size() / 2; };
    for (auto curr = v.begin(); curr != mid(); ++curr)
        if (*curr == some_val)
            ++(curr = v.insert(curr, 2 * some_val));
}

int main()
{
    std::vector<int> v{ 1, 9, 1, 9, 9, 9, 1, 1 };
    double_and_insert(v, 1);

    for (auto i : v)
        std::cout << i << std::endl;
}
```
[Exercise 9.22:](https://github.com/Mooophy/Cpp-Primer/tree/master/ch09#exercise-922)

#### 访问元素
Warning:`对一个空容器调用 front 和 back ，就像使用一个越界的下标一样，是一个严重的程序设计错误。`

在容器中访问元素的成员函数（即，front,back,下标和 at）返回的都是引用。

#### 删除元素
Warning:`删除 deque 中除首尾位置之外的任何元素都会使所有迭代器、引用和指针失效。指向 vector 或 string 中删除点之后位置的迭代器、引用和指针都会失效。`

Warning:`删除元素的成员函数并不检查其参数。在删除元素之前，程序员必须确保它们是存在的。`

例如，下面的循环删除一个list 中的所有奇数元素：
```c++
list<int> lst={0,1,2,3,4,5,6,7,8,9};
auto it=lst.begin();
while (it!=lst.end()) {
	if(*it%2) // 若元素为奇数
		it=lst.erase(it); //删除此元素
	else
		++it;
}
```

练习 9.26: 使用下面代码定义的 ia，将 ia 拷贝到一个 vector 和一个 list 中。使用单迭代器版本的 erase 从 list 中删除奇数元素，从 vector 中删除偶数元素。
```highLight
int ia[] = { 0, 1, 1, 2, 3, 5, 8, 13, 21, 55, 89 };
```
```c++
#include <iostream>
#include <vector>
#include <list>

using std::vector; using std::list; using std::cout; using std::endl; using std::end;

int main()
{
    int ia[] = { 0, 1, 1, 2, 3, 5, 8, 13, 21, 55, 89 };

    // init
    vector<int> vec(ia, end(ia));
    list<int> lst(vec.begin(), vec.end());

    // remove odd value
    for(auto it = lst.begin();  it != lst.end(); )
        if(*it & 0x1) it = lst.erase(it);
        else ++it;

    // remove even value
    for(auto it = vec.begin();  it != vec.end(); )
        if(! (*it & 0x1)) it = vec.erase(it);
        else ++it;

    // print
    cout << "list : ";
    for(auto i : lst)   cout << i << " ";
    cout << "\nvector : ";
    for(auto i : vec)   cout << i << " ";
    cout << std::endl;

    return 0;
}
```
[Cpp-Primer/ch09/ex9_26.cpp](https://github.com/Mooophy/Cpp-Primer/blob/master/ch09/ex9_26.cpp)

#### 特殊的 forward_list 操作
[std::forward_list](https://en.cppreference.com/w/cpp/container/forward_list)

当在 forward_list 中添加或删除元素时，我们必须关注两个迭代器-一个指向我们要处理的元素，另一个指向其前驱。例如，可以改写之前从list 中删除奇数元素的循环程序，将其改为从 forward_list 中删除元素：
```c++
forward_list<int> flst={0,1,2,3,4,5,6,7,8,9};
auto prev=flst.before_begin(); // 表示 flst 的“首前元素”
auto curr=flst.begin();
while (curr!=flst.end()) {
	if(*curr % 2)
		curr=flst.erase_after(prev); //删除并移动 curr
	else
	{
		prev=curr; // 移动迭代器 curr,指向下一个元素，prev 指向
		++curr; // curr 之前的元素
	}
}
```

#### 改变容器大小
我们可以使用 resize 来增大或缩小容器，与往常一样， array 不支持 resize。如果当前大小大于所要求的大小，容器后部的元素会被删除；如果当前大小小于新大小，会将新元素添加到容器后部；
```c++
c.resize(n); // 调整 c 的大小为 n 个元素。
c.resize(n,t); //调整 c 的大小为 n 个元素。任何新添加的元素都被初始化为 t
```
Warning:`如果 resize 缩小容器，则指向被删除元素的迭代器、引用和指针都会失效；对 vector string deque 进行 resize 可能导致迭代器、指针和引用失效。`

#### 容器操作可能使迭代器失效
向容器中添加元素和从容器中删除元素的操作可能会使指向容器元素的指针、引用或迭代器失效。一个失效的指针、引用或迭代器将不再表示任何元素。使用失效的指针、引用或迭代器是一种严重的程序设计错误，很可能引起与使用未初始化指针一样的问题。

在向容器添加元素后：
* 如果容器是 vector 或 string ，且存储空间被重新分配，则指向容器的迭代器、指针和引用都会失效。如果存储空间未重新分配，指向插入位置之前的元素迭代器、指针和引用仍有效，但指向插入位置之后元素的迭代器、指针和引用将会失效。
* 对于 deque ,插入到除首尾位置之外的任何位置都会导致迭代器、指针和引用失效。如果在首尾位置添加元素，迭代器会失效，但指向存在的元素的引用和指针不会失效。
* 对于 list 和 forward_list ，指向容器的迭代器（包括尾后迭代器和首前迭代器）、指针和引用仍有效。

当我们从一个容器中删除元素后，指向被删除元素的迭代器、指针和引用会失效，这应该不会令人惊讶。毕竟，这些元素都已经被销毁了。当我们删除一个元素后：
* 对于 list 和 forward_list ，指向容器其他位置的迭代器（包括尾后迭代器和首前迭代器）、引用和指针仍有效。
* 对于 deque,如果在首尾之外的任何位置删除元素，那么指向被删除元素外其他元素的迭代器、引用或指针也会失效。如果是删除 deque 的尾元素，则尾后迭代器也会失效，但其他迭代器、引用和指针不受影响；如果是删除首元素，这些也不会受影响。
* 对于 vector 和 string ，指向被删元素之前元素的迭代器、引用和指针仍有效。注意：当我们删除元素时，尾后迭代器总是会失效。

Warning:`使用失效的迭代器、指针或引用时严重的运行时错误。`

建议：管理迭代器
`当你使用迭代器（或指向容器元素的引用或指针）时，最小化要求迭代器必须保持有效的程序片段是一个好的方法。
由于向迭代器添加元素和从迭代器删除元素的代码可能会使迭代器失效，因此必须保证每次改变容器的操作之后都正确地重新定位迭代器。这个建议对 vector string 和 deque 尤为重要。`

##### 编写改变容器的循环程序
添加/删除 vector string 或 deque 元素的循环程序必须考虑迭代器、引用和指针可能失效的问题。程序必须保证每个循环步中都更新迭代器、引用或指针。如果循环中调用的是 insert 或 erase ，那么更新迭代器很容易。这些操作都返回迭代器，我们可以用来更新：
```c++
//傻瓜循环，删除偶数元素，复制每个奇数元素
std::vector<int> vi={0,1,2,3,4,5,6,7,8,9};
auto iter=vi.begin();
while (iter != vi.end()) {
	if(*iter % 2)
	{
		iter=vi.insert(iter,*iter); // 复制当前元素
		iter+=2; // 向前移动迭代器，跳过当前元素以及插入到它之前的元素
	}else
		iter=vi.erase(iter); //删除偶数元素
		//不应向前移动迭代器，iter 指向我们删除的元素之后的元素
}
```
在调用 erase 后，不必递增迭代器，因为 erase 返回的迭代器已经指向序列中下一个元素。调用 insert 后，需要递增迭代器两次。记住，insert 在给定位置之前插入新元素，然后返回指向新插入元素的迭代器。因此，在调用 insert 后，iter 指向新插入元素，位于我们正在处理的元素之前。我们将迭代器递增两次，恰好越过了新添加的元素和正在处理的元素，指向下一个未处理的元素。

##### 不要保存 end 返回的迭代器
当我们添加/删除 vector 或 string 的元素后，或在 deque 中首元素之外任何位置添加/删除元素后，原来 end 返回的迭代器总会失效。因此，添加或删除元素的循环程序必须反复调用 end ,而不能在循环之前保存 end 返回的迭代器，一直当作容器末尾使用。通常 c++ STL 实现中 end() 操作都很快，部分就是因为这个原因。
```c++
// 灾难： 此循环的行为是未定义的
auto begin=v.begin(),end=v.end(); // 保存尾迭代器的值是一个坏主意
while (begin!=end) {
	// 做一些处理
	//插入新值，对 begin 重新赋值，否则的话它就会失效
	++begin; //向前移动 begin,因为我们想在此元素之后插入元素
	begin=v.insert(begin,42); //向前移动 begin 跳过我们刚刚加入的元素
	++begin;
}
```
此代码的行为是未定义的。在很多标准库实现上，此代码会导致无限循环。

Tips:`如果在一个循环中插入/删除 deque string 或 vector 中的元素，不要缓存 end 返回的迭代器。`

必须在每次插入操作后重新调用end()，而不能在循环开始前保存它返回的迭代器：
```c++
// 更安全的方法：在每个循环步添加/删除元素后都重新计算 end
while (begin!=v.end()) {
	// 做一些处理
	++begin;  //向前移动 begin,因为我们想在此元素之后插入元素
	begin=v.insert(begin,42);
	++begin; //向前移动 begin 跳过我们刚刚加入的元素
}
```

### vector 对象是如何增长的
为了支持快速随机访问，vector 将元素连续存储。

为了避免每次添加新元素都重新分配容器内存空间，STL 实现者采用了可以减少容器重新分配次数的策略。当不得不获取新的内存空间时，vector 和 string 的实现通常会分配比新的空间需求更大的内存空间。其实际性能也表现的足够好-虽然 vector 在每次重新分配内存空间时都要移动所有元素，但使用此策略后，其扩张操作通常比 list 和 deque 还要快。

管理容量的成员函数：
* [std::vector::shrink_to_fit](https://en.cppreference.com/w/cpp/container/vector/shrink_to_fit) 请将 capacity() 减少为与 size() 相同大小
* [std::vector::capacity](https://en.cppreference.com/w/cpp/container/vector/capacity) 不重新分配内存空间的话，可以保存多少元素
* [std::vector::reserve](https://en.cppreference.com/w/cpp/container/vector/reserve) 分配至少能容纳 n 个元素的内存空间

shrink_to_fit 只适用于 vector string 和 deque
capacity 和 reserve 只适用于 vector 和 string

Note:`reserve 并不改变容器中元素的数量，它仅影响 vector 预先分配多大的内存空间。`

只有当需要的内存空间超过当前容量时，reserve 调用才会改变容器的容量。
我们可以调用 shrink_to_fit 来要求容器退回不需要的内存空间。但是具体的实现可以选择忽略此请求。也就是说，调用 shrink_to_fit 也不保证一定退回内存空间。
下面是一个简单的示例:
```c++
#include <vector>
#include <iostream>

using namespace std;

void showInfo(const std::vector<int> &v)
{
  std::cout << "v: size: "<< v.size()
    << " capacity: "<< v.capacity() << '\n';
}

int main(int argc, char const *argv[]) {
  std::vector<int> v;
  showInfo(v);

  // add 24 item
  std::cout << "add 24 item" << '\n';
  for(vector<int>::size_type i=0;i!=24;++i)
    v.push_back(i);
  showInfo(v);

  std::cout << "reserve(50)" << '\n';
  v.reserve(50); //将 capacity 至少设定为50，可能会更大
  showInfo(v);

  // 用光这些预留空间
  std::cout << "use all" << '\n';
  while (v.size() != v.capacity()) {
    v.push_back(0);
  }
  showInfo(v);

  //再添加一个新的元素，vector 就不得不重新分配空间：
  std::cout << "add one item,vector must resize " << '\n';
  v.push_back(12);
  showInfo(v);

  // 用光这些预留空间
  std::cout << "use all" << '\n';
  while (v.size() != v.capacity()) {
    v.push_back(0);
  }
  showInfo(v);

  //再添加一个新的元素，vector 就不得不重新分配空间：
  std::cout << "add one item,vector must resize " << '\n';
  v.push_back(12);
  showInfo(v);

  std::cout << "use shrink_to_fit function" << '\n';
  v.shrink_to_fit();
  showInfo(v);
  return 0;
}
```
Run:
```sh
v: size: 0 capacity: 0
add 24 item
v: size: 24 capacity: 32
reserve(50)
v: size: 24 capacity: 50
use all
v: size: 50 capacity: 50
add one item,vector must resize
v: size: 51 capacity: 100
use all
v: size: 100 capacity: 100
add one item,vector must resize
v: size: 101 capacity: 200
use shrink_to_fit function
v: size: 101 capacity: 101
```
vector的实现策略似乎是在每次需要分配新内存空间时将当前容量翻倍。

Note:`每个 vector 实现都可以选择自己的内存分配策略。但是必须遵守的一条原则是：只有当迫不得已时才可以分配新的内存空间。`

虽然不同的实现可以采用不同的分配策略，但所有实现都应遵循一个原则：确保用 push_back 向 vector 添加元素的操作有高效率。从技术角度说，就是通过再一个初始为空的 vector 上调用n次 push_back 来创建一个 n 个元素的 vector,所花费的时间不能超过 n 的常数倍。

练习 9.38: 编写程序，探究在你的标准库实现中，vector 是怎样增长的.
```c++
#include <vector>
#include <iostream>

using namespace std;

int main(int argc, char const *argv[]) {
  std::vector<int> v;
  std::cout << "v: size: "<< v.size()
    << " capacity: "<< v.capacity() << '\n';
  v.push_back(1);

  for (size_t i = 0; i < 10; i++) {
    while (v.size() != v.capacity()) {
      v.push_back(0);
    }
    v.push_back(1);
    std::cout << "v: size: "<< v.size()
      << " capacity: "<< v.capacity() << '\n';
  }
  return 0;
}
```
Run:
```sh
v: size: 0 capacity: 0
v: size: 2 capacity: 2
v: size: 3 capacity: 4
v: size: 5 capacity: 8
v: size: 9 capacity: 16
v: size: 17 capacity: 32
v: size: 33 capacity: 64
v: size: 65 capacity: 128
v: size: 129 capacity: 256
v: size: 257 capacity: 512
v: size: 513 capacity: 1024
```

### 额外的 string 操作
[std::basic_string](https://en.cppreference.com/w/cpp/string/basic_string)
除了顺序容器的共同操作之外，string 类型还提供了一些额外的操作。
#### 构造函数的其他方法
* string s(cp,n) s 是 cp 指向的数组中前 n 个字符的拷贝。此数组至少应该包含 n 个字符
* string s(s2,pos) s 是 string s2 从下标 pos2 开始的字符的拷贝。若 pos2 > s2.size() ，构造函数的行为未定义
* string s(s2,pos2,len2) s 是 string s2 从下标 pos2 开始 len2 个字符的拷贝。若 pos2>s2.size(), 构造函数的行为未定义。不管 len2 的值是多少，构造函数至多拷贝 s2.size()-pos2 个字符。
* n,len2,pos2 都是无符号值。
##### substr 操作
* [std::basic_string::substr](https://en.cppreference.com/w/cpp/string/basic_string/substr)

#### 改变 string 的其他方法
* [std::basic_string::insert](https://en.cppreference.com/w/cpp/string/basic_string/insert)
* [std::basic_string::erase](https://en.cppreference.com/w/cpp/string/basic_string/erase)
* [std::basic_string::assign](https://en.cppreference.com/w/cpp/string/basic_string/assign)
* [std::basic_string::append](https://en.cppreference.com/w/cpp/string/basic_string/append)
* [std::basic_string::replace](https://en.cppreference.com/w/cpp/string/basic_string/replace)

string 类型支持顺序容器的赋值运算符以及 assign,insert,erase 操作。除此之外，它还定义了额外的 insert 和 erase 版本。

除了接受迭代器的 insert 和 erase 版本外， string 还提供了接受下标的版本。下标指出了开始删除的位置，或是 insert 到给定值之前的位置：
```c++
s.insert(s.size(),5,'!'); //在 s 末尾插入 5 个感叹号
s.erase(s.size()-5,5); //从 s 删除最后 5 个字符
```
STL string 类型还提供了接受 c 风格字符数组的 insert 和 assign 版本。
```c++
const char *cp="Stately,Plump Buck";
s.assign(cp,7); //s == "Stately"
s.insert(s.size(),cp+7); //s == "Stately,Plump Buck"
```
我们也可以指定将来自其他 string 或 子字符串的字符插入到当前 string 中赋予当前 string:
```c++
string s="some string",s2="some other string";
s.insert(0,s2); //再 s 中位置 0 之前插入 s2 的拷贝
s.insert(0,s2,0,s2.size()); //在 s[0] 之前插入 s2 中 s2[0] 开始的 s2.size() 个字符
```

append 操作是在 string 末尾进行插入操作的一种简写形式：
```c++
string s("C++ Primer"),s2=s;
s.insert(s.size()," 4th Ed.");
s2.append(" 4th Ed.");
```
replace 操作是调用 erase 和 insert 的一种简写形式：
```c++
// 将 "4th" 替换为"5th" 的等价方法
s.erase（11，3）；
s.insert(11,"5th");
// equal
s2.replace(11,3,"5th"); // s==s2
```
此例中调用 replace 时，插入的文本恰好与删除的文本一样长。这不是必须的，可以插入一个更长或更短的 string：
```c++
s.replace(11,3,"Fifth"); //s == "C++ Primer Fifth Ed."
```

#### string 搜索操作
* [std::basic_string::find(args)](https://en.cppreference.com/w/cpp/string/basic_string/find) 查找 s 中 args 第一次出现的位置
* [std::basic_string::rfind(args)](https://en.cppreference.com/w/cpp/string/basic_string/rfind) 查找 s 中 args 最后一次出现的位置
* [std::basic_string::find_first_of(args)](https://en.cppreference.com/w/cpp/string/basic_string/find_first_of) 在 s 中查找 args 中任何一个字符第一次出现的位置
* [std::basic_string::find_first_not_of(args)](https://en.cppreference.com/w/cpp/string/basic_string/find_first_not_of) 在 s 中 查找第一个不在 args 中的字符
* [std::basic_string::find_last_of(args)](https://en.cppreference.com/w/cpp/string/basic_string/find_last_of) 在 s 中查找 args 中任何一个字符最后一次出现的位置
* [std::basic_string::find_last_not_of(args)](https://en.cppreference.com/w/cpp/string/basic_string/find_last_not_of) 在 s 中 查找最后一个不在 args 中的字符

每个搜索操作都返回一个 string::size_type 值，表示匹配发生位置的下标。如果搜索失败，则返回一个名为 string::npos 的 static 成员。STL 将 npos 定义为一个 const string::size_type 类型，并初始化为 -1。由于 npos 是一个 unsigned 类型，此初时值意味着 npos 等于任何 string 最大的可能大小。

Warning:`用一个 int 或其他带符号类型来保存 搜索函数的返回值不是一个好主意`

指定在哪里开始搜索：
```c++
string::size_type pos=0;
//每步循环查找 name 中下一个数
while ((pos= name.find_first_of(number,pos)!= string::npos)) {
	std::cout << "found number at index:"<< pos
	<< " element is "<< name[pos] << '\n';
	++pos;
}
```

逆向搜索：
```c++
string river("Mississippi");
auto first_pos=river.find("is"); //return 1
auto last_pos=river.rfind("is") //return 4
```
#### compare 函数
* [std::basic_string::compare](https://en.cppreference.com/w/cpp/string/basic_string/compare)

STL string 类型还提供了一组 compare 函数，这些函数与 c 标准库的 strcmp 函数很相似。类似 strcmp,根据 s 是 等于、大于还是小于参数指定的字符串，s.compare 返回0，正数或负数。

#### 数值转换
* [std::stoi, std::stol, std::stoll](https://en.cppreference.com/w/cpp/string/basic_string/stol)
* [std::stoul, std::stoull](https://en.cppreference.com/w/cpp/string/basic_string/stoul)
* [std::stof, std::stod, std::stold](https://en.cppreference.com/w/cpp/string/basic_string/stof)
* [std::to_string](https://en.cppreference.com/w/cpp/string/basic_string/to_string)
* [std::to_wstring](https://en.cppreference.com/w/cpp/string/basic_string/to_wstring)

Note:`如果 string 不能转换为一个数值，这些函数抛出一个 invalid_argument 异常。如果转换得到的数值无法用任何类型来表示，则抛出一个 out_of_range 异常。`

```c++
#include <iostream>

int main(int argc, char const *argv[]) {
  std::size_t index=0;
  float j=std::stof("+0.9_fjdsk",&index);
  std::cout << "val:"<< j <<",invaild index:"<<index <<'\n';
  int k=std::stoi("0x66",&index,16); //or 0X66
  std::cout << "val:"<< k <<",invaild index:"<<index <<'\n';
  return 0;
}
```
Run :
```sh
val:0.9,invaild index:4
val:102,invaild index:4
```

### 容器适配器
除了顺序容器外，STL 还定义了三个顺序容器适配器：stack queue 和 priority_queue。适配器是 STL 中的一个通用概念。容器、迭代器和函数都有适配器。本质上，一个适配器是一个机制，能使某种事物的行为看起来像另外一种事物一样。一个适配器接受一种已有的容器类型，使其行为看起来像一种不同的类型。例如，stack适配器接受一个顺序容器（除 array 或 forward_list外），并使其操作起来像一个 stack 一样。

所有容器适配器都支持的操作和类型：

|       size_type      |                     一种类型，足以保存当前类型的最大对象的大小                     |
|:--------------------:|:----------------------------------------------------------------------------------:|
|      value_type      |                                      元素类型                                      |
|    container_type    |                              实现适配器的底层容器类型                              |
|         A a;         |                              创建一个名为 a 的空适配器                             |
|        A a(c);       |                   创建一个名为 a 的适配器，带有容器 c 的一个拷贝                   |
|      关系运算符      | 每个适配器都支持关系运算符：==、！=、<、<=、>、>= 这些运算符返回底层容器的比较结果 |
|       a.empty()      |                                   适配器是否为空                                   |
|       a.size()       |                                 返回 a 中的元素数目                                |
| swap(a,b)  a.swap(b) |       交换a 和 b 的内容，a 和 b 必须有相同的类型，包括底层容器类型也必须相同       |

定义一个适配器:
默认情况下，stack 和 queue 是基于 deque 实现的，priority_queue 是在 vector 之上实现的。我们可以创建一个适配器时将一个命名的顺序容器作为第二个类型参数，来重载默认容器类型。
```c++
deque<int> deq;
stack<int> stk(deq); //从 deq 拷贝元素到 stk

// 在vector 上实现的空栈
stack<string,vector<string>> str_stk;
// str_stk2 在 vector 上实现，初始化时保存 svec的拷贝
stack<string,vector<string>> str_stk2(svec);
```

对于一个给定的适配器，可以使用哪些容器是有限制的。
* 所有适配器都要求容器具有添加和删除元素的能力。因此，适配器不能构造在 array 之上。类似的，我们也不能用 forward_list 来构造适配器，因为所有适配器要求容器具有添加、删除以及访问尾元素的能力。
* stack 只要求 push_back pop_back 和 back 操作，因此可以使用除 array 和 forward_list 之外的任何容器类型来构造 stack。
* queue 适配器要求 back push_back front 和 push_front,因此它可以构造于 list 或 deque 之上，但不能基于 vector 构造。
* priority_queue 除了 front push_back 和 pop_back 操作之外还要求随机访问能力，因此它可以构造于 vector 和 deque 之上，但不能基于 list 构造。

#### 栈适配器
* [std::stack](https://en.cppreference.com/w/cpp/container/stack)

```c++
stack<int> intStack;
for(size_t i=0;i!=10;++i)
	intStack.push(i);
while (!intStack.empty) {
	int value=intStack.top();
	// 使用栈顶值的代码
	intStack.pop(); // 弹出栈顶元素，继续循环
}
```

#### deque
* [std::queue](https://en.cppreference.com/w/cpp/container/queue)

#### priority_queue
* [std::priority_queue](https://en.cppreference.com/w/cpp/container/priority_queue)

priority_queue 允许我们为队列中的元素建立优先级。新加入的元素会排在所有优先级比它低的已有元素之前。

```c++
#include <functional>
#include <queue>
#include <vector>
#include <iostream>

template<typename T> void print_queue(T& q) {
    while(!q.empty()) {
        std::cout << q.top() << " ";
        q.pop();
    }
    std::cout << '\n';
}

int main() {
    std::priority_queue<int> q;

    for(int n : {1,8,5,6,3,4,0,9,7,2})
        q.push(n);

    print_queue(q);

    std::priority_queue<int, std::vector<int>, std::greater<int> > q2;

    for(int n : {1,8,5,6,3,4,0,9,7,2})
        q2.push(n);

    print_queue(q2);

    // Using lambda to compare elements.
    auto cmp = [](int left, int right) { return (left ^ 1) < (right ^ 1);};
    std::priority_queue<int, std::vector<int>, decltype(cmp)> q3(cmp);

    for(int n : {1,8,5,6,3,4,0,9,7,2})
        q3.push(n);

    print_queue(q3);

}
```
Run:
```sh
9 8 7 6 5 4 3 2 1 0
0 1 2 3 4 5 6 7 8 9
8 9 6 7 4 5 2 3 0 1
```
练习 9.52: 使用 stack 处理括号化的表达式。当你看到一个左括号，将其记录下来。当你在一个左括号之后看到一个右括号，从 stack 中 pop 对象，直到遇到左括号，将左括号也一起弹出栈。然后将一个值（括号内的运算结果）push 到栈中，表示一个括号化的（子）表达式已经处理完毕，被其运算结果所替代。
[Cpp-Primer/ch09/ex9_52.cpp](https://github.com/Mooophy/Cpp-Primer/blob/master/ch09/ex9_52.cpp)

## 关联容器
关联容器和顺序容器有着根本的不同：关联容器中的元素是按照关键字来保存和访问的。与之相对，顺序容器中的元素时按它们在容器中的位置来顺序保存和访问的。

关联容器类型：
* [std::map](https://en.cppreference.com/w/cpp/container/map)
* [std::set](https://en.cppreference.com/w/cpp/container/set)
* [std::multimap](https://en.cppreference.com/w/cpp/container/multimap)
* [std::multiset](https://en.cppreference.com/w/cpp/container/multiset)
* [std::unordered_map](https://en.cppreference.com/w/cpp/container/unordered_map)
* [std::unordered_set](https://en.cppreference.com/w/cpp/container/unordered_set)
* [std::unordered_multimap](https://en.cppreference.com/w/cpp/container/unordered_multimap)
* [std::unordered_multiset](https://en.cppreference.com/w/cpp/container/unordered_multiset)

注：其中 unordered_* 是由哈希实现的。

### 关联容器概述
关联容器不支持顺序容器的位置相关的操作，例如 push_front 或 push_back。 原因是关联容器中元素是根据关键字存储的，这些操作对关联容器没有意义。而且，关联容器也不支持构造函数或插入操作这些接受一个元素值和一个数量值的操作。

除了与顺序容器相同的操作之外，关联容器还支持一些顺序容器不支持的操作和类型别名。此外，无序容器还提供一些用来调整哈希性能的操作。

关联容器的迭代器是双向的。
#### 关键字类型的要求
对于有序容器--map、multimap、set以及 multiset,关键字类型必须定义元素比较的方法。默认情况下，STL 使用关键字类型的 < 运算符来比较两个关键字。在集合类型中，关键字类型就是元素类型；在映射类型中，关键字类型是元素的第一部分的类型。

Note:`传递给排序算法的可调用对象必须满足与关联容器中关键字一样的类型要求`

有序容器的关键字类型：
可以向一个算法提供我们自己定义的比较类型，与之类似，也可以提供自己定义的操作来代替关键字上的 < 运算符。所提供的操作必须在关键字类型上定义一个严格弱序（strict weak ordering）。可以将严格弱序看作“小于等于”。如果两个关键字是等价的（即，任何一个都不“小于等于”另一个），那么容器将它们视为相等来处理。
Note:`在实际编程中，重要的是，如果一个类型定义了“行为正常”的 < 运算符，则它可以用作关键字类型。`
eg:
```c++
bool compareIsbn(const Sales_data &lhs,const Sales_data &rhs)
{
	return lhs.isbn() < rhs.isbn();
}

multiset<Sales_data,decltype(compareIsbn) *> bookstore(compareIsbn);
```
此处，我们使用 decltype 来指出自定义操作的类型。记住，当用 decltype 来获得一个函数指针类型时，必须加上一个 * 来指出我们要使用一个给定函数类型的指针。这里可以用 compareIsbn 代替 &compareIsbn 作为构造函数的参数，因为当我们使用一个函数的名字时，在需要的情况下它会自动转换为一个指针。当然，使用 &compareIsbn 的效果也是一样的。

练习 11.11:不使用 decltype 重新定义 bookstore;
```c++
using compareIsbn1=bool (* )(const Sales_data &,const Sales_data &);
std::multiset<Sales_data, compareIsbn1> bookstore(compareIsbn1);
```

#### pair 类型
创建 pair 对象的函数
想象由一个函数需要返回一个pair。在新标准下，我们可以对返回值进行列表初始化：
```c++
pair<string,int> process(vector<string> &v)
{
	if (!v.empty()) {
		return {v.back(),v.back().size()}; //列表初始化
	}
	else
		return pair<string,int> ();//隐式构造函数
}
```
在较早的 c++ 版本中，不允许用花括号包围的初始化器来返回 pair 这种类型的对象，必须显式构造返回值：
```c++
if(!v.empty())
{
	return make_pair(v.back(),v.back().size());
	//or
	return pair<string,int>(v.back(),v.back().size());
}
```

### 关联容器操作
关联容器额外的类型别名：
* key_type 此容器类型的关键字类型
* mapped_type 每个关键字关联的类型；只适用于 map(unordered_map unordered_multimap multimap map)
* value_type 对于 set ，与 key_type 相同；对于 map ，为 pair<const key_type,mapped_type>

#### 关联容器迭代器

Note:`必须记住，一个 map 的 value_type 是一个 pair,我们可以改变 pair 的值，但不能改变关键字成员的值。`

set 的迭代器是 const的：
* 虽然 set 类型同时定义了 iterator 和 const_iterator 类型，但两种类型都只允许访问 set 中的元素。与不能改变一个 map 元素的关键字一样，一个 set 中的关键字也是 const 的。可以用一个 set 迭代器来读取元素的值，但不能修改。

关联容器和算法：
* 我们通常不对关联容器使用泛型算法。关键字 const 这一特性意味着不能将关联容器传递给修改或重排容器元素的算法，因为这类算法需要向元素写入值，而 set 类型中的元素是 const 的，map 中的元素是 pair，其第一个成员是 const 的。
* 关联容器可用于只读取元素的算法。但是，很多这类算法都要搜索序列。由于关联容器中的元素不能通过它们的关键字进行（快速）查找，因此对其使用泛型搜索算法几乎总是个坏主意。我们可以使用泛型 find 算法来查找一个元素，但此算法会进行顺序搜索。使用关联容器定义的专用的 find 成员会比调用泛型 find 快得多。
* 在实际编程中，如果我们真要对一个关联容器使用算法，要么是将它作为一个源序列，要么当作一个目的位置。

#### 添加元素

向 map 添加元素：
```c++
map<string,size_t> word_count;
// 向 word_count 插入 word 的 4 种方法
word_count.insert({word,1});
word_count.insert(make_pair(word,1));
word_count.insert(pair<string,size_t>(word,1));
word_count.insert(map<string,size_t>::value_type(word,1));
```
如我们所见，在新标准下，创建一个 pair 最简单的方法是在参数列表中使用花括号初始化。

关联容器还提供 `c.insert(p,v)、c.emplace(p,args) : 类似 insert(v)（或 emplace(args)）,但将迭代器 p 作为一个提示，指出从那里开始搜索新元素应该存储的位置。返回一个迭代器，指向具有给定关键字的元素 `

检测 inset 的返回值
insert(或 emplace) 返回的值依赖于容器类型和参数。对于不包含重复关键字的容器，添加单一元素的 insert 和 emplace 版本返回一个 pair，告诉我们插入操作是否成功。pair 的 first 成员是一个迭代器，指向具有给定关键字的元素；second 成员是一个 bool 值，指出元素是插入成功还是已经存在于容器中。如果关键字已在容器中，则 insert 什么事情也不做，且返回值中的 bool 部分为 false 。如果关键字不存在，元素被插入容器中，且 bool 值为 true。
eg:
```c++
map<string,size_t> word_count;
string word;
while (cin>>word) {
	//插入一个元素，关键字等于 word，值为 1；
	//若 word 已经在 word_count 中， insert 什么也不做
	auto ret=word_count.insert({word,1});
	if (!ret.second) { // word 已经在 word_count 中
		++ret.first->second; //递增计数器
	}
}
```

#### 删除元素
* [std::map::erase](https://en.cppreference.com/w/cpp/container/map/erase)

从关联容器删除元素：
* c.erase(k) 从 c 中删除每个关键字为 k 的元素，返回一个 size_type 值，指出删除的元素的数量
* c.erase(p) 从 c 中删除迭代器 p 指定的元素，p 必须指向 c 中一个真实元素， 不能等于 c.end() 。返回一个指向 p 之后元素的迭代器，若 p 指向 c 中的尾元素，则返回 c.end()
* c.erase(b,e) 删除迭代器对 b 和 e 所表示的范围中的元素。返回 e

#### map 的下标操作
map 和 unordered_map 容器提供了下标运算符和一个对应的 at 函数。set 类型不支持下标，因为 set 中没有与关键字相关联的“值”。元素本身就是关键字。因此“获取与一个关键字相关联的值”的操作就没有意义了。我们不能对一个 multimap 或一个 unordered_multimap 进行下标操作，因为这些容器中可能有多个值与一个关键字相关联。

类似我们用过的其他下标运算符，map下标运算符接受一个索引，获取与此关键字相关联的值。但是，与其它下标运算符不同的是，如果关键字并不在 map 中，会为它创建一个元素并插入到 map 中，关联值将进行值初始化。

eg:
```c++
map<string,size_t> word_count; //empty map
// 插入一个关键字为 Anna 的元素，关联值进行值初始化；然后将 1 赋予它
word_count["Anna"]=1;
```

由于下标运算符可能插入一个新元素，我们只可以对非 const 的 map 使用下标操作。
Note:`对一个map 使用下标操作，其行为与数组或 vector 上的下标操作很不相同：使用一个不再容器中的关键字作为下标，会添加一个具有此关键字的元素到 map 中。`

Note:`与 vector 与 string 不同，map 的下标运算符返回的类型与解引用 map 迭代器得到的类型不同。`

如果关键字还未在 map 中，下标运算符会添加一个新元素，这一特性允许我们编写出异常简洁的程序。另一方面，有是只是想直到一个元素是否在 map 中，但在不存在时并不想添加元素。在这种情况下，就不能使用下标运算符。

#### 访问元素

在一个关联容器中查找元素的操作：
* c.find(k) 返回一个迭代器，指向第一个关键字为 k 的元素，若k 不在容器中，则返回尾后迭代器
* c.count(k) 返回关键字等于 k 的元素的数量。对于不允许重复关键字的容器，返回值永远是 0 或 1
* c.lower_bound(k) 返回一个迭代器，指向第一个关键字不小于 k 的元素
* c.upper_bound(k) 返回一个迭代器，指向第一个关键字大于 k 的元素
* c.equal_range(k) 返回一个迭代器 pair,表示关键字等于 k 的元素的范围，若 k 不存在，pair 的两个成员均等于 c.end()

注意： lower_bound 和 upper_bound 不适用于无序容器。下标和 at 操作只适用与非 const 的 map 和  unordered_map。

在 multimap 或 multiset 中查找元素：
对于允许重复关键字的容器来说，查找一个元素要更为复杂：在容器中可能有很多元素具有给定的关键字。如果一个 multimap 或 multiset 中有多个元素具有给定关键字，则这些元素在容器中会相邻存储。

例如，给定一个从作者到著作题目的影射，我们可能想打印一个特定作者的所有著作。可以用三种不同方法来解决这个问题。最直观的方法是使用 find 和 count：
```c++
string search_item("Alain de Botton"); //要查找的作者
auto entries = authors.count(search_item);
auto iter=authors.find(search_item);

while (entries) {
	std::cout << iter->second << '\n';
	++iter;
	--entries;
}
```
Note:`当我们遍历一个 multimap 或 multiset 时，保证可以得到序列中所有具有给定关键字的元素。`

使用 lower_bound 和 upper_bound 操作，我们可以重写前面的程序：
```c++
// authors 和 search_item 的定义，与前面的程序一样
// beg 和 end 表示对应此作者的元素的范围

for (auto beg =authors.lower_bound(search_item),
	end = authors.upper_bound(search_item);
	beg != end; ++beg)
	{
		std::cout << beg->second << '\n';
	}
```

Note:`lower_bound 返回的迭代器可能指向一个具有给定关键字的元素，但也可能不指向。如果关键字不在容器中，则 lower_bound 会返回关键字的第一个安全插入点-不影响容器中元素顺序的插入位置。 如果 lower_bound 和 upper_bound 返回相同的迭代器，则给定关键字不在容器中`

我们也可以直接调用 equal_range 函数,我们可以重写之前的程序：
```c++
// authors 和 search_item 的定义，与前面的程序一样
// pos 保存迭代器对，表示与关键字匹配的元素范围

for(auto pos=authors.equal_range(search_item);
	pos.first != pos.second;++pos.first)
	{
		std::cout << pos.first->second << '\n';
	}
```

### 无序容器
新标准定义了 4 个无序关联容器(unordered associative container)。这些容器不是使用比较运算符来组织元素，而是使用一个哈希函数(hash function) 和关键字类型的 == 运算符。在关键字类型的元素没有明显的序关系的情况下，无序容器是非常有用的。在某些应用中，维护元素的序代价非常高昂，此时无序容器也很有用。

虽然理论上哈希技术能获得更好的平均性能，但在实际中想要达到很好的效果还需要进行一些性能测试和调优工作。因此，使用无序容器通常通常更为简单（通常也会有更好的性能）。

Tips:`如果关键字类型固有就是无序的，或者性能测试发现问题可以用哈希技术解决，就可以使用无序容器。`

管理桶：
* 无序容器在存储上组织为一组桶，每个桶保存 0 个或多个元素。无序容器使用一个哈希函数将元素映射到桶。为了访问一个元素，容器首先计算元素的哈希值，它指出应该搜索哪个桶。`容器将具有一个特定哈希值的所有元素保存在相同的桶中。`如果容器允许重复关键字，所有具有相同关键字的元素也都会在同一个桶中。因此，无序容器的性能依赖于哈希函数的质量和桶的数量和大小。
* 对于相同的参数，哈希函数必须总是产生相同的结果。理想情况下，哈希函数还能将每个特定值映射到唯一的桶。但是，将不同关键字的元素映射到相同的桶也是允许的。当一个桶保存多个元素时，需要顺序搜索这些元素来查找我们想要的那个。计算一个元素的哈希值和在桶中搜索通常都是很快的操作。但是，如果一个桶中保存了很多元素，那么查找一个特定元素就需要大量比较操作。

无序容器提供了一组管理桶的函数。这些成员函数允许我们查询容器的状态以及在必要时强制容器进行重组。

无序容器管理操作：
* 桶接口
	* c.bucket_count 正在使用的桶的数目
	* c.max_bucket_count()  容器能容纳的最多的桶的数量
	* c.bucket_size(n) 第n个桶中有多少个元素
	* c.bucket(k) 关键字为 k 的元素在哪个桶中
* 桶迭代
	* local_iterator 可以用来访问桶中元素的迭代器类型
	* const_local_iterator 桶迭代器的 const 版本
	* c.begin(n),c.end(n) 桶 n 的首元素迭代器和尾后迭代器
	* c.cbegin(n),c.cend(n) 与前两个函数类似，但返回 const_local_iterator
* 哈希策略
 	* c.load_factor() 每个桶的平均元素数量，返回 float 值
	* c.max_load_factor() c 试图维护的平均桶大小，返回 float值。c 会在需要时添加新的桶，以使得 load_factor<=max_load_factor
	* c.rehash(n) 重组存储，使得 bucket_count>=n 且 bucket_count > size/max_load_factor
	* c.reserve(n) 重组存储，使得 c 可以保存 n 个元素且不必 rehash

无序容器对关键字类型的要求
默认情况下，无序容器使用关键字类型的 == 运算符来比较元素，它们还使用一个 hash<key_type> 类型的对象来生成每个元素的哈希值。STL 为内置类型（包括指针）提供了 [std::hash](https://en.cppreference.com/w/cpp/utility/hash) 模板。还为一些 STL 类型，包括 string 和智能指针类型定义了 hash。因此，我们可以直接定义关键字是内置类型（包括指针类型）、string还是智能指针类型的无序容器。

但是，我们不能直接定义关键字类型为自定义类类型的无序容器。与容器不同，不能直接使用哈希模板，而必须提供我们自己的 hash 模板版本。我们将在 16.4 中介绍如何做到这一点。

eg:
```c++
// 提供 == 运算符 和 哈希函数
size_t hasher(const Sales_data &sd)
{
	return hash<string>() (sd.isbn());
}
bool eqOp(const Sales_data& lhs, const Sales_data& rhs)
{
	return lhs.isbn() == rhs.isbn();
}

using SD_multiset=unordered_multiset<Sales_data,decltype(hasher) *,decltype(eqOp) *>;
SD_multiset bookstore(42,hasher,eqOp);
```

## 动态内存
除了静态内存和栈内存，每个程序还拥有一个内存池。这部分内存被称作自由空间(free store)或堆(heap)。程序使用堆来存储动态分配(dynamically allocate)的对象-即，那些在程序运行时分配的对象。动态对象的生存期由程序来控制，也就是说，当动态对象不再使用时，我们的代码必须显式地销毁它们。

Warning:`虽然使用动态内存有时是必要的，但众所周知，正确地管理动态内存是非常棘手的。`

### 动态内存与智能指针
为了更容易（同时也更安全）地使用动态内存，新的标准库提供了两种智能指针类型来管理动态对象。新标准提供的这两种智能指针的区别在于管理底层指针的方式：shared_ptr 允许多个指针指向同一个对象；unique_ptr 则“独占”所指向的对象。STL 还定义了一个名为 weak_ptr 的伴随类，它是一种弱引用，指向 shared_ptr 所管理的对象。这三种类型都定义在 memory 头文件中。

#### shared_ptr 类
* [std::shared_ptr](https://en.cppreference.com/w/cpp/memory/shared_ptr)

shared_ptr 和 unique_ptr 都支持的操作：
* shared_ptr<T> sp ， unique_ptr<T> up ：空智能指针，可以指向类型为 T 的对象
* p ：将 p 用作一个条件判断，若 p 指向一个对象，则为 true
* \* p : 解引用 p，获得它所指向的对象
* p->mem :等价于 (* p).mem
* p.get() ： 返回 p 中保存的指针，要小心使用，若智能指针释放了其对象，返回的指针所指向的对象也就消失了
* swap(p,q)， p.swap(q) : 交换 p 和 q 的指针

shared_ptr 独有的操作：
* make_shared<T>(args) : 返回一个 shared_ptr,指向一个动态分配的类型为 T 的对象。使用 args 初始化此对象
* shared_ptr<T>p(q) ：p 是 shared_ptr q 的拷贝；此操作会递增 q 中的计数器。q 中的指针必须能转换为 T*
* p=q : p 和 q 都是 shared_ptr,所保存的指针必须能相互转换。此操作会递减 p 的引用计数，递增 q 的引用计数；若 p 的引用计数变为 0，则将其管理的原内存释放
* p.unique() : 若 p.use_count() 为 1，返回 true；否则返回 false
* p.use_count() ：返回与 p 共享对象的智能指针数量；可能很慢，主要用于调试

make_shared 函数：
```c++
shared_ptr<int> p=make_shared<int>();
// 如果我们不传递任何参数，对象就会进行值初始化
```

shared_ptr 的拷贝和赋值：
当进行拷贝或赋值操作时，每个 shared_ptr 都会记录有多少个其他 shared_ptr 指向相同的对象：
```c++
auto p = make_shared<int>(42); // p 指向的对象只有只有 p 一个引用者
auto q(p); //p 和 q 指向相同对象，此对象有两个引用者
```
我们可以认为每个 shared_ptr 都有一个关联的计数器，通常称其为引用计数（refefence count）。无论何时我们拷贝一个 shared_ptr ,计数器都会递增。当我们给 shared_ptr 赋予一个新值或是 shared_ptr 被销毁（例如一个局部的 shared_ptr 离开其作用域）时，计数器就会递减。
一旦一个 shared_ptr 的计数器变为 0，它就会自动释放自己所管理的对象：
```c++
auto r = make_shared<int>(42); //r 指向的 int 只有一个引用者
r = q； //给 r 赋值，令他指向另一个地址，递增 q 指向的对象的引用计数，递减 r 原来指向的对象的引用计数，r 原来指向的对象已没有引用者，会自动释放
```
此例中我们分配了一个 int，将其指针保存在 r 中。接下来，我们将一个新值赋予 r。在此情况下，r 是唯一指向此 int 的 shared_ptr，在把 q 赋值给 r 的过程中，此 int 被自动释放。

Note:`到底是用一个计数器还是其他数据结构来记录有多少指针共享对象，完全由 STL 的具体实现来决定。关键是智能指针类能记录有多少个 shared_ptr 指向相同的对象，并能在恰当的时候自动释放对象。`

Note:`如果你将 shared_ptr 存放于一个容器中，而后不再需要全部元素，而只使用其中一部分，要记得用 erase 删除不再需要的那些元素。`

使用了动态生存期的资源的类:

程序使用动态内存出于以下三种原因之一：
1. 程序不知道自己需要使用多少对象
2. 程序不知道所需对象的准确类型
3. 程序需要在多个对象间共享数据

容器类是出于第一种原因而使用动态内存的典型例子。一般而言，如果两个对象共享底层的数据，当某个对象被销毁时，我们不能单方面地销毁底层数据：
```c++
Blob<string> b1; // 空 Blob
{ // 新作用域
	Blob<string> b2={"a","an","the"};
	b1 = b2; //b1 和 b2 共享相同的元素
} // b2 被销毁了，但 b2 中的元素不能销毁
// b1 指向最初由 b2 创建的元素
```

定义 StrBlob 类
```c++
#include <string>
#include <vector>
#include <iostream>
#include <memory>

using namespace std;

class StrBlob{
public:
	typedef std::vector<std::string>::size_type size_type;
	StrBlob();
	StrBlob(std::initializer_list<std::string> il);
	size_type size() const {return data->size();}
  bool empty() const {return data->empty();}

	//添加和删除元素
	void push_back(const std::string &t)
	{
		data->push_back(t);
	}
	void pop_back();
	//元素访问
	std::string& front();b,c}; //错误：括号中只能有单个初始化器
	std::string& back();
private:
	std::shared_ptr<std::vector<std::string>> data;
	//如果 data[i]不合法，抛出一个异常
	void check(size_type i,const std::string &msg) const;
};

StrBlob::StrBlob():data(make_shared<vector<string>>()){}
StrBlob::StrBlob(initializer_list<string> il):data(make_shared<vector<string>>(il)){}

void StrBlob::check(size_type i,const string &msg) const
{
	if(i >= data->size())
		throw out_of_range(msg);
}

string& StrBlob::front()
{
	check(0,"front on empty StrBlob");
	return data->front();
}

string& StrBlob::back()
{
	check(0,"back on empty StrBlob");
	return data->back();
}

void StrBlob::pop_back()
{
	check(0,"pop_back on empty StrBlob");
	data->pop_back();
}

// StrBlob 使用默认版本的拷贝、赋值和销毁成员函数来对此类型的对象进行这些操作。

int main(int argc, char const * argv[]) {
  StrBlob b1;
  {
    StrBlob b2={"a","an","the"};
    b1=b2;
    b2.push_back("about");
    std::cout << "b1 size:"<< b1.size() << '\n';
    std::cout << "b2 size:"<< b2.size() << '\n';
  }
  return 0;
}
```
Run it:
```sh
b1 size:4
b2 size:4
```

#### 直接管理内存
c++ 语言定义了两个运算符来分配和释放动态内存。运算符 new 分配内存，delete 释放 new 分配的内存。
相对于智能指针，使用这两个运算符管理内存非常容易出错。

使用 new 动态分配和初始化对象
在自由空间分配的内存时无名的，因此 new 无法为其分配的对象命名，而是返回一个指向该对象的指针。默认情况下，动态分配的对象时默认初始化的，这意味着内置类型或组合类型的对象的值将是未定义的，而类类型将用默认构造函数进行初始化。

Best practices:`由于与变量初始化相同的原因，对动态分配的对象进行初始化通常是个好主意。`

如果我们提供了一个括号包围额初始化器，就可以使用 auto 从此初始化器来推断我们想要分配的对象的类型。但是，由于编译器要用初始化器的类型来推断要分配的类型，只有当括号中仅有单一初始化器才可以使用 auto:

```c++
auto p1 = new auto(obj); //p 指向一个与 obj 类型相同的对象，该对象用 obj 进行初始化
auto p2 = new auto{a,b,c}; //错误：括号中只能有单个初始化器
```

动态分配的 const 对象：
用 new 分配 const 对象是合法的：
```c++
const int * pci = new const int(1024);
const string * pcs = new const string;
```

内存耗尽：
虽然现代计算机通常都配备大容量内存，但是自由空间被耗尽的情况还是有可能发生。一旦一个程序用光了它所可用的的内存， new 表达式就会失败。默认情况下，如果 new 不能分配所要求的内存空间，它会抛出一个类型为 bad_alloc 的异常。我们可以改变使用 new 的方式来阻止它抛出异常：
```c++
//如果分配失败，new 返回一个空指针
int * p1 = new int; // 如果分配失败，new 抛出 bad_alloc
int * p2 = new (nothrow) int; //如果分配失败，new 返回一个空指针
```
我们称这种形式的 new 为 定位new(placement new)，定位 new 表达式允许我们向 new 传递额外的参数。在此例中，我们传递给它由 STL 定义的名为 nothrow 的对象。

指针值和 delete
我们传递给 delete 的指针必须指向动态分配的内存，或者是一个空指针。释放一块并非 new 分配的内存，或者将相同的指针值释放多次，其行为是未定义的：
```c++
int i,* pi1=&i, * pi2=nullptr;
double * pd=new double(33),* pd2=pd;
delete i; // 错误，i 不是指针
delete pi1; //未定义：pi1 指向一个局部变量
delete pd; // 正确
delete pd2; // 未定义： pd2 指向的内存已经被释放了
delete pi2; // 正确：释放一个空指针总是没有错误的
```

Warning:`由内置指针（而不是智能指针）管理的动态内存在被显式释放前一直都会存在。`

小心：动态内存的管理非常容易出错：
`使用 new 和 delete 管理动态内存存在三个常见的问题：1. 忘记delete 内存；2. 使用已经释放掉的对象；3. 同一块内存释放两次`

delete 之后重置指针值 ......
当我们 delete 一个指针后，指针值就变得无效了。虽然指针已经无效，但在很多机器上指针仍旧保存着（已经释放了的）动态内存的地址。在 delete 之后，指针就变成人们所说的 空悬指针(dangling pointer),即，指向一块曾经保存数据对象但现在已经无效的内存的指针。
未初始化指针的所有缺点空悬指针也都有。

...... 这只是提供了有限的保护
动态内存的一个基本问题是可能有多个指针指向相同的内存。在 delete 内存之后重置指针的方法只对这个指针有效，对其他任何指向（已释放的）内存的指针是没有作用的。例如：
```c++
int *p (new int(42));
auto q=p;
delete p;
p=nullptr;
```
但是，重置 p 对 q 没有任何作用，在我们释放 p 所指向的内存时，q 也变为无效了。在实际系统中，查找指向相同内存的所有指针是异常困难的。

#### shared_ptr 和 new 结合使用
shared_ptr 接受指针参数的智能指针的构造函数是 explicit 的。因此，我们不能将一个内置指针隐式转换为一个智能指针，必须使用直接初始化形式来初始化一个智能指针：
```c++
shared_ptr<int> p1=new int(1024); //error
shared_ptr<int> p2(new int(1024)); //正确

// 一个返回 shared_ptr 的函数不能在其返回语句中隐式转换一个普通指针：
shared_ptr<int> clone(int p)
{
	return new int(p); //error
}
//我们必须将 shared_ptr 显式绑定到一个想要返回的指针上：
shared_ptr<int> clone(int p)
{
	return shared_ptr(new int(p));
}
```

定义和改变 shared_ptr 的其他方法：
* shared_ptr<T> p(q): p 管理内置指针 q 所指向的对象；q 必须指向 new 分配的内存，且能够转换为 T* 类型
* shared_ptr<T> p(u): p 从 unique_ptr u 那里接管了对象的所有权；将 u 置为空
* shared_ptr<T> p(q,d):p 接管了内置指针 q 所指向的对象的所有权；q 必须能转换为 T* 类型。p 将使用可调用对象 d 来代替 delete
* shared_ptr<T> p(p2,d): p 是 shared_ptr p2 的拷贝，唯一的区别是 p 将用可调用对象 d 来代替 delete
* p.reset()，p.reset(q)，p.reset(q,d): 若 p 是唯一指向其对象的 shared_ptr,reset 会释放此对象。若传递了可选的参数内置指针 q，会令 p 指向 q，否则会将 p 置为空。若还传递了参数 d ，将会调用 d 而不是 delete 来释放 q

不要混合使用普通指针和智能指针 ......
```c++
void process(shared_ptr<int> ptr)
{
	//use ptr
} // ptr 离开作用域，被销毁

//正确使用
shared_ptr<int> p(new int(42)); //引用计数为 1
process(p); // 拷贝会递增它的引用计数；在 process 中引用计数为 2
int i = *p; //正确，引用计数值为 1

//错误的使用
int * x(new int(42)); // 危险，x 是一个普通指针，而不是一个智能指针
process(x); //错误：不能将 int * 转换为一个 shared_ptr<int>
process(shared_ptr<int>(x)); // 合法的，但内存会被释放
int j=* x;// 未定义的: x 是一个空悬指针
```

...... 也不要使用 get 初始化另一个智能指针或为智能指针赋值

```c++
shared_ptr<int> p(new int(42));
int * q=p.get();
{
	shared_ptr<int>(q);
}// 程序块结束，q 被销毁，它指向的内存被释放
int foo = * p; // 未定义： p 指向的内存已经被释放了
```
Warning:`get 用来将指针的访问权限传递给代码，你只有在确定代码不会 delete 指针的情况下，才能使用 get。特别是，永远不要用 get 初始化另一个智能指针或为另一个智能指针赋值`

其他 shared_ptr 操作：
我们可以用 reset 来将一个新的指针赋予一个 shared_ptr:
```c++
p = new int(1024);  //错误：不能将一个指针赋予 shared_ptr
p.reset(new int(1024)); // 正确：p指向一个新对象
```
与赋值类似，reset 会更新引用计数，如果需要的话，会释放 p 指向的对象。reset 成员经常与 unique 一起使用，来控制多个 shared_ptr 共享的对象。在改变底层对象之前，我们检查自己是否是当前对象仅有的用户。如果不是，在改变之前要制作一份新的拷贝：
```c++
if (!p.unique()) {
	p.reset(new string(*p));// 我们不是唯一用户；分配新的拷贝
}
* p+=newVal; //现在我们知道自己是唯一的用户，可以改变对象的值
```

#### 智能指针和异常
```c++
void f()
{
	shared_ptr<int> sp(new int(42)); //分配一个新对象
	//这段代码抛出一个异常，且在 f 中未被捕获
} // 在函数结束时 shared_ptr 自动释放内存
```

智能指针和哑类：
与管理动态内存类似，我们通常可以使用类似的技术用来管理不具有良好定义的析构函数的类。例如，假定我们正在使用一个 c 和 c++ 都使用的网络库，使用这个库的代码可能是这样的：
```c++
struct destination; // 表示我们正在连接什么
struct connection; // 使用连接所需的信息
connection connect(destination *); // 打开连接
void disconnect(connection); //关闭给定的连接
void f(destination &d /* 其他参数 */)
{
	// 获得一个连接；记住使用完后要关闭它
	connection c= connect(&d);
	// 使用连接
	// 如果我们在 f 退出前忘记调用 disconnect,就无法关闭 c 了
}
```

使用我们自己的释放操作：
```c++
void end_connection(connection * p)
{
	disconnect(* p);
}

void f(destination &d /* 其他参数 */)
{
	connection c= connect(&d);
	shared_ptr<connection> p(&c,end_connection);
	// 使用连接
	// 当 f 退出时（即使是由于异常而退出），connection 会被正确关闭
}

// 改为使用 lambda ，可以这样写
shared_ptr<connection> p(&c,[](connection * ptr){
	disconnect(* ptr);
});
```

注意：智能指针陷阱：
智能指针可以提供对动态分配的内存安全而又方便的管理，但这建立在正确使用前提下。为了正确使用智能指针，我们必须坚持一些基本规范：
* 不使用相同的内置指针值初始化（或 reset）多个智能指针。
* 不 delete get（） 返回的指针。
* 不使用 get() 初始化或 reset 另一个指针
* 如果你使用 get() 返回的指针，记住当最后一个对应的智能指针销毁后，你的指针就变为无效了。
* 如果你使用智能指针管理的资源不是 new 分配的内存，记住传递给它一个删除器。

#### unique_ptr
一个 unique_ptr "拥有" 它所指向的对象。与 shared_ptr 不同，某个时刻只能有一个 unique_ptr 指向一个给定对象。当 unique_ptr 被销毁时，它所指向的对象也被销毁。
与 shared_ptr 不同，没有类似 make_shared 的标准库函数返回一个 unique_ptr。当我们定义一个 unique_ptr 时，需要将其绑定到一个 new 返回的指针上。类似 shared_ptr ,初始化 unique_ptr 必须采用直接初始化形式：
```c++
unique_ptr<double> p1;
unique_ptr<int> p2(new int(42));
// 由于一个 unique_ptr 拥有它指向的对象，因此 unique_ptr 不支持普通的拷贝或赋值操作：

unique_ptr<string> p1(new string("Stegosaurus"));
unique_ptr<string> p2(p1); //error
unique_ptr<string> p3;
p3=p2; //error
```

unique_ptr 操作：
* unique_ptr<T> u1 : 空 unique_ptr,可以指向类型为 T 的对象。u1 会使用 delete 来释放它的指针；
* unique_ptr<T,D> u2 :  u2 会使用一个类型为 D 的可调用对象来释放它的指针
* unique_ptr<T,D> u(d) : 空 unique_ptr,指向类型为 T 的对象，用类型为 D 的对象 d 代替 delete
* u =nullptr: 释放 u 指向的对象，将 u 置为空
* u.release() : u 放弃对指针的控制权，返回指针，并将 u 置为空
* u.reset() : 释放 u 指向的对象
* u.reset(q),u.reset(nullptr) :如果提供了内置指针 q ，令 u 指向这个对象；否则将 u 置为空

我们可以通过调用 release 或 reset 将指针的所有权从一个（非 const） unique_ptr 转移给另一个 unique_ptr：
```c++
unique_ptr<string> p2(p1.release()); // release 将 p1  置为空
unique_ptr<string> p3(new string("Trex"));
p2.reset(p3.release()); //reset 释放了 p2 原来指向的内存
```

调用 release 会切断 unique_ptr 和它原来管理的对象间的联系。在本例中，管理内存的责任简单地从一个智能指针转移给另一个。但是，如果我们不用另一个智能指针来保存 release 返回的指针，我们的程序就要负责资源的释放：
```c++
p2.release(); //错误： p2 不会释放内存，而且我们丢失了指针
auto p = p2.release(); // 正确，但我们必须记得 delete(p)
```

传递 unique_ptr 参数和返回 unique_ptr:
不能拷贝 unique_ptr 的规则有一个例外：我们可以拷贝或赋值一个将要被销毁的 unique_ptr。最常见的例子是从函数返回一个 unique_ptr:
```c++
unique_ptr<int> clone(int p)
{
	return unique_ptr<int>(new int(p));
}
```
还可以返回一个局部对象的拷贝：
```c++
unique_ptr<int> clone(int p)
{
	unique_ptr<int> ret(new int(p));
	// ...
	return ret;
}
```
对于两段代码，编译器都知道要返回的对象将要被销毁。在此情况下，编译器执行一种特殊的“拷贝”。

向后兼容： auto_ptr
`STL 的较早版本包含了一个名为 auto_ptr 的类，它具有 unique_ptr 的部分特性，但不是全部。特别是，我们不能在容器中保存 auto_ptr,也不能从函数中返回 auto_ptr。
虽然 auto_ptr 仍然是标准库中的一部分，但编写程序应该使用 unique_ptr。`

向 unique_ptr 传递删除器：
unique_ptr 管理删除器的方式与 shared_ptr 不同。重载一个 unique_ptr 中的删除器会影响到 unique_ptr 类型以及如何构造（或 reset）该类型的对象。与重载关联容器的比较操作类似，我们必须在尖括号中 unique_ptr 指向类型之后提供删除器类型。在创建或reset一个这种 unique_ptr 类型的对象时，必须提供一个指定类型的可调用对象（删除器）：
```c++
unique_ptr<objT,delT> p(new objT,fcn);
```
我们重写连接程序，用 unique_ptr 代替 shared_ptr:
```c++
void f(destination &d /* 其他参数 */)
{
	connection c= connect(&d);
	unique_ptr<connection,decltype(end_connection)* > p(&c,end_connection);
	// 使用连接
	// 当 f 退出时（即使是由于异常而退出），connection 会被正确关闭
}
```

#### weak_ptr
weak_ptr 是一种不控制所指向对象生存期的智能指针，它指向由一个 shared_ptr 管理的对象。将一个 weak_ptr 绑定到一个 shared_ptr 不会改变 shared_ptr 的引用计数。一旦最后一个指向对象的 shared_ptr 被销毁，对象就会被销毁。即使有 weak_ptr 指向对象，对象也还是会被释放，因此， weak_ptr 的名字抓住了这种智能指针“弱”共享对象的特点。

weak_ptr:
* weak_ptr<T> w : 空 weak_ptr
* weak_ptr<T> w(sp) : 与 shared_ptr sp 指向相同对象的 weak_ptr。 T 必须能转换为 sp 指向的类型
* w = p : p 可以是一个 shared_ptr 或一个 weak_ptr。赋值后 w 与 p 共享对象
* w.reset() : 将 w 置为空
* w.use_count() ：与 w 共享对象的 shared_ptr 的数量
* w.expired() : 若 w.use_count 为 0，返回 true,否则返回 false
* w.lock() : 如果 expired 为 true，返回一个空 shared_ptr；否则返回一个指向 w 的对象的 shared_ptr

当我们创建一个 weak_ptr 时，要用一个 shared_ptr 来初始化它：
```c++
auto p=make_shared<int>(42);
weak_ptr<int> wp(p); //wp 弱共享p； p 的引用计数未改变
```
由于对象可能不存在，我们不能使用 weak_ptr 直接访问对象，而必须调用 lock。此函数检查 weak_ptr 指向的对象是否存在。与其他 shared_ptr 类似，只要此 shared_ptr 存在，它所指向的底层对象也就会一直存在。例如：
```c++
if (shared_ptr<int> np = wp.lock()) { //如果 np 不为空则条件成立
	// 在 if 中 ，np 与 p 共享对象
}
```

StrBlobPtr.h:
```c++
#include <string>
#include <vector>
#include <iostream>
#include <memory>

class StrBlobPtr; //对于 StrBlob 中的友元声明来说，此前置声明是必要的
class StrBlob{
public:
	friend class StrBlobPtr;
	StrBlobPtr begin();
	StrBlobPtr end();
public:
	typedef std::vector<std::string>::size_type size_type;
	StrBlob();
	StrBlob(std::initializer_list<std::string> il);
	size_type size() const {return data->size();}
  bool empty() const {return data->empty();}

	//添加和删除元素
	void push_back(const std::string &t)
	{
		data->push_back(t);
	}
	void pop_back();
	//元素访问
	std::string& front();
	std::string& back();
private:
	std::shared_ptr<std::vector<std::string>> data;
	//如果 data[i]不合法，抛出一个异常
	void check(size_type i,const std::string &msg) const;
};

class StrBlobPtr{
public:
  StrBlobPtr():curr(0) {}
  StrBlobPtr(StrBlob &a,size_t sz=0):
  wptr(a.data),curr(sz){}
  std::string& deref() const;
  StrBlobPtr& incr(); //前缀递增
	bool operator!=(const StrBlobPtr& p)
	{ return p.curr != curr; }
private:
  std::shared_ptr<std::vector<std::string>>
  check(std::size_t i,const std::string& msg) const ;
  std::weak_ptr<std::vector<std::string>> wptr;
  std::size_t curr;
};
```

StrBlobPtr.cpp:
```c++
#include "StrBlobPtr.h"

using namespace std;

std::shared_ptr<std::vector<std::string>>
StrBlobPtr::check(std::size_t i,const std::string& msg) const
{
  auto ret = wptr.lock();
  if (!ret) {
    throw std::runtime_error("unbound StrBlobPtr");
  }
  if (i>= ret->size()) {
    throw std::out_of_range(msg);
  }
  return ret;
}

std::string& StrBlobPtr::deref() const
{
  auto p =check(curr,"dereference past end");
  return (* p)[curr]; //(* p) 是对象所指向的 vector
}

StrBlobPtr& StrBlobPtr::incr()
{
  check(curr,"increment past end of StrBlobPtr");
  ++curr;
  return * this;
}

StrBlobPtr StrBlob::begin()
{
	return StrBlobPtr(* this);
}
StrBlobPtr StrBlob::end()
{
	auto ret = StrBlobPtr(* this,data->size());
	return ret;
}

StrBlob::StrBlob():data(make_shared<vector<string>>()){}
StrBlob::StrBlob(initializer_list<string> il):data(make_shared<vector<string>>(il)){}

void StrBlob::check(size_type i,const string &msg) const
{
	if(i >= data->size())
		throw out_of_range(msg);
}

string& StrBlob::front()
{
	check(0,"front on empty StrBlob");
	return data->front();
}

string& StrBlob::back()
{
	check(0,"back on empty StrBlob");
	return data->back();
}

void StrBlob::pop_back()
{
	check(0,"pop_back on empty StrBlob");
	data->pop_back();
}

int main(int argc, char const *argv[]) {
  StrBlob b1;
  {
    StrBlob b2={"a","an","the"};
    b1=b2;
    b2.push_back("about");
    std::cout << "b1 size:"<< b1.size() << '\n';
    std::cout << "b2 size:"<< b2.size() << '\n';
  }

	for(auto i(b1.begin()),e(b1.end());i!=e;i.incr())
	{
		std::cout << "val:"<< i.deref() << '\n';
	}
  return 0;
}
```
Run it:
```sh
b1 size:4
b2 size:4
val:a
val:an
val:the
val:about
```

### 动态数组
STL 中包含一个名为 allocator 的类，允许我们将分配和初始化分离。使用 allocate 通常会提供更好的性能和更灵活的内存管理能力。

#### new 和数组
```c++
int * pia = new int[get_size()]; //pia 指向第一个 int
```
方括号中的大小必须是整形，但不必是常量。
还可以这样：
```c++
typedef int arrT[42];
int * p = new arrT; //分配一个 42 个 int 的数组； p 指向第一个 int
// 编译器执行如下形式：
// int * p = new int[42];
```

分配一个数组会得到一个元素类型的指针：
虽然我们通常称 new T[] 分配的内存为“动态数组”，但这种叫法某种程度上有些误导。当用 new 分配一个数组时，我们并未得到一个数组类型的对象，而是得到一个数组元素类型的指针。即使我们使用类型别名定义了数组类型，new 也不会分配一个数组类型的对象。在上例中，我们正在分配一个数组的事实甚至都是不可见的  -- 连 [num] 都没有。new 返回的是一个元素类型的指针。
由于分配的内存并不是一个数组类型，因此不能对动态数组调用 begin 或 end。这些函数使用数组维度来返回指向首元素和尾后元素的指针。出于相同的原因，也不能用范围 for 语句来处理（所谓的）动态数组中的元素。

Warning:`要记住我们所说的动态数组并不是数组类型，这点很重要的。`

初始化动态分配对象的数组：
默认情况下，new 分配的对象，不管是单个分配的还是数组中的，都是默认初始化的。可以对数组中的元素进行值初始化，方法是在大小之后跟一对空括号：
```c++
int * pia = new int[10]; //10 个未初始化的 int
int * pia2 = new int[10](); //10 个值初始化为 0 的 int
int * p = new int[1](6); // 错误：
```
当然在新标准中，我们还可以提供一个元素初始化器的花括号列表。

虽然我们用空括号对数组中元素进行值初始化，但不能在括号中给出初始化器，这意味着不能用 auto 分配数组。

动态分配一个空数组是合法的
```c++
size_t n = get_size();
int * p = new int[n];
for(int * q = p;q != p+n;++q)
// 处理数组
;
```
```c++
char arr[0]; //错误：不能定义长度为 0 的数组
char * cp = new char[0]; //正确：但 cp 不能解引用
```
当我们用 new 分配一个大小为 0 的数组时，new 返回一个合法的非空指针。此指针保证与 new 返回的其他任何指针都不相同。对于零长度的数组来说，此指针就像尾后指针一样，我们可以像使用尾后迭代器一样使用这个指针。

释放动态数组：
```c++
delete [] pa; // pa 必须指向一个动态分配的数组或为空
```
销毁 pa 指向的数组中的元素，并释放对应的内存。数组中的元素按逆序销毁，即最后一个元素首先被销毁，然后是倒数第二个，依次类推。
如果我们在 delete 一个指向数组的指针时忽略了方括号（或者在 delete 一个指向单一对象的指针时使用了方括号），其行为是未定义的。
```c++
typedef int arrT[42];
int * p = new arrT;
delete [] p;// 方括号是必须的
```

Warning:`如果我们在 delete 一个数组指针时忘记了方括号，或者在 delete 一个单一对象的指针时使用了方括号，编译器很可能不会给出警告。我们的程序可能在执行过程中在在没有任何警告的情况下行为异常。`

#### allocator 类
当分配一大块内存时，我们通常计划在这块内存上按需构造对象。在此情况下，我们希望将内存分配和对象构造分离。这意味着我们可以分配大块内存，但只在真正需要时才真正执行对象创建操作（同时付出一定开销）。
一般情况下，将内存和对象构造组合在一起可能会导致不必要的浪费。例如:
```c++
string * const p =new string[n];
string s;
string * q=p;
while (cin>>s && q!=p+n) {
	* q++=s;
}
const size_t size =q-p;
delete [] p;
```
new 表达式分配并初始化了 n 个 string 。但是，我们可能不需要 n 个 string，少量 string 可能就足够了。这样，我们就创建了一些永远也用不到的对象。

[std::allocator](https://en.cppreference.com/w/cpp/memory/allocator)，它帮助我们将内存分配和对象构造分离开来。它提供了一种类型感知的内存分配方法，它分配的内存是原始的、未构造的。

STL allocator 类及其算法：
* allocator<T> a 定义了一个名为 a 的 allocator 对象，它可以为类型 T 的对象分配内存
* a.allocate(n) 分配一段原始的、未构造的内存，保存 n 个类型为 T 的对象
* a.deallocate(p,n) 释放从 T* 指针 p 中地址开始的内存，这块内存保存了 n 个类型为 T 的对象；p 必须是一个先前由 allocate 返回的指针，且 n 必须是 p 创建时所要求的大小。在调用 deallocate 之前，用户必须对每个在这块内存中创建的对象调用 destroy
* a.construct(p,args) p 必须是一个类型为 T* 的指针，指向一块原始内存；arg 被传递给类型为 T 的构造函数，用来在 p 指向的内存中构造一个对象
* a.destroy(p) p 为 T* 类型的指针，此算法对 p 所指向的对象执行析构函数

```c++
allocator<string> alloc;
auto const p = alloc.allocate(n); //分配 n 个未初始化的 string
auto q =p;
alloc.construct(q++); //* q 为空字符串
alloc.construct(q++,10,'c'); //*q 为 cccccccccc
alloc.construct(q++,"hi"); //* q 为 hi
```
还未构造对象的情况下就使用原始内存是错误的：
```c++
cout<<* p<<endl; // 正确：使用string 的输出运算符
cout<<* q<<endl; // 灾难：q 指向未构造的内存
```

Warning:`为了使用 allocate 返回的内存，我们必须用 construct 构造对象。使用未构造的内存，其行为是未定义的。`

```c++
while (q!=p) {
	alloc.destroy(--q); //释放我们真正构造的 string
}
```
Warning:`我们只能对真正构造了的元素进行 destroy 操作。`

```c++
alloc.deallocate(p,n); //释放内存
```

拷贝和填充未初始化内存的算法：
* [std::uninitialized_copy](https://en.cppreference.com/w/cpp/memory/uninitialized_copy)
* [std::uninitialized_copy_n](https://en.cppreference.com/w/cpp/memory/uninitialized_copy_n)
* [std::uninitialized_fill](https://en.cppreference.com/w/cpp/memory/uninitialized_fill)
* [std::uninitialized_fill_n](https://en.cppreference.com/w/cpp/memory/uninitialized_fill_n)

## Link
* [Mooophy/Cpp-Primer](https://github.com/Mooophy/Cpp-Primer)

[上一级](base.md)
[下一篇](MFC_VS_QT.md)
