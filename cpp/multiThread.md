# c++并发


参考书籍：The C++ Standard Library <2rd>
<!-- @import "[TOC]" {cmd="toc" depthFrom=1 depthTo=6 orderedList=false} -->
<!-- code_chunk_output -->

* [c++并发](#c并发)
	* [高级接口：async()和future](#高级接口async和future)
		* [Launch (发射)策略](#launch-发射策略)
		* [等待和轮询](#等待和轮询)
		* [传递实参](#传递实参)
		* [shared future](#shared-future)
	* [底层接口Thread](#底层接口thread)
		* [class std::thread](#class-stdthread)
			* [当心 detached thread](#当心-detached-thread)
			* [Thread ID](#thread-id)
		* [Namespace this_thread](#namespace-this_thread)
	* [线程同步化与并发问题](#线程同步化与并发问题)
		* [当心并发](#当心并发)
		* [什么情况下可能出错](#什么情况下可能出错)

<!-- /code_chunk_output -->

## 高级接口：async()和future

* async()提供了一个接口，让一段机能（a piece of functionality）或者说是一个callable object若是可能的化在后台运行，成为一个独立线程。
* class future<>允许你等待线程结束并获取其结果（一个返回值，或者也许是一个异常）。

```c++
#include <chrono>
#include <exception>
#include <future>
#include <iostream>
#include <random>
#include <thread>

int doSomething(char c) {
  std::default_random_engine dre(c);
  std::uniform_int_distribution<int> id(10, 1000);

  for (size_t i = 0; i < 10; i++) {
    std::this_thread::sleep_for(std::chrono::milliseconds(id(dre)));
    std::cout.put(c).flush();
  }
  return c;
}

int func1() { return doSomething('.'); }

int func2() { return doSomething('+'); }

int main(int argc, char const *argv[]) {
  std::cout << "starting fun1 in background and fun2 in foreground:" << '\n';

  auto result1(std::async(func1));

  int result2 = func2();
  int result = result1.get() + result2;
  std::cout << "\nresult of fun1 + fun2 : " << result << '\n';
  return 0;
}
```

output:
```sh
starting fun1 in background and fun2 in foreground:
+..+..+...+..+.+++++
result of fun1 + fun2 : 89
```

```c++
int result = result1.get() + result2;
```
随着get（）被调用，以下三件事情之一会发生：
1. 如果func1（）被启动于一个分离线程中并且已结束，你会立刻获得其结果
2. 如果func（）被启动但尚未结束，get()会引发停滞，待func1()结束后获得结果
3. 如果func1（）尚未启动，会被强迫启动，如同一个同步调用：get（）会引发停滞直至产生结果

这样的行为很重要，因为这确保了在单线程环境中，或是当async（）无法启动新线程时（不论基于任何理由），程序仍能够有效运作。

### Launch (发射)策略
强迫async()绝不推迟目标函数的执行：
```c++
//force func1() to start asynchronously now or throw std::system_error
std::future<int> result1=std::async(std::launch::async,func1);
```
延缓func1()直到你调用std::future的get（）
```c++
std::future <...> f(std::launch::deferred,func1);
//defer func1 until get()
```

### 等待和轮询

一个future调用get()一次。在那之后future就处于无效状态，而这种状态只能借由“对future调用valid()”来检测。此情况下对它的任何调用（析构函数除外）会不可预期的行为。

但是future也提供了一个接口，允许我们等待后台操作完成而不需要处理其结果。这个接口可以被调用一次以上；也可以结合一个duration（时间段）或timepoint（时间点）以限制等待时间.

只要对某个future调用wait(),就可以强制启动该future象征的线程并等待这一后台操作终止：
```c++
std::future<...> f(std::async(func));
//try to call func asynchronously

...

f.wait() ; //wait for func to be done
//(might start background task)
```
另外，还有两个相似的函数，但是它们并不强制启动线程（如果线程尚未启动的话）：
```c++
std::future<...> f(std::async(func));

...

f.wait_for(std::chrono::seconds(10));
//wait at most 10 seconds for func

f.wait_until(std:system_clock::now()+std::chrono::minutes(1));
```

不论时wait_for() 或wait_until()都返回以下三种东西之一：
* std::future_status::deferred --如果async延缓了操作而程序中又完全没有wait()或get()(那会强制启动)。这种情况下以上两个函数都会立刻返回。
* std::future_status::timeout --如果某个操作被异步启动但尚未结束，而waiting又已逾期（对于给定的时间段而言）
* std::future_status::ready --如果操作已完成

示例：等待两个task
```c++
#include <chrono>
#include <exception>
#include <future>
#include <iostream>
#include <random>
#include <thread>
using namespace std;
int doSomething(char c) {
  std::default_random_engine dre(c);
  std::uniform_int_distribution<int> id(10, 1000);

  for (size_t i = 0; i < 10; i++) {
    std::this_thread::sleep_for(std::chrono::milliseconds(id(dre)));
    std::cout.put(c).flush();
  }
  return c;
}

int main(int argc, char const *argv[]) {
  std::cout << "starting 2 operations asynchronously" << '\n';

  auto f1 = std::async([] { doSomething('.'); });
  auto f2 = std::async([] { doSomething('+'); });

  if (f1.wait_for(chrono::seconds(0)) != future_status::deferred ||
      f2.wait_for(chrono::seconds(0)) != future_status::deferred) {
    while (f1.wait_for(chrono::seconds(0)) != future_status::ready &&
           f2.wait_for(chrono::seconds(0)) != future_status::ready) {
      this_thread::yield(); // hint to reschedule to next thread
    }
  }
  std::cout << '\n';

  try {
    f1.get();
    f2.get();
  } catch (const exception &e) {
    std::cout << "\nexception : " << e.what() << '\n';
  }
  std::cout << "done" << '\n';
  return 0;
}
```

output:
```sh
starting 2 operations asynchronously
+..+..+...+..+.
+++++done
```

### 传递实参
如果你使用async(),就应该以by value方式传递所有“用来处理目标函数”的必要object，使async()只需使用局部拷贝（local copy）。如果复制成本太高，请让那些object以const reference 的形式传递，且不使用mutable；

### shared future
class std::future 提供了“处理并发运算之未来结果”的能力。然而你只能处理该结果一次。第二次调用get（）会导致不可预期的行为（
++ STL,通常时鼓励（但不强制）抛出一个std::future_error）。
然而有时候，多次处理“并发运算之未来结果”时合理的，特别当多个线程都想处理这份结果时。基于这个目的，c++标准库提供了class std::shared_future,于是你可以多次调用get（），导致相同的结果，或抛出相同的异常。

code:
```c++
#include <exception>
#include <future>
#include <iostream>
#include <stdexcept>
#include <thread>

using namespace std;

int queryNumber() {
  std::cout << "read number:" << '\n';
  int num;
  cin >> num;


//handle wrong input
  if (!cin) {
    throw runtime_error("no number read");
  }
  return num;
}

void doSomething(char c, shared_future<int> f) {
  try {
    int num = f.get();

    for (ssize_t i = 0; i < num; i++) {
      this_thread::sleep_for(chrono::milliseconds(100));
      cout.put(c).flush();
    }
  } catch (const exception &e) {
    std::cerr << "exception in thread" << this_thread::get_id() << ":"
              << e.what() << '\n';
  }
}

int main(int argc, char const *argv[]) {
  try {

    shared_future<int> f = async(queryNumber);
    //or auto f=async(queryNumber).share();

    auto f1 = async(launch::async, doSomething, '.', f);
    auto f2 = async(launch::async, doSomething, '+', f);
    auto f3 = async(launch::async, doSomething, '*', f);

    f1.get();
    f2.get();
    f3.get();
  } catch (const exception &e) {
    std::cout << "exception:" << e.what() << '\n';
  }
  std::cout << "\n done" << '\n';
}
```

## 底层接口Thread

除了高级接口async()和(shared) future,c++ 标准库还提供了一个启动处理线程的底层接口。

### class std::thread
[class thread](http://www.cplusplus.com/reference/thread/thread)

预启动某个线程，只需先声明一个class std::thread对象，并将目标任务（task）当作初始实参，然后要么就等待它结束(join())，要么就将它卸离（detach()）

```c++
void doSomething();
std::thread t(doSomething);
...
t.join();
```

注意：除非你真的知道你在做什么，否则面对“处理目标函数所必须”的所有object都应该以by value方式传递，使得thread只使用local copy。

thread() vs async() :
* class thread 没有所谓的发射策略（launch policy）。C++标准库永远试着将目标函数启动于一个新线程中。如果无法做到会抛出std::system_error并带有差错码resource_unavailable_try_again
* 没有接口可处理线程结果(std::future_status::deferred,std::future_status::timeout,std::future_status::ready).唯一可以获得的是一个独一无二的线程ID
* 如果发生异常，但未被捕获于线程之内，程序会立刻中止并调用std::terminate()。若向将异常传播至线程外的某个context，必须使用exception_ptr
* 你必须使用join()或者detach().如果你在thread object 寿命结束前不这么做，或如果它发生了一次move assignment,程序会中止并调用std::terminate()
* 如果你让线程运行与后台而main()结束了，所有线程会被鲁莽而硬性的终止

```c++
#include <chrono>
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

#### 当心 detached thread
我们失去了detached thread的控制权。因此，请确定绝不让一个detached thread访问任何寿命已经结束的object。基于这个理由，“以 by reference 方式传递变量和object”给线程，总是带有风险。强烈建议以by value方式传递。

尽管如此，请牢记一个经验法则：终止detached thread 的唯一安全方法为搭配“ at_thread_exit()”函数群中的某一个。这会“强制main thread等待detached thread 真正结束”

#### Thread ID
```c++
//inner thread call
this_thread::get_id();
```

```c++
//outer thread call
std::thread t(test);
test.get_id();
```

### Namespace this_thread
| fuction | info    |
| :------------- | :------------- |
|this_thread::get_id()| 获取当前线程的ID      |
|this_thread::sleep_for(dur)|将某个线程阻塞 dur时间段
|this_thread::sleep_until(tp)|将某个线程阻塞直到时间点tp
|this_thread::yield()|让下一个线程能够执行

函数this_thread::yield()用来告诉系统，放弃当前线程的时间切片余额，这将石运行环境得以重新调度以便允许其他线程执行。

“放弃控制-yield()”的典型的例子：
1. 当等待或轮询另一线程，或等待或轮询“某个atomic flag被另一线程设定”：
```c++
while (!readyFlag) { //loop until data is ready
	std::this_thread::yield();
}
```
2. 当你尝试锁定多个lock/mutex却无法取得其中一个lock或mutex，那么在尝试不同次序的lock/mutex之前可以使用yield()，这会让你的程序更快些

## 线程同步化与并发问题

线程同步化技术：
* mutex 和 lock
* condition variable
* atomic

### 当心并发
多个线程并发处理相同的数据而又不曾同步化，那么唯一安全的情况就是：所有的线程只读取数据

### 什么情况下可能出错
* 未同步化的数据访问：并行运行两个线程读和写同一笔数据，不知道哪一个语句先来
* 写至半途的数据：某个线程正在读数据，另一个线程改动它，于是读取中的线程甚至可能读到改了一半的数据，读到一个半新半旧的值
* 重新安排的语句：语句和操作有可能被重新排序，也许对于每一个单线程正确，但对于多个线程的组合却破坏了预期的行为

1. 未同步化的数据访问
```c++
if(val>=0)
	f(val); //pass positive val
else
	f(-val); //pass negated negative val
```
在单线程环境中上述代码没问题。然而在一个多线程环境中，这段代码不一定能成功运作。如果多个线程处理val，val的值有可能在"if 子句"和“调用f()”之间被改变，造成负值被传给f()

2. 写至半途的数据

考虑我们有一个变量：
```c++
long long x=0;
```
某个线程对它写入数值：
```c++
x=-1;
```
另一个线程读取它：
```c++
std::cout<<x;
```
第二个线程的输出可能：
*  0
* -1
* 任何其他值--如果第二个线程在“第一线程对x赋值-1的过程中”读取-1；



3. 重新安排的语句
假设有两个共享对象，一个是long，用来将data从某个线程传递到另一个线程，另一个是readyFlag，用来表示第一线程是否已提供数据：
```c++
long data=0;
bool readyFlag=false;
```
一种天真的做法是，将“某线程对data的设定”和“另一线程对data的消费”同步化。于是供应端这么调用：
```c++
data=42;
readyFlag=true;
```
而消费段这么调用：
```c++
while (!readyFlag) {
	; //loop until data is ready
}
foo(data);
```
在不知任何细节的情况下，几乎每个程序员一开始都会认为第二线程必是在data有值42之后才调用foo()。
但其实这并非必要。事实上第二线程的输出有可能是data“在第一线程赋值42之前”旧值（甚至任何值，因为42赋值动作有可能只做一半）。
也就是说，编译器和/或硬件有可能重新安排语句，使得实际执行下列动作：
```c++
readyFlag=true;
data=42;
```

一般而言，基于c++规则，在不影响线程的行为的前提下，这样重新安排是允许的。

基于相同理由，设置第二线程也可能被安排重新安排语句，前提是不影响该线程的行为：
```c++
foo(data);
while (!readyFlag) {
	; //loop until data is ready
}
```

[上一级](base.md)
[上一篇](inner_class.md)
[下一篇](mutable.md)
