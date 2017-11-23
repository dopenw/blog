# 函数参数入栈的顺序

栈的生长方向是向下的，即是从高地址向底地址伸展的。

1. 测试程序：
```c
#include <stdio.h>
int foo(int x, int y, int z)

{

  printf("x = %d at [%X]\n", x, &x);

  printf("y = %d at [%X]\n", y, &y);

  printf("z = %d at [%X]\n", z, &z);
  return 1;
}

int main(int argc, char *argv[])

{
  foo(100, 200, 300);
  return 0;
}
```
64bit,gcc version 7.2.1:
```
x = 100 at [3E84B9DC]
y = 200 at [3E84B9D8]
z = 300 at [3E84B9D4]
```

32 bit,use
```sh
g++ -m32 test.cpp -o au
```

```sh
x = 100 at [FFF97860]
y = 200 at [FFF97864]
z = 300 at [FFF97868]
```

参数的顺序却是相反的.也就是说而前者则是从左到右,后者的函数参数初始化顺序是从右到左


如果不考虑编译器进行inline优化的话,应该属于调用约定的范畴. 那么什么是调用约定?在汇编中,JUMP和CALL指令的区别在于, JUMP会直接进行跳转,而CALL会自动把当前的指令寄存器(PC指针), 系统状态寄存器(程序状态字),以及其他重要的寄存器值先压入堆栈,再执行跳转,通常在函数结束后配合RET进行反向的出桟操作. 可以看到,汇编指令本身并没有涉及到参数的传入和返回值的规定,所以怎么传参,是写汇编的人自己定义的,你可以将参数放在某个硬编码的RAM里,可以存在寄存器里,也可以存在桟上,只要你自己知道怎么取出来.

但是,这在多人协作的时候,你一套我一套,函数就没法复用了.因此,才有了可以算是准规则的calling conventions([调用约定](https://en.m.wikipedia.org/wiki/X86_calling_conventions)）

在不同的平台(处理器架构)上,不同的编译器可能使用的调用约定并不相同. 不同的调用约定表格(C/C++编译器) [见这里](https://en.m.wikipedia.org/wiki/X86_calling_conventions#List_of_x86_calling_conventions)

不同的调用约定会用不同的方法来传参以及获得返回值, linux x86-64 GCC, 所使用的调用约定则是System V AMD64 ABI convention, 函数参数在执行CALL之前依次存入64位寄存器RDI, RSI, RDX, RCX, R8, R9, 浮点数(如果有)则存入XMM0–7, 更多的参数从右往左入桟. 可以自行汇编(gcc -S main.c -o main.s)进行确认.

结论：
除了PASCAL约定之外,函数传参如果用到栈,一定是将函数参数按照从右到左的顺序入桟的.

[source link](https://www.zhihu.com/question/42091882)
[source link](http://www.cnblogs.com/xkfz007/archive/2012/03/27/2420158.html)


2. c 中计算参数时是从右到左压栈的
```c
#include <stdio.h>

int main(int argc, char const *argv[]) {
  int b = 3;
  int arr[] = {6, 7, 8, 9, 10};
  int * ptr = arr;
  * (ptr++) += 123;
  printf("%d,%d\n", * ptr, * (++ptr));
  return 0;
}
```
无论使用32位编译，还是64位，其结果都是
```
8,8
```

[上一级](base.md)
[上一篇](do_while_false.md)
[下一篇](initalization_list.md)
