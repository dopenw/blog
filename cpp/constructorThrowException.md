# c++ 构造函数抛出异常


<!-- @import "[TOC]" {cmd="toc" depthFrom=1 depthTo=6 orderedList=false} -->
<!-- code_chunk_output -->

* [c++ 构造函数抛出异常](#c-构造函数抛出异常)
	* [处理构造函数错误的方法](#处理构造函数错误的方法)
	* [在构造函数中捕获异常](#在构造函数中捕获异常)
	* [参考链接](#参考链接)

<!-- /code_chunk_output -->


在构造函数中抛出异常，示例如下：
```c++
#include <iostream>
#include <exception>

class test {
private:
  int m_i;
  int * m_array;

public:
  test ()
  {
    m_i=3;
    m_array=new int[30];
    for(std::size_t i=0;i<30;i++)
    {
    	m_array[i]=i;
    }
    throw std::runtime_error("test constructor throw exception!");
  }
  void tt()
  {
    std::cout<<"use test::tt() function"<<m_i<<std::endl;
  }
  virtual ~test ()
  {
	 delete [] m_array;
    std::cout<<"call ~test() functon"<<std::endl;
  }
};

int main(int argc, char const *argv[]) {
  test * t;
  try
  {
    t=new test;
  }
  catch(std::exception &e)
  {
    std::cout<<e.what()<<std::endl;
  }

  t->tt();
  delete t;
  return 0;
}
```
Compile it:
```sh
ConstructorThrowExcp.cpp: In function ‘int main(int, const char**)’:
ConstructorThrowExcp.cpp:43:10: warning: ‘t’ may be used uninitialized in this function [-Wmaybe-uninitialized]
   delete t;
```

Run it:
```sh
[breap@breap other]$ ./ConstructorThrowExcp
test constructor throw exception!
Segmentation fault (core dumped)
```

Use valgrind:
```sh
[breap@breap other]$valgrind --leak-check=full ./ConstructorThrowExcp
==7267== HEAP SUMMARY:
==7267==     in use at exit: 120 bytes in 1 blocks
==7267==   total heap usage: 6 allocs, 5 frees, 74,074 bytes allocated
==7267==
==7267== 120 bytes in 1 blocks are definitely lost in loss record 1 of 1
==7267==    at 0x4C2F933: operator new[](unsigned long) (vg_replace_malloc.c:423)
==7267==    by 0x400F54: test (ConstructorThrowExcp.cpp:13)
==7267==    by 0x400F54: main (ConstructorThrowExcp.cpp:35)
==7267==
==7267== LEAK SUMMARY:
==7267==    definitely lost: 120 bytes in 1 blocks
==7267==    indirectly lost: 0 bytes in 0 blocks
==7267==      possibly lost: 0 bytes in 0 blocks
==7267==    still reachable: 0 bytes in 0 blocks
==7267==         suppressed: 0 bytes in 0 blocks
==7267==
==7267== For counts of detected and suppressed errors, rerun with: -v
==7267== ERROR SUMMARY: 2 errors from 2 contexts (suppressed: 0 from 0)
Segmentation fault (core dumped)
```

```c++
#include <iostream>
#include <exception>

class test {
private:
  int m_i;
  int * m_array;

public:
  test ()
  {
    m_i=3;
    m_array=new int[30];
    for(std::size_t i=0;i<30;i++)
    {
    	m_array[i]=i;
    }
    throw std::runtime_error("test constructor throw exception!");
  }
  void tt()
  {
    std::cout<<"use test::tt() function"<<m_i<<std::endl;
  }
  virtual ~test ()
  {
	 delete [] m_array;
    std::cout<<"call ~test() functon"<<std::endl;
  }
};

int main(int argc, char const *argv[]) {
  try
  {
     test t;
     t.tt();
  }
  catch(std::exception &e)
  {
    std::cout<<e.what()<<std::endl;
  }

  return 0;
}
```

Compile it:
```sh
[breap@breap other]$ g++ ConstructorThrowExcp.cpp -o test
# 无错误信息
```

Run it:
```c++
test constructor throw exception!
```

Use valgrind:
```sh
HEAP SUMMARY:
==8423==     in use at exit: 120 bytes in 1 blocks
==8423==   total heap usage: 5 allocs, 4 frees, 74,050 bytes allocated
==8423==
==8423== 120 bytes in 1 blocks are definitely lost in loss record 1 of 1
==8423==    at 0x4C2F933: operator new[](unsigned long) (vg_replace_malloc.c:423)
==8423==    by 0x400EA3: test (ConstructorThrowExcp.cpp:13)
==8423==    by 0x400EA3: main (ConstructorThrowExcp.cpp:34)
==8423==
==8423== LEAK SUMMARY:
==8423==    definitely lost: 120 bytes in 1 blocks
==8423==    indirectly lost: 0 bytes in 0 blocks
==8423==      possibly lost: 0 bytes in 0 blocks
==8423==    still reachable: 0 bytes in 0 blocks
==8423==         suppressed: 0 bytes in 0 blocks
==8423==
==8423== For counts of detected and suppressed errors, rerun with: -v
==8423== ERROR SUMMARY: 1 errors from 1 contexts (suppressed: 0 from 0)
```

## 处理构造函数错误的方法

通常有三种常见的处理方法，标记错误状态、使用一个额外的initialize函数来初始化，或者直接抛出异常。

1. 使用一个额外的initialize函数来初始化： 众所周知，C++的析构函数中不应该抛出异常，一旦析构函数中的异常没有被捕获，整个程序都要被中止掉。于是许多人就对在构造函数中抛出异常也产生了对等的恐惧，宁可使用一个额外的初始化函数在里面初始化对象的状态并抛出异常（或者返回错误代码）。这样做违背了对象产生和初始化要在一起的原则，强迫用户记住调用一个额外的初始化函数，一旦没有调用直接使用了其他函数，其行为很可能是未定义的。
使用初始化函数的惟一好处可能是避免了手动释放资源（释放资源的操作交给析构函数来做），因为C++的一个特点是构造函数抛出异常以后析构函数是不会被调用的，所以如果你在构造函数里面申请了内存或者打开了资源，需要在异常产生时关闭。但想想看其实并不能完全避免，因为有些资源可能是在调用产生错误的函数后才被申请创建的，还是无法完全避免手工的释放。

2. 标记错误状态: 譬如STL中的ifstream类，当构造时传入一个无法访问的文件作为参数，它不会返回任何错误，而是标记的内部状态为不可用，用户需要手工通过is_open()函数来判断是否打开成功了。同时它还有good()、fail()两个函数，同时也重载了bool类型转换运算符用于在if语句中判断。标记状态的方法在实践中相当丑陋，因为在使用前总是需要判断它是否「真的创建成功了」。

3. 在构造函数中抛出异常: 它并不会向析构函数中抛出异常那样有严重的后果，只是需要注意的是抛出异常以后对象没有被创建成功，析构函数也不会被调用，所以应该自行把申请的资源全部都释放掉。

## 在构造函数中捕获异常
```c++
func::func() : foo()
{
    try {...}
    catch (...) // will NOT catch exceptions thrown from foo constructor
    { ... }
}
```
vs.
```c++
func::func()
    try : foo() {...}
    catch (...) // will catch exceptions thrown from foo constructor
    { ... }
```

在构造函数中捕获异常的一个简单的方法：
```c++
#include <iostream>
#include <exception>

class B {
 public:
  B(int val) : val_(val * val) {
    throw std::runtime_error("throw exception from B");
  }
 private:
  int val_;
};

class A {
 public:
  A(int val) try : b_(val) {
    a_ = val;
  } catch (std::exception& e) {
    std::cerr << e.what() << std::endl;
    throw std::runtime_error("throw exception from A"); //如果注释掉这行，则默认会将原来捕获到的异常重新抛出
  }
 private:
  int a_;
  B b_;
};

int main(int argc, char const *argv[]) {
  try
  {
    A a(1);
  }
  catch(std::exception &e)
  {
    std::cerr << e.what() << std::endl;
  }
  return 0;
}
```

Run it：
```sh
throw exception from B
throw exception from A
```

## 参考链接
* [如何处理C++构造函数中的错误——兼谈不同语言的错误处理](https://www.byvoid.com/zhs/blog/cpp-constructor-exception)
* [Throwing exceptions from constructors
](https://stackoverflow.com/questions/810839/throwing-exceptions-from-constructors)


- [上一级](README.md)
- 上一篇 -> [const限定符](const.md)
- 下一篇 -> [将string转换为char指针](conv_string_to_char_pointer.md)
