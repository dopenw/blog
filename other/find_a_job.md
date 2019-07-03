# c/c++面试

1. 一个有10个指针的数组，该指针指向一个函数，该函数有一个整型参数并返回一个整型数
答：
```
int (* a[10])(int);
// An array of 10 pointers to functions that take an integer argument and return an integer
```
2. 进程和线程的区别?
答：什么是进程（ Process ）：普通的解释就是，进程是程序的一次执行，而什么是线程（ Thread ），线程可以理解为进程中的执行的一段程序片段。在一个多任务环境中下面的概念可以帮助我们理解两者间的差别：
进程间是独立的，这表现在内存空间，上下文环境；线程运行在进程空间内。 一般来讲（不使用特殊技术）进程是无法突破进程边界存取其他进程内的存储空间；而线程由于处于进程空间内，所以同一进程所产生的线程共享同一内存空间。 同一进程中的两段代码不能够同时执行，除非引入线程。线程是属于进程的，当进程退出时该进程所产生的线程都会被强制退出并清除。线程占用的资源要少于进程所占用的资源。 进程和线程都可以有优先级。在线程系统中进程也是一个线程。可以将进程理解为一个程序的第一个线程。
线程是指进程内的一个执行单元 , 也是进程内的可调度实体 . 与进程的区别 :
(1) 地址空间 : 进程内的一个执行单元 ; 进程至少有一个线程 ; 它们共享进程的地址空间 ; 而进程有自己独立的地址空间 ;
(2) 进程是资源分配和拥有的单位 , 同一个进程内的线程共享进程的资源
(3) 线程是处理器调度的基本单位 , 但进程不是 .
(4) 二者均可并发执行 .

3. C++ SWAP 不用第三方变量 实现
method 1:
```c++
   void swap(type& a, type& b)
   {
      a = a + b;
      b = a - b;
      a = a - b;
  }
```
上面这段代码是比较危险的，有数值溢出的危险。

 如果交换的数值是整形，则可以有如下可用代码：
method 2:
```c++
void swap(int& a, int& b)
  {
      a = a ^ b;
      b = a ^ b;
      a = a ^ b;
  }
```

