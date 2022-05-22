<!-- do_while_false的功用 -->

<!-- @import "[TOC]" {cmd="toc" depthFrom=1 depthTo=6 orderedList=false} -->
<!-- code_chunk_output -->

- [用法一](#用法一)
- [用法二](#用法二)

<!-- /code_chunk_output -->

## 用法一
```cpp
if( A==true )  
{  
    if( B==true )  
    {  
        if( C==true )  
        {  
            if( D==true )  
            {  
                //code
            }  
        }  
    }  
}  
```
等价于
```cpp
do  
{  
    if( A==false )  
        break;  
    if( B==false )  
        break;  
    if( C==false )  
        break;  
    if( D==false )  
        break;  
    //code
}while(false);
```

## 用法二
在宏定义中使用：
```c
#define Foo(x) do {\
  statement one;\
  statement two;\
} while(0)
```
初看会觉得很奇怪，集让循环里面只执行一次，那要这个多余的do...while(0)有什么意义呢？
再看这样的宏：
```c
#define Foo(x){\
statement one;\
statement two;\
}
```
这两个看似一样的宏其实是不一样的。前者定义的红是一个非复合语句，而后者却是一个复合语句。假如有这样的使用场景：
```c
if (condition)
  Foo(x);
else
  ...;
```

因为宏在预处理的时候会直接被展开，采用第二种写法，会变成：
```c
if (condition)
  statement one;
  statement two;
else
  ...;
```
这样会导致else语句孤立而出现编译错误。加了do{...}while(0),这就使得宏展开后，仍然保留初始的语义，从而保证程序的正确性。


---
- [上一级](README.md)
- 上一篇 -> [开发注意事项](develop_care_detail.md)
- 下一篇 -> [Effective c++ 改善程序与设计的55个具体做法](effectiveCpp55.md)
