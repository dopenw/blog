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
	在下面这些情况下，编译器会自动地转换运算对象的类型：
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
		reinterpret_cast 本质上依赖于机器。要想安全地使用 reinterpret_cast ，必须对涉及的类型和编译器实现转换的过程都非常了解。

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
将在编译过程中展开成类似于下面地形式：
```c++
std::cout << (s1.size() <= s2.size() ? s1:s2) << '\n';
```
从而消除了 shorterString 函数地运行时开销。

Note:
* 内联说明只是向编译器发出一个请求，编译器可以选择忽略这个请求。

一般来说，内联机制用于优化规模较小、流程直接、频繁调用的函数。很多编译器都不支持内联递归函数，而且一个 75 行的函数也不大可能在调用点内联地展开。

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
对于某个给定地内联函数或者 constexpr函数来说，它的多个定义必须完全一致。基于这个原因，内联函数和 constexpr 函数通常定义在头文件中。

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
	* 实参从数组类型或函数类型转换成对应地指针类型
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
F *f1(int); //正确，显式地指定返回类型是指向函数地指针
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
只有当类没有声明任何构造函数时，编译器才会自动地生成默认构造函数。
```

```highLight
如果类包含有内置类型或者复合类型地成员，则只有当这些成员全都被赋予了类内的初始值时，这个类才适合于使用合成的默认构造函数。
```

* `=default` 的含义
我们定义这构造函数的目的仅仅是因为我们既需要其他形式的构造函数，也需要默认的构造函数。我们希望这个函数的作用完全等同于之前使用的合成默认构造函数。

[Default constructors](https://en.cppreference.com/w/cpp/language/default_constructor)

```highLight
如果你的编译器不支持类内初始值，那么你的默认构造函数就应该使用构造函数初始值列表来初始化类的每个成员。
```

构造函数初始值列表([constructor initialized list](https://en.cppreference.com/w/cpp/language/initializer_list))

目前所用的 gcc version 8.2.1 是支持 类内初始值  的,eg:
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

[上一级](base.md)
[下一篇](MFC_VS_QT.md)