[source link](http://blog.csdn.net/menggucaoyuan/article/details/6629230)

4. c++的设计模式?
[简单总结](https://www.cnblogs.com/FG123/p/5046649.html)
[pdf Preview](https://manyones.files.wordpress.com/2010/07/dp-2nd.pdf)
[设计模式概览](../cpp/design_patterns.md)

5. new 、 delete 、 malloc 、 free 关系，及 如果混用这些会怎么样？
delete 会调用对象的析构函数 , 和 new 对应 free 只会释放内存， new 调用构造函数。 malloc 与 free 是 C++/C 语言的标准库函数， new/delete 是 C++ 的运算符。它们都可用于申请动态内存和释放内存。对于非内部数据类型的对象而言，光用 maloc/free 无法满足动态对象的要求。对象在创建的同时要自动执行构造函数，对象在消亡之前要自动执行析构函数。由于 malloc/free 是库函数而不是运算符，不在编译器控制权限之内，不能够把执行构造函数和析构函数的任务强加于 malloc/free 。因此 C++ 语言需要一个能完成动态内存分配和初始化工作的运算符 new ，以及一个能完成清理与释放内存工作的运算符 delete 。注意 new/delete 不是库函数。

总结：new和delete会自动调用对象的构造与析构函数而malloc与free不会；new和delete式C++运算符，而malloc和free是C/C++标准库函数。

不过，将malloc/free和new/delete混合使用绝对不是什么好主意。

如果用free来释放通过new创建的动态对象，或者用delete释放通过malloc申请的动态内存，其结果都是未定义的。

换句话说，不能保证它会出现什么问题。如果程序在关键时刻就因为这个在重要客户面前出现问题，那么懊悔恐怕已经来不及了。

请记住：

```
（1）不要企图用malloc/free 来完成动态对象的内存管理，应该用new/delete。
（2）请记住：new是C++的，而malloc是c的。如果混淆了它们，那将是件蠢事。
    所以new/delete必须配对使用，malloc/free也一样。
```
[source link](http://www.nljb.net/default/%E5%B0%BD%E9%87%8F%E7%94%A8new%E5%92%8Cdelete%E4%BB%A3%E6%9B%BFmalloc%E5%92%8Cfree/)

6. QT的信号槽机制性能较会调函数会差一些

7. 堆和栈究竟有什么区别？

    主要的区别由以下几点：
    1、管理方式不同；
    2、空间大小不同；
    3、能否产生碎片不同；
    4、生长方向不同；
    5、分配方式不同；
    6、分配效率不同；

管理方式：对于栈来讲，是由编译器自动管理，无需我们手工控制；对于堆来说，释放工作由程序员控制，容易产生memory leak。

空间大小：一般来讲在32位系统下，堆内存可以达到4G的空间，从这个角度来看堆内存几乎是没有什么限制的。但是对于栈来讲，一般都是有一定的空间大小的，例如，在linux下使用ulimit -a  命令查看得到,当然这个值是可以修改的：
```sh
core file size          (blocks, -c) unlimited
data seg size           (kbytes, -d) unlimited
scheduling priority             (-e) 0
file size               (blocks, -f) unlimited
pending signals                 (-i) 31430
max locked memory       (kbytes, -l) 64
max memory size         (kbytes, -m) unlimited
open files                      (-n) 1024
pipe size            (512 bytes, -p) 8
POSIX message queues     (bytes, -q) 819200
real-time priority              (-r) 0
stack size              (kbytes, -s) 8192
cpu time               (seconds, -t) unlimited
max user processes              (-u) 31430
virtual memory          (kbytes, -v) unlimited
file locks                      (-x) unlimited
```

碎片问题：对于堆来讲，频繁的new/delete势必会造成内存空间的不连续，从而造成大量的碎片，使程序效率降低。对于栈来讲，则不会存在这个问题

生长方向：对于堆来讲，生长方向是向上的，也就是向着内存地址增加的方向；对于栈来讲，它的生长方向是向下的，是向着内存地址减小的方向增长。

分配方式：堆都是动态分配的，没有静态分配的堆。栈有2种分配方式：静态分配和动态分配。静态分配是编译器完成的，比如局部变量的分配。动态分配由alloca函数进行分配，但是栈的动态分配和堆是不同的，他的动态分配是由编译器进行释放，无需我们手工实现。

分配效率：栈是机器系统提供的数据结构，计算机会在底层对栈提供支持：分配专门的寄存器存放栈的地址，压栈出栈都有专门的指令执行，这就决定了栈的效率比较高。堆则是C/C++函数库提供的，它的机制是很复杂的，例如为了分配一块内存，库函数会按照一定的算法（具体的算法可以参考数据结构/操作系统）在堆 内存中搜索可用的足够大小的空间，如果没有足够大小的空间（可能是由于内存碎片太多），就有可能调用系统功能去增加程序数据段的内存空间，这样就有机会分 到足够大小的内存，然后进行返回。显然，堆的效率比栈要低得多。

从这里我们可以看到，堆和栈相比，由于大量new/delete的使用，容易造成大量的内存碎片；由于没有专门的系统支持，效率很低；由于可能引发用户态和核心态的切换，内存的申请，代价变得更加昂贵。所以栈在程序中是应用最广泛的，就算是函数的调用也利用栈去完成，函数调用过程中的参数，返回地址， EBP和局部变量都采用栈的方式存放。
[source link](http://blog.csdn.net/wo17fang/article/details/52244238)
[source link](https://unix.stackexchange.com/questions/127602/default-stack-size-for-pthreads)

8. 有3个线程A，B， C， 请用多线程编程实现在屏幕上循环打印10次ABCABC...， 其中A线程打印“A”， B线程打印“B”， C线程打印“C”。

```c++
#include <condition_variable>
#include <iostream>
#include <thread>

std::mutex mtx;
std::condition_variable CondVar;
char gCh = 0;

void printFun(char ch) {
  size_t count = 10;
  char ch_ = ch - 'A';

  for (size_t i = 0; i < count; i++) {
    std::unique_lock<std::mutex> ul(mtx);
    CondVar.wait(ul, [ch_] { return ch_ == gCh; });
    std::cout << (char)(ch_ + 'A') << '\n';
    gCh = (ch_ + 1) % 3;
    ul.unlock();
    CondVar.notify_all();
  }
}

int main(int argc, char const *argv[]) {

  std::thread t1(printFun, 'A');
  std::thread t2(printFun, 'B');
  std::thread t3(printFun, 'C');

  t1.join();
  t2.join();
  t3.join();

  std::cout << '\n';
  return 0;
}
```
9.下列两个程序的输出结果
```c++
#include <iostream>

char * test() {
  char * p = "hello world";
  return p;
}

int main(int argc, char const *argv[]) {
  std::cout << test() << '\n';
  return 0;
}
```
该程序能够正常输出 “hello world”

```c++
#include <iostream>

char *test() {
  char p[] = "hello";
  return p;
}

int main(int argc, char const *argv[]) {
  std::cout << test() << '\n';
  return 0;
}
```

程序正常运行，但只有输出一个换行符

10. 进程间通信有哪几种? 着重理解消息队列
[apue/IPC](../apue/IPC.md)

11. mysql 存储过程和函数的区别?
[mysql/procedureVSFunction](../mysql/procedureVSFunction.md)

[上一级](README.md)
[上一篇](fedoraInstallSS.md)
[下一篇](googleCloud.md)
