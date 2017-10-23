# 以stream完成I/O


<!-- @import "[TOC]" {cmd="toc" depthFrom=1 depthTo=6 orderedList=false} -->
<!-- code_chunk_output -->

* [以stream完成I/O](#以stream完成io)
	* [1. I/O stream 的共通基础](#1-io-stream-的共通基础)
		* [1.1 stream对象](#11-stream对象)
		* [1.2 stream class](#12-stream-class)
		* [1.3 全局的stream对象](#13-全局的stream对象)
		* [1.4 将标准stream重定向](#14-将标准stream重定向)

<!-- /code_chunk_output -->


## 1. I/O stream 的共通基础

### 1.1 stream对象
* c++ I/O 由stream完成。
所谓stream就是一条数据流（stream of data）,字符序列在其中“川流不息”。按照面向对象原则，stream是由某个class定义出来具有特定性质的对象。输出动作被解读为“数据流进stream”，输入动作则是“数据流出stream”。

### 1.2 stream class
* istream --> input stream,用来读数据
* ostream --> output stream,用来写数据

### 1.3 全局的stream对象

IOStream程序库定义了数个类型为istream和ostream的全局对象，他们对应于标准的I/O通道：

* cin,隶属于istream，是供使用者输入用的标准输入通道，对应于c中的stdin。操作系统通常将它和键盘连接。

* cout，隶属于ostream，是供使用者输出用的标准输出通道，对应于c中的stdout。操作系统通常将它和显示器连接。

* cerr，隶属于ostream，是所有报错信息所使用的标准报错输出通道，对应于c中的stderr。操作系统通常也将它和显示器连接。默认情况下cerr无缓冲（not buffered）

* clog，隶属于ostream，是标准日志通道，c中没有对应物。默认情况下操作系统将它连接与cerr所连接的设备，但clog有缓冲（is buffered）

将“正常输出”和“错误信息输出”加以分离，可以让程序以不同的方式对待两种不同的输出。例如可以将正常输出重新定向至某个文件，而同时仍然令报错信息显示于控制台。当然，前提是操作系统必须支持标准I/O通道的重定向功能（redirection，大部分操作系统的确可以）。

### 1.4 将标准stream重定向

* 只要通过“设置stream缓冲区”就可以重定向某个stream。

* “设置stream缓冲区”意味着I/OStream 的重定向可由程序控制，不必借助操作系统

例如以下语句做了些设置，使得cout的信息不被送到标准的output通道，而是被送到cout.txt中去：
```c++
std::ofstream file("cout.txt");
std::cout.rdbuf (file.rdbuf());
```

函数copyfmt()用来将某个stream的所有格式信息赋值给另外一个stream对象：
```c++
std::ofstream file("cout.txt");
file.copyfmt (std::cout);
std::cout.rdbuf (file.rdbuf());
```

eg:
```c++
#include <iostream>     // std::cout
#include <fstream>      // std::ofstream

int main () {
  std::ofstream filestr;
  filestr.open ("test.txt");

  std::cout.fill ('*');
  std::cout.width (10);

  //copyfmt()
  //Copies the values of all the internal members of rhs (except the state flags and the associated stream buffer) to the corresponding members of * this.
  filestr.copyfmt (std::cout);

  // save cout.rdbuf()

  auto sou=std::cout.rdbuf();

  //重定向到文件
  std::cout.rdbuf (filestr.rdbuf());
  std::cout << 40;
  std::cout.rdbuf (sou);
  std::cout<<"test"<<std::endl;

  return 0;
}
```

结果：
```
test
test.txt 中内容为********40
```

* #### 小心！！！
上述的file是局部对象，将在上述程序区段结束时被销毁，相应的stream缓冲区也一并销毁。这个标准的stream不同，因为通常file stream在构造过程分配stream缓冲区，并于析构时销毁他们。所以本例中的cout不能再被用于wrting。事实上，它甚至无法在程序结束时被安全的销毁。因此我们应该保留旧缓冲区并于时后恢复！
下例的函数redirec()就负责这个事情：

```c++
#include <iostream>
#include <fstream>
#include <memory> // unique_ptr

using namespace std;

void redirect(ostream &);

int main(int argc, char const *argv[]) {
  cout<<"the first row"<<endl;
  redirect(cout);
  std::cout << "the last row" << '\n';
  return 0;
}

void redirect(ostream strm)
{
  auto del=[&](streambuf* p){
    strm.rdbuf(p);
  };

  unique_ptr<streambuf,decltype(del)> origBuffer(strm.rfbuf(),del);

  //redirect output into the file redirect.txt

  ofstream file("redirect.txt");
  strm.rdbuf(file.rdbuf());

  file<<"one row for the file"<<endl;
  strm<<"one row for the stream"<<endl;
}

```
借着使用一个unique_ptr，我们可以确保，甚至当resize()由于异常而丢下，被存储于origBuffer的原先输出缓冲区还是会被还原。

程序输出：
```
the first row
the last row
```

文件 redirect.txt的内容如下：
```
one row for the file
one row for the stream
```
redirect()中写至cout的输出信息被送到文件去了。mian()执行redirect()后，接下来的输出再次被送到（已恢复了的）输出通道。


参考资料：<C++ 标准库_第二版>

[上一级](base.md)
[上一篇](regex.md)
[下一篇](vir_del.md)
