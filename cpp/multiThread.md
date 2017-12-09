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
[上一级](base.md)
[上一篇](inner_class.md)
[下一篇](mutable.md)
