# 多线程


<!-- @import "[TOC]" {cmd="toc" depthFrom=1 depthTo=6 orderedList=false} -->
<!-- code_chunk_output -->

* [多线程](#多线程)
	* [线程概述](#线程概述)
	* [线程执行模型](#线程执行模型)
* [名词解释](#名词解释)

<!-- /code_chunk_output -->

## 线程概述

[操作系统 wikipedia](https://zh.wikipedia.org/wiki/%E6%93%8D%E4%BD%9C%E7%B3%BB%E7%BB%9F)
[线程 wikipedia](https://zh.wikipedia.org/wiki/%E7%BA%BF%E7%A8%8B)

线程就是运行在进程 [上下文](#名词解释) 中的 [逻辑流](#名词解释) 。在现代系统中允许我们编写一个进程里同时运行多个线程的程序。线程由内核自动调度。每个线程都有它自己的线程上下文，包括一个唯一的整数线程 ID 、栈、栈指针、程序计数器、通用目的寄存器和条件码。所有的运行在一个进程里的线程共享该进程的整个 [虚拟地址空间](http://wiki.legendit.ca/cn/137.html) 。

## 线程执行模型

多线程的执行模型在某些方面和多进程的执行模型是相似的。思考一下图中的示例。每个进程开始生命周期时都是单一线程，这个线程称为主线程。在某一时刻，主线程创建一个对等线程，从这个时间点开始，两个线程就并发地运行。最后，因为主线程执行一个慢速系统调用，例如 read 或 sleep，或者因为它被系统的间隔计时器中断，控制就会通过上下文切换传递到对等线程。对等线程会执行一段时间，然后控制传递回主线程，依次类推。

在一些重要的方面，线程执行是不同于进程的。因为一个线程的上下文要比一个进程的上下文小得多，线程的上下文切换要比进程的上下文切换快得多。另一个不同的就是线程不像进程那样，不是按照严格的父子层次来组织的。和一个进程相关的线程组成一个对等（线程）池（pool），独立于其他线程创建的线程。主线程和其他线程的区别仅在于它总是进程中第一个运行的线程。对等（线程）池概念的主要影响是，一个线程可以杀死它的任何对等线程，或者等待它的任意对等线程终止。另外，每个对等线程都能读写相同的共享数据。

## 分离线程

在任何一个时间点上，线程是可结合的 (joinable) 或者是分离的 (detached)。一个可结合的线程能够被其它线程收回其资源和杀死。在被其它线程回收之前，他的存储器资源 （例如栈） 是没有被释放的。相反，一个分离的线程是不能被其它线程回收或杀死的。他的存储器资源在它终止时由系统自动释放。

## 多线程示例

```c++
#include <chrono> // 一个精度中立的时间程序库
#include <exception>
#include <iostream>
#include <random>
#include <thread>
using namespace std;

void doSomething(int num, char c) {
  try {
    default_random_engine dre(42 * c);
    uniform_int_distribution<int> id(10, 1000);
    for (ssize_t i = 0; i < num; i++) {
      this_thread::sleep_for(chrono::milliseconds(id(dre)));
      std::cout.put(c).flush();
    }
  }

  catch (const exception &e) {
    std::cerr << "thread_exception(thread " << this_thread::get_id()
              << "):" << e.what() << '\n';
  }

  catch (...) {
    std::cerr << "thread_exception (thread " << this_thread::get_id() << ")"
              << '\n';
  }
}

int main(int argc, char const *argv[]) {
  try {
    thread t1(doSomething, 5, '.');
    std::cout << "- started fg thread" << t1.get_id() << '\n';

    for (size_t i = 0; i < 5; i++) {
      thread t(doSomething, 10, 'a' + i);
      std::cout << "- detach started bg thread " << t.get_id() << '\n';
      t.detach();//注释掉这行会core dump
    }
    cin.get();
    std::cout << "- join fg thread " << t1.get_id() << '\n';
    t1.join();
  }

  catch (const exception &e) {
    std::cerr << "exception:" << e.what() << '\n';
  }
  return 0;
}
```




# 名词解释

1. 逻辑控制流:如果用调试器单步执行程序，我们会看到一系列的 [程序计数器](https://zh.wikipedia.org/wiki/%E7%A8%8B%E5%BC%8F%E8%A8%88%E6%95%B8%E5%99%A8) （PC） 的值，这些值唯一地对应与包含在程序的可执行目标文件中的指令。这个 PC 的值的序列叫做逻辑控制流，或者简称逻辑流。

* 上下文是由程序正确运行所需的状态组成的。这个状态包括存放在存储器中的程序的代码和数据，他的栈、通用目的寄存器的内容、程序计数器、环境变量以及打开的文件描述符的集合。

* [上下文切换](https://zh.wikipedia.org/wiki/%E4%B8%8A%E4%B8%8B%E6%96%87%E4%BA%A4%E6%8F%9B#%E4%B8%8A%E4%B8%8B%E6%96%87%E4%BA%A4%E6%8F%9B%EF%BC%9A%E5%85%B7%E9%AB%94%E6%AD%A5%E9%A9%9F)


[上一级](base.md)
[上一篇](graph.md)
[下一篇](sort.md)
