# const限定符


<!-- @import "[TOC]" {cmd="toc" depthFrom=1 depthTo=6 orderedList=false} -->
<!-- code_chunk_output -->

* [const限定符](#const限定符)
	* [const修饰的引用](#const修饰的引用)
	* [const修饰的变量](#const修饰的变量)
	* [const作用域](#const作用域)
	* [const成员函数](#const成员函数)
	* [const修饰指针](#const修饰指针)
	* [const数据成员的初始化](#const数据成员的初始化)

<!-- /code_chunk_output -->

## const修饰的引用
* 如果既要利用引用提高程序的效率，又要保护传递给函数的数据不在函数中被改变，就应使用const修饰

```c++
#include <iostream>

using namespace std;

int main(int argc, char const *argv[]) {
  int a;
  const int &ra = a;
  // ra = 1; error
  a = 1;
  std::cout << ra << '\n';
  return 0;
}
```

output:
```
1
```


## const修饰的变量

* const修饰的变量表示只读，且必须初始化

```c++
#include <iostream>

using namespace std;

int main(int argc, char const *argv[]) {
  const int a = 1;
  // int &ra = a; error
  const int &ra = a;
  // a = 1; error
  std::cout << ra << '\n';
  return 0;
}
```

output:
```
1
```

## const作用域
1. 在c++中，默认状态下，const对象仅在文件内有效

2. C默认const是外部连接的，C++默认const是内部连接的。如果想在C++中完成与C同样的事：实现多个文件之间共享const对象，必须在变量定义之前添加extern关键字


## const成员函数
我们定义的类的成员函数中，常常有一些成员函数不改变类的数据成员，也就是说这些函数是“只读”函数，而有一些函数要修改数据成员的值。如果把把不改变数据成员的函数都加上const关键字进行标识，显然，可提高程序的可读性。其实还能提高程序的可靠性，以定义成const的成员函数，一旦企图修改数据成员的值，则编译器安错误处理。

```c++
class Point
{
  int xVal,int yVal;
public:
  int GetY() const;
};

//关键字const必须用同样的方式重复出现在函数实现里
//，否则编译器会把它看成不同的函数

int Point::GetY() const
{
  return yVal;
}
```

## const修饰指针
```c++
int b=500;
const int * a=&b;//1
int const * a=&b;//2
int * const a=&b;//3
const int * const a=&b;//4
```
1. 指针所指向的内容为常量
```c++
int b=500;
const int *a =&b;
*a=600; //wrong
```
但也有别的办法去改变*a的值：
```c++
//method 1
int b=500;
const int * a=&b;
b=600;
std::cout << *a << '\n';//600

//method 2
int b=500,c=600;
const int *a =&b;
a=&c;
std::cout << *a << '\n'; // 600
```
可以先不进行初始化。因为虽然指针内容是常量，但指针本身不是常量。
```c++
const int * a; //正确
```

2. 和情况1是一样的
3. 指针本身为常量，这种情况下不能对指针本身进行更改操作，而指针所指向的内容不是常量。
该情况下，定义时必须同时初始化：
```c++
int b=500,c=600;
int * const a;//wrong
int * const a=&b; // true
*a =600;//true
cout<<a++<<endl; //wrong
```
4. 指针本身和指向的内容均为常量

## const数据成员的初始化
只能在类的构造函数的初始化列表中进行

- [上一级](README.md)
- 上一篇 -> [判断浮点数是否相等](compareDoubleFloat.md)
- 下一篇 -> [c++ 构造函数抛出异常](constructorThrowException.md)
