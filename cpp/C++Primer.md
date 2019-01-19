# 再探 c++ primer


<!-- @import "[TOC]" {cmd="toc" depthFrom=1 depthTo=6 orderedList=false} -->
<!-- code_chunk_output -->

* [再探 c++ primer](#再探-c-primer)
	* [变量和基本类型](#变量和基本类型)
		* [列表初始化](#列表初始化)
		* [默认初始化](#默认初始化)
		* [变量声明和定义的关系](#变量声明和定义的关系)
		* [引用](#引用)
		* [指针](#指针)
		* [const限定符](#const限定符)
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

<!-- /code_chunk_output -->

## 变量和基本类型

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

[上一级](base.md)
[下一篇](MFC_VS_QT.md)
