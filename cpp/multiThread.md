<!-- c++并发 -->


参考书籍：The C++ Standard Library <2rd>
<!-- @import "[TOC]" {cmd="toc" depthFrom=1 depthTo=6 orderedList=false} -->
<!-- code_chunk_output -->

- [高级接口：async()和future](#高级接口async和future)
  - [Launch (发射)策略](#launch-发射策略)
  - [等待和轮询](#等待和轮询)
  - [传递实参](#传递实参)
  - [shared future](#shared-future)
- [底层接口Thread](#底层接口thread)
  - [class std::thread](#class-stdthread)
    - [当心 detached thread](#当心-detached-thread)
    - [Thread ID](#thread-id)
  - [Namespace this_thread](#namespace-this_thread)
- [线程同步化与并发问题](#线程同步化与并发问题)
  - [当心并发](#当心并发)
  - [什么情况下可能出错](#什么情况下可能出错)
- [Mutex 和 Lock](#mutex-和-lock)
  - [使用Mutex和Lock](#使用mutex和lock)
    - [lock_guard](#lock_guard)
    - [递归的(Recursive) Lock](#递归的recursive-lock)
    - [尝试性的Lock以及带时间性的Lock](#尝试性的lock以及带时间性的lock)
    - [处理多个Lock](#处理多个lock)
    - [class unique_lock](#class-unique_lock)
    - [只调用一次](#只调用一次)
- [Condition Variable(条件变量)](#condition-variable条件变量)
  - [条件变量的第一个完整的例子](#条件变量的第一个完整的例子)
  - [使用条件变量实现多线程Queue](#使用条件变量实现多线程queue)
- [Atomic](#atomic)
  - [Atomic 用例](#atomic-用例)
  - [Atomic 的 C-Style 接口](#atomic-的-c-style-接口)
  - [Atomic 的底层接口](#atomic-的底层接口)
- [信号量](#信号量)
  - [使用信号量来实现互斥](#使用信号量来实现互斥)
  - [利用信号量来调度共享资源](#利用信号量来调度共享资源)
    - [生产者-消费者问题](#生产者-消费者问题)
    - [读者-写者问题](#读者-写者问题)
- [线程安全](#线程安全)
  - [可重入函数](#可重入函数)

<!-- /code_chunk_output -->

## 高级接口：async()和future

* async()提供了一个接口，让一段机能（a piece of functionality）或者说是一个callable object若是可能的话在后台运行，成为一个独立线程。
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
* 如果你让线程运行于后台而main()结束了，所有线程会被鲁莽而硬性的终止

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
t.get_id();
```

### Namespace this_thread
| fuction | info    |
| :------------- | :------------- |
|this_thread::get_id()| 获取当前线程的ID      |
|this_thread::sleep_for(dur)|将某个线程阻塞 dur时间段
|this_thread::sleep_until(tp)|将某个线程阻塞直到时间点tp
|this_thread::yield()|让下一个线程能够执行

函数this_thread::yield()用来告诉系统，放弃当前线程的时间切片余额，这将使运行环境得以重新调度以便允许其他线程执行。

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

## Mutex 和 Lock

Mutex（互斥体）

### 使用Mutex和Lock

```c++
int val;
std::mutex valMutex;
```

one thread:
```c++
valMutex.lock();
if(val>=0)
	f(val);
else
	f(-val);
valMutex.unlock();
```

other thread:
```c++
valMutex.lock();
++val;
valMutex.unlock();
```

#### lock_guard

[class std::lock_guard](http://en.cppreference.com/w/cpp/thread/lock_guard) 是一个互斥包装器，它提供了一个方便的 [RAII](https://zh.wikipedia.org/wiki/RAII) 风格的机制来在一个作用域块持续时间内拥有一个互斥锁。 当一个lock_guard对象被创建时，它会尝试获取它给出的互斥量的所有权。当控制权离开创建lock_guard对象的范围时，lock_guard被销毁并释放互斥锁。 lock_guard类不可复制。

代码示例：
```c++
#include <future>
#include <iostream>
#include <mutex>
#include <string>

using namespace std;
mutex printMutex;

void print(const string &s) {
  lock_guard<mutex> l(printMutex);
  for (char c : s) {
    cout.put(c);
  }
  std::cout << '\n';
}

int main(int argc, char const *argv[]) {
  auto f1 = async(std::launch::async, print, "hello from a first thread");
  auto f2 = async(std::launch::async, print, "hello from a second thread");
  print("hello from the main thread");
  return 0;
}
```

run it:
```terminate
hello from the main thread
hello from a second thread
hello from a first thread
```

如果没有mutex，输出可能是：
```terminate
hello from the main threadhello from a fir
st thread
hello from a second thread
```

#### 递归的(Recursive) Lock

一个典型的例子：
```c++
class DatabaseAccess
{
private:
	std::mutex dbMutex;
	... //state of database access
public:
	void createTable(...)
	{
		std::lock_guard<std::mutex> lg(dbMutex);
		...
	}

	void insertData(...)
	{
		std::lock_guard<std::mutex> lg(dbMutex);
		...
	}

	...

};

//当我们引入一个public成员函数而它可能调用其它public成员函数，情况变得复杂：

void createTableAndInsertData(...)
{
	std::lock_guard<std::mutex> lg(dbMutex);

	...

	createTable(...);  //error: deadLock because dbMutex is locked again
}

```

借着使用recursive_mutex,上述的行为不再有问题。这个mutex允许同一线程多次锁定，并在最近一次（last）相应的unlock()时释放lock：

```c++
class DatabaseAccess
{
private:
	std::recursive_mutex dbMutex;
	... //state of database access
public:
	void createTable(...)
	{
		std::lock_guard<std::recursive_mutex> lg(dbMutex);
		...
	}

	void insertData(...)
	{
		std::lock_guard<std::recursive_mutex> lg(dbMutex);
		...
	}

	void createTableAndInsertData(...)
	{
		std::lock_guard<std::recursive_mutex> lg(dbMutex);

		...

		createTable(...);  //ok ,no deadLock
	}

	...

};
```

#### 尝试性的Lock以及带时间性的Lock

有时候程序想要获得一个lock，但如果不可能成功的话它不想永远阻塞。针对这种情况，mutex提供成员函数try_lock(),它试图获取一个lock，成功就返回true，失败则返回false。

为了仍能够使用lock_guard,你可以传一个额外实参adopt_lock给其构造函数：
```c++
std::mutex m;

while (m.try_lock()==false) {
	doSomething();
}

std::lock_guard<std::mutex> lg(m,std::adopt_lock);
...
```

注意，try_lock()有可能假性失败，也就是说即使lock并未被他人拿走它也可能失败。

为了等待特定长度的时间，你可以选用所谓的 timed mutex:
* class std::timed_mutex
* class std::recursive_timed_mutex
这两个类，允许你调用try_lock_for(),try_lock_until()

```c++
std::timed_mutex m;

if(m.try_lock_for(std::chrono::seconds(1)))
{
	std::lock_guard<std::timed_mutex> lg(m,std::adopt_lock);
	...
}
else
{
	couldNotGetTheLock();
}
```

#### 处理多个Lock

通常一个线程一次只该锁定一个mutex。然而有时候必须锁定多个mutex（例如为了传递数据，从一个受保护资源到另一个。）

这种情况下若以目前介绍过的lock机制来应付，可能变得复杂且具有风险：你或许取得第一个lock却拿不到第二个lock，或许发生deadlock（如果以不同的次序去锁住相同的lock）

c++标准库为此提供了若干便捷函数，让你锁定多个mutex。
```c++
std::mutex m1;
std::mutex m2;

...

{
	std::lock(m1,m2) ; //lock both mutexs(or none if possible)
	std::lock_guard<std::mutex> lockM1(m1,std::adopt_lock);
	std::lock_guard<std::mutex> lockM2(m2,std::adopt_lock);

	...
} //automatically unlock all mutexs
```
全局std::lock()会锁住它收到的所有mutex，而且阻塞直到mutex都被锁定或直到抛出异常。如果是后者，已被成功锁定的mutex都会被解锁。
注意，这个lock()提供了一个deadlock回避机制，但也意味着多个lock的锁定次序并不明确。

以此相同方式你可以尝试“取得多个lock”且“若并非所有lock都可用也不至于造成阻塞”。全局函数std::try_lock()会在取得所有lock情况下返回-1，否则返回第一个失败的lock的索引（从0开始计），且如果这样的话所有成功的lock会被unlocked。

```c++
std::mutex m1;
std::mutex m2;

int idx=std::try_lock(m1,m2); //try to lock both mutexs

if (idx<0)
{
	std::lock_guard<std::mutex> lockM1(m1,std::adopt_lock);
	std::lock_guard<std::mutex> lockM2(m2,std::adopt_lock);
	...
}// automatically unlock all mutexs
else
{
	//idx has zero-base index of first failed lock
	std::cerr << "could not lock mutex m" <<idx+1<< '\n';
}
```

注意，这个try_lock()不提供deadlock回避机制，但它保证以出现于实参列的次序来试着完成锁定。

请注意，通常我们不会“只调用lock()或try_lock()”却“不把那些lock过继(adopt)给一个lock_guard”。虽然代码看起来好像建立了“离开作用域是会自动解锁的lock，其实并非如此，这些mutex仍然保持锁定”。
```c++
std::mutex m1;
std::Mutex m2;

...
{
	std::lock(m1,m2); //lock both mutexs (or none if not possible)
	//no lock adopted
	...
}
... //OOPS: mutexs are still locked!!!
```

#### class unique_lock

除了class lock_guard<>,c++ STL还提供了[class unique_lock<>](http://en.cppreference.com/w/cpp/thread/unique_lock),它对mutex更有弹性。class unique_lock<>提供的接口和class lock_guard<>相同，而又允许明确写出“何时”以及“如何”锁定或解锁其Mutex。因此其object可能（但也可能不）拥有一个被锁住的mutex。这和lock_guard<>不同，后者的object生命中总是锁定一个mutex。此外，对unique_lock你可以调用owns_lock()或bool()来查询其mutex目前是否被锁住。

注解：unique_lock 的命名解释了其行为的由来。就像unique pointer,你可以把他们在作用域之间搬移，但保证一次只有一个lock 拥有mutex。

这个class的主要优点仍然是：
* 如果析构时mutex仍被锁住，其析构函数会自动调用unlock()。如果当时没有锁住mutex，则析构函数不做任何事。

class unique_lock<> VS class lock_guard<>,unique_lock添加了以下三个构造函数：
* 你可以传递try_to_lock,表示企图锁定mutex但不希望阻塞:
```c++
std::mutex m;
std::unique_lock<std::mutex> lock(m,std::try_to_lock);
...
if(lock)
{// if lock was successful
	...
}
```

* 你可以传递一个时间段或时间点给构造函数，表示尝试在一个明确的时间周期内锁定：
```c++
std::timed_mutex m;
std::unique_lock<std::timed_mutex> lock(m,std::chrono::seconds(1));
...
```
* 你可以传递defer_lock,表示初始化这个lock object但尚未打算锁住mutex:
```c++
std::mutex m;
std::unique_lock<std::mutex> lock(m,std::defer_lock);
...
lock.lock(); //or (timed) try_lock()
...
```

有了lock_guard 和 unique_lock 作为工具，现在我们可以实现一个粗浅例子，以轮询某个ready flag的方式，令一个线程等待另一个线程：
```c++
#include <mutex>

...

bool readyFlag;
std::mutex readyFlagMutex;

void thread1()
{
	//do doSomething thread2 needs as preparation

	...
	std::lock_guard<std::mutex> lg(readyFlagMutex);
	readyFlag=true;
}

void thread2()
{
	//wait until readyFlag is true (thread1 is done)

	{
		std::unique_lock<std::mutex>  u1(readyFlagMutex);
		while (!readyFlag) {
			u1.unlock();
			std::this_thread::yield(); //hint to reschedule to the next thread
			std::this_thread::sleep_for(std::chrono::milliseconds(100));
			u1.lock();
		}
	}//release lock

	//do whatever shall happen after threa1 has prepared things
	...
}
```

#### 只调用一次
有时候某些机能初次被某个线程使用过后，其他线程再也不需要它。在多线程环境下，可以这样：只需使用一个std::once_flag以及调用[std::call_once](http://en.cppreference.com/w/cpp/thread/call_once)(也由\<mutex\>提供)
```c++
std::once_flag oc;
static std::vector<std::string> staticData;

void foo()
{
	static std::once_flag oc;
	std::call_once(oc,[]{
						staticData=initializeStaticData();
						});
...
}
```
## Condition Variable(条件变量)

[class condition_variable](http://en.cppreference.com/w/cpp/thread/condition_variable)

有时候，被不同线程执行的task必须彼此等待。所以对“并发操作”实现同步化除了data race 之外还有其他原因。

条件变量可以用来同步化线程之间的数据流逻辑依赖关系。

在之前的章节中介绍了“让某个线程等待另一个线程”的一个粗浅的方法，就是使用ready flag之类的东西。当某个线程已有准备，或它已经为另一个线程提供了某个东西，上述flag就发出信号。这通常意味着等待中的线程需要轮询其所需要的数据或条件是否已达到：
```c++
bool readyFlag;
std::mutex readyFlagMutex;

//wait until readyFlag is true
{
	std::unique_lock<std::mutex> u1(readyFlagMutex);
	while (!readyFlag) {
		ul.unlock();
		std::this_thread::yield(); //hint to reschedule to the next thread
		std::this_thread::sleep_for(std::chrono::milliseconds(100));
		ul.lock();
	}
}//release lock
```
然而这样“针对目标条件而轮询”的动作通常不是好的解决办法。

一个较好的做法是使用条件变量，c++ STL在<condition_variable>中提供了它。他是一个变量，借由它可以唤醒一个或多个其他等待中的线程。

原则上，条件变量运作如下：
* 你必须同时包含<mutex> and <condition_variable>,并声明一个mutex和一个condition variable:
```c++
#include <mutex>
#include <condition_variable>

std::mutex readyMute;
std::condition_variable readyCondVar;
```
* 那个激发“条件终于满足”的线程必须调用：
```c++
readyCondVar.notify_one() ;//notify one of the waiting threads
//or
readyCondVar.notify_all(); //notify all the waiting threads
```
* 那个“等待条件被满足”的线程必须调用
```c++
std::unique_lock<std::mutex> l(readyMute);
//需要 unique_lock,因为lock_guard是不够的，因为等待中的函数有可能锁定或者解除mutex
readyCondVar.wait(l);
```

此外，条件变量也许有所谓的假醒。也就是某个条件变量的wait动作有可能在该条件变量尚未被notified时返回。
因此，发生wakeup不一定一位这线程所需要的条件已经掌握了。更确切的说，在wakeup之后你仍然需要代码去验证“条件实际已达成”。例如我们必须检查数据是否真正备妥，或是我们仍需要ready flag之类的东西。为了设立和查询它端供应的数据或ready flag，可以使用同一个mutex。

### 条件变量的第一个完整的例子
```c++
#include <condition_variable>
#include <future>
#include <iostream>
#include <mutex>

bool readyFlag;
std::mutex readyMutex;
std::condition_variable readyCondVar;

void thread1() {
  // do something thread2 needs as preparation

  std::cout << "<return>" << '\n';
  std::cin.get();

  // signal that thread1 prepared a condition
  {
    std::lock_guard<std::mutex> lg(readyMutex);
    readyFlag = true;
  } // release lock
  readyCondVar.notify_one();
}

void thread2() {
  // wait until thread1 is ready
  {
    std::unique_lock<std::mutex> ul(readyMutex);
    readyCondVar.wait(ul, [] { return readyFlag; });

    // 在这里条件变量的 wait()成员函数的第二个 lambda，用来检测条件是否真的满足。
    // 效果等同于下面代码： {
    //   std::unique_lock<std::mutex> ul(readyMutex);
    //   while (!readyFlag) {
    //     readyCondVar.wait(ul);
    //   }
    // } // release lock

  } // release lock

  // do whatever shall happen after thread1 has prepared things

  std::cout << "done" << '\n';
}

int main(int argc, char const * argv[]) {
  auto f1 = std::async(std::launch::async, thread1);
  auto f2 = std::async(std::launch::async, thread2);
  return 0;
}
```

### 使用条件变量实现多线程Queue

在该例中，三个线程都把数值push某个queue，另外两个线程则是从中读取数据：
```c++
#include <condition_variable>
#include <future>
#include <iostream>
#include <mutex>
#include <queue>

std::queue<int> queue;
std::mutex queueMutex;
std::condition_variable queueCondVar;

void provider(int val) {
  // push different values (val til val+5 with timeouts of val milliseconds into
  // the queue)

  for (size_t i = 0; i < 6; i++) {
    std::lock_guard<std::mutex> lg(queueMutex);
    queue.push(val + i);
  } // release lock
  queueCondVar.notify_one();

  std::this_thread::sleep_for(std::chrono::milliseconds(val));
}

void consumer(int num) {
  // pop values if available (num identifies the consumer)

  while (true) {
    int val = 0;
    {
      std::unique_lock<std::mutex> ul(queueMutex);
      queueCondVar.wait(ul, [] { return !queue.empty(); });
      val = queue.front();
      queue.pop();
    } // release lock
    std::cout << "consumer " << num << ": " << val << '\n';
  }
}

int main(int argc, char const *argv[]) {
  // start three provider for values 100+,300+,and 500+
  auto p1 = std::async(std::launch::async, provider, 100);
  auto p2 = std::async(std::launch::async, provider, 300);
  auto p3 = std::async(std::launch::async, provider, 500);

  // start two consumer printing the values
  auto c1 = std::async(std::launch::async, consumer, 1);
  auto c2 = std::async(std::launch::async, consumer, 2);
  return 0;
}
```

run it :
```terminate
consumer consumer 12: : 100
consumer 1: 102
consumer 1: 103
consumer 1: 104
consumer 1: 105
consumer 1: 300
consumer 1: 301
consumer 1: 302

consumer 1: 303
consumer 1: 304
consumer 1: 305
consumer 1: 500
consumer 1: 501
consumer 1: 502
consumer 1: 503
consumer 1: 504
consumer 1: 505
101
```

也请注意：condition_variable也提供接口允许你等待某个最大时间量：

* wait_for() 等待一个时间段
* wiat_until() 等待直到某个时间点

## Atomic

[atomic](http://en.cppreference.com/w/cpp/atomic/atomic)

在之前我们提到的例子
```c++
bool readyFlag;
std::mutex readyFlagMutex;

//wait until readyFlag is true
{
	std::unique_lock<std::mutex> u1(readyFlagMutex);
	while (!readyFlag) {
		ul.unlock();
		std::this_thread::yield(); //hint to reschedule to the next thread
		std::this_thread::sleep_for(std::chrono::milliseconds(100));
		ul.lock();
	}
}//release lock
```
中我们使用bool readyFlag让任意线程激发，表示某件事情已经为另一线程备妥或提供。你也许会惊讶为什么仍然需要mutex。如果我们有个bool值，为什么不能并发地让某些线程改变它而让另一线程校验它？在供应端（线程）将bool设置为true的那个时刻，观测端（线程）应能够看到它并执行随之发生的处理才是。

正如我们之前介绍的那样，这里我们需要面对两个问题：
1. 一般而言，即使面对基本数据类型，读和写也不是atomic（不可切割的）。因此本例你可能读取到一个被写一半的bool值，c++ Standard说这会带来不明确的行为。
2. 编译器生成的代码有可能改变操作次序，所以供应端（线程）有可能在供应数据之前就先设置ready flag,而消费端亦有可能在侦测ready flag 之前就处理数据。

借由mutex，两个问题迎刃而解，但是从必要的资源和潜藏的独占访问来看，mutex也许是个相对昂贵的操作，所以也许值的以atomic取代mutex和lock。

本节中我首先介绍atomic的高层接口，它所提供的操作将使用默认保证，不论内存访问次序如何。这个默认保证提供了顺序一致性：在线程中的atomic操作保证一定“像代码出现的次序”那样地发生，“重排语句”将不会发生。本节末尾将展示atomic的底层接口：带有“放宽之次序保证”的操作。

注意，c++ 标准库并不区分atomic的高层或底层接口。某些时候atomic底层接口也被称为weak或relaxed接口，而高层接口被称为normal或strong接口。

### Atomic 用例
改写上面提到的例子，改用atomic:

```c++
#include <atomic> //for atomic types
...

std::atomic<bool> readyFlag(false);

void thread1()
{
	//do something thread2 needs as preparation
	...
	readyFlag.store(true);
}

void thread2()
{
	//wait until readyFlag is true (thread1 is done)
	while (!readyFlag.load()) {
		std::this_thread::sleep_for(std::chrono::milliseconds(100));
	}

	//do whatever shall happen after thread1 has prepared things
	...
}
```

注意你总是应该将atomic_object初始化，因为其default构造函数并不完全初始化它（倒不是其初值不明确，而是其lock未被初始化）。面对一个static-duration atomic 对象，你应该使用一个常量作为初值。如果只使用default构造函数，接下来唯一允许的操作是如下调用global atomic_init():
```c++
std:atomic<bool> readyFlag;
...
std::atomic_init(&readyFlag,false);
```

这种初始化方式之所以出现，是为了让你写出c编译器可接受的代码。

处理atomic的两个重要语句是：
```c++
std::atomic<bool> test(false);

test.store(true); //赋予一个新值
test.load(); //取当前值
```

重点是，这些操作都保证是atomic（不可切割的），所以我们不需要像以前那样“需要mutex的保护才能够设置 readyFlag ”

然而，使用condition variable 时我们仍然需要mutex才能保护对condition variable的消费（即使它现在是个atomic object）：
```c++
//wait until thread1 is ready (readyFlag is true)
{
	std::unique_lock<std::mutex> l(readyMute);
	readyCondVar.wait(l,[]{
		return readyFlag.load();
	});
}//release lock
```
让我们来看下，使用atomic的一个完整的例子：
```c++
#include <atomic>
#include <chrono>
#include <future>
#include <iostream>
#include <thread>

long data;
std::atomic<bool> readyFlag(false);

void provider() {
  // after reading a character
  std::cout << "<return>" << '\n';
  std::cin.get();

  // provide some data
  data = 42;

  // and signal  readiness
  readyFlag.store(true);
}

void consumer() {
  // wait for readlines and do something else
  while (!readyFlag.load()) {
    std::cout.put('.').flush();
    std::this_thread::sleep_for(std::chrono::milliseconds(500));
  }

  // and process provided data
  std::cout << "\nvalue: " << data << '\n';
}

int main(int argc, char const *argv[]) {

  // start provider and consumer
  auto p = std::async(std::launch::async, provider);
  auto c = std::async(std::launch::async, consumer);

  return 0;
}
```

* store()会对影响所及的内存区执行一个所谓的release操作，确保此前所有内存操作不论是否为atomic，在store发挥效用之前都变成“可被其他线程看见”。

* load()会对影响所及的内存区执行一个所谓的acquire操作，确保此前所有内存操作不论是否为atomic，在load之后都变成“可被其他线程看见”。

于是，由于data的设值在 provider()将readFlay存储为true之前，而对data的处理发生在 consumer() 将true 载入 放进readFlag之后，因此对data的处理保证发生在data已提供之后。

这项保证之所以存在，是因为所有atomic操作默认使用一个特别的内存次序，名为memory_order_seq_cst,它代表 sequential consistent mememory order (顺序一致的内存次序)。底层的atomic操作能够放宽这一次序保证。

### Atomic 的 C-Style 接口

针对c++的atomic提案，c有一分对应的提案，它应该提供相同的语义但是（当然）不使用template、reference和memeber function等c++特性。

例如：
```c
std::atomic_bool ab;
std::atomic_init(&ab,false);
...
std::atomic_store(&ab,true);
...
if (std::atomic_load(&ab))
{
		...
}
```

### Atomic 的底层接口

atomic 底层接口意味着使用 atomic 操作时不保证顺序一致性。因此编译器和硬件有可能（局部）重排对atomic的处理次序。

再请小心：虽然我给了一个实例，本区域仍然地雷重重。你需要很多专家经验才能知道何时值得在内存重排上花心力-即便是专家对此也常常犯错。

考虑之前提到的atomic运用实例：

如果指定另外一种内存处理次序，我们就可以削弱对次序的保证，在我们的例子中这就足以（例如）要求 provider 不推迟 atomic store 之后的操作，而 consumer 不会在 atomic load 之后带来向前操作。

```c++
data=42;

readyFlag.store(true,std::mememory_order_release);

while (!readyFlag.load(std::mememory_order_acquire)) {
	...
}
std::cout << data <<std::endl;
```

然而如果放宽 atomic 操作次序上的所有约束，会导致不明确的行为：
```c++
// error:undefined behavior:
data=42;
readyFlag.store(true,std::mememory_order_relaxed);
```
原因是 std::mememory_order_relaxed 不保证此前所有内存操作 在store 发挥效用前都变得 “可被其他线程看见”。因此provider 线程有可能在设置ready flag之后才写data，于是 consumer 线程有可能在data 正被写时读它，这就会造成 data race

你也可以让 data 成为 atomic 并以 std::mememory_order_relaxed 作为内存次序：
```c++
std::atomic<long> data(0);
std::atomic<bool> readyFlag(false);

//providing thread:
data.store(42,std::mememory_order_relaxed);
readyFlag.store(true,std::mememory_order_relaxed);

//consuming thread
while (!readyFlag.load(std::mememory_order_relaxed)) {
	...
}
std::cout << data.load(std::mememory_order_relaxed) << '\n';
```

只有当我们在 atomic 变量上的读/写动作彼此独立，memory_order_relaxed才能显现出用途。例如一个 global 计数器，不同的线程可能会对它累加或递减，而我们只需要在所有线程终结之后获得该计数器的最终值即可。

底层操作概览：load,store,exchange,CAS,fetch等操作提供了一个增补能力：它们允许你额外传递一个内存次序实参。

例如：
```c++
a.store(val,mo)
a.load(mo)
a.exchange(val,mo)
a.compare_exchange_strong(exp,des,mo)
a.compare_exchange_strong(exp,des,mo1,mo2)
a.compare_exchange_weak(exp,des,mo)
a.compare_exchange_weak(exp,des,mo1,mo2)
a.fetch_add(val,mo)
a.fetch_sub(val,mo)
a.fetch_and(val,mo)
a.fetch_or(val,mo)
a.fetch_xor(val,mo)
```

## 信号量

参考：深入理解计算机系统 2rd 12.5

信号量 s 是具有非负整数值的全局变量，只能由两种特殊的操作来处理，这两种操作称为 P 和 V：
* P(s):如果 s 是非零的，那么 P 将 s 减 1，并且立即返回。如果 s 为零，那么就挂起这个线程，知道 s 变为非零，而一个 V 操作会重启这个线程。在重启之后， P 操作将 s 减1，并将控制返回给调用者。
* V(s):V 操作将 s 加 1 。如果有任何线程阻塞在 P 操作等待s变为非零，那么V操作会重启这些线程中的一个，然后该线程将s减 1，完成它的 P 操作。

P 中的测试和减 1 操作是不可分割的，也就是说，一旦预测信号量 s 变为非零，就会将 s 减 1，不能有中断。
V 中的加 1 操作也是不可分割的，也就是加载、 加 1 和存储信号量的过程中没有中断。
注意：V 的定义中没有定义等待线程被重新启动的顺序。唯一要求的是 V 必须值只能重启一个等待的线程。因此，当有多个线程在等待同一个信号量时，你不能预测 V 操作要重启哪一个线程。

Posix：
```c
#include <semaphore.h>

int sem_init(sem_t *sem,unsigned int value);
int sem_wait(sem_t *s); // P(s)
int sem_post(sem_t *s); // V(s)

// if successful resturn 0,an error occurred return -1
```

为了简明，我们可以用包装函数：
```c++
#include <csapp.h>

void P(sem_t *s); // wrapper function for sem_wait
void V(sem_t *s); // wrapper function for sem_post
```

### 使用信号量来实现互斥

以这种方式来保护共享变量的信号量叫做 二元信号量，因为它的值总是 0 或 1 。以提供互斥为目的的二元信号量常常也称为互斥锁。

```c
volatile int cnt=0;
sem_t mutex;
...
sem_init(&mutex,0,1);
...
for (ssize_t i = 0; i < niters; i++) {
	P(&mutex);
	cnt++;
	V(&mutex);
}
```

### 利用信号量来调度共享资源

#### 生产者-消费者问题

生产者和消费者线程共享一个有n个槽的有限缓冲区。生产者线程反复地生成新的项目，并把他们插入到缓冲区中。消费者线程不断地从缓冲区中取出这些项目，然后消费它们。也可能是多个生产者和消费者的变种。

![](../images/multiThread_201801071724_1.png)

结合书上的例子，现给出完整例子：
```c++
#include <iostream>
#include <semaphore.h>
#include <stdlib.h>
#include <thread>

using namespace std;

void P(sem_t *s) {
  if (sem_wait(s) != 0)
    std::cerr << "sem_wait is failed" << '\n';
  // care you need handle error
}

void V(sem_t *s) {
  if (sem_post(s) != 0)
    std::cerr << "sem_post is failed" << '\n';
  // care you need handle error
}

struct sbuf_t {
  int * buf;    // buffer array
  int n;       // maximum number of slots
  int front;   // buf[(front+1)%n] is first item
  int rear;    // buf[rear%n] is last item
  sem_t mutex; // protects accesss to buf
  sem_t slots; // counts available slots
  sem_t items; // counts available items
};

void sbuf_init(sbuf_t *sp, int n) {
  sp->buf = (int * )calloc(n, sizeof(int));
  sp->n = n;
  sp->front = sp->rear = 0;
  sem_init(&sp->mutex, 0, 1);
  sem_init(&sp->slots, 0, n);
  sem_init(&sp->items, 0, 0);
}

void sbuf_deinit(sbuf_t *sp) { free(sp->buf); }

void sbuf_insert(sbuf_t *sp, int item) {
  P(&sp->slots);
  P(&sp->mutex);
  sp->buf[(++sp->rear) % (sp->n)] = item;
  std::cout << "insert: " << item << '\n';
  V(&sp->mutex);
  V(&sp->items);
}

void sbuf_remove(sbuf_t *sp) {
  int item;
  P(&sp->items);
  P(&sp->mutex);
  item = sp->buf[(++sp->front) % (sp->n)];
  std::cout << "remove: " << item << '\n';
  V(&sp->mutex);
  V(&sp->slots);
}

void createProducer(sbuf_t *sp, int item) {
  for (ssize_t i = 0; i < item; i++) {
    sbuf_insert(sp, i);
  }
}

void createConsumer(sbuf_t *sp, int item) {
  while (--item >= 0) {
    sbuf_remove(sp);
  }
}

int main(int argc, char const *argv[]) {
  sbuf_t buffer;
  sbuf_init(&buffer, 10);

  thread t1(createProducer, &buffer, 500);
  thread t2(createConsumer, &buffer, 500);
  t1.join();
  t2.join();
  sbuf_deinit(&buffer);
  return 0;
}
```

#### 读者-写者问题

修改对象的线程叫做写者。只读对象叫做读者。写者必须拥有对对象的独占的访问，而读者可以和无限多个其他的读者共享对象。一般来说，有无限多个并发的读者和写者。

读者-写者问题有几个变种，每个都是基于读者和写者的优先级的。
* 第一类：读者优先，要求不要让读者等待，除非已经把使用对象的权限赋予了一个写者
* 第二类：写者优先，要求一旦一个写者准备好可以写，他就会尽可能快地完成它的写操作。同第一类问题不同，在一个写者后到达的读者必须等待，即使这个写者也是在等待。

对于这两种读者-写者问题的正确解答可能会导致饥饿，饥饿就是一个线程无限期地阻塞，无法进展。

## 线程安全

当用线程编写程序时，我们必须小心地编写那些具有称为 线程安全性 属性的函数。一个函数被称为线程安全的，当且仅当被多个并发线程反复地调用时，它会一直产生正确的结果。

我们能够定义出四个（不相交的）线程不安全函数类：
* 不保护共享变量的函数
* 保持跨越多个调用的状态的函数。例如，rand函数是线程不安全的，因为当前调用的结构依赖于前次调用的中间结果。使得像rand这样的函数线程安全的唯一方式是重写它。
* 返回指向静态变量的指针的函数。例如，ctime和gethostbyname，将计算结果放在一个static变量中，然后返回一个指向这个变量的指针。有两种方式来处理这类不安全函数。一种选择重写，另一种是使用加锁-拷贝。
* 调用线程不安全函数的函数。

### 可重入函数
有一类重要的线程安全函数，叫做可重入函数，其特点就在于他们具有：当它们被多个线程调用时，不会引用任何共享数据。

可重入函数通常比不可重入的线程安全函数高效一些，因为他们不需要同步操作。

* 如果所有的函数参数都是传值传递的（即没有指针），并且所有的数据引用都是本地的自动栈变量（即没有引用静态或全局变量），那么函数就是 显式可重入 ，也就是说，无论它是被如何调用，我们都可以断言它是可重入的。
* 我们把假设放松一点，允许显式可重入函数中的一些参数是引用传递的，那么我们就得到一个 隐式可重入的函数，也就是说，如果调用线程小心的传递指向非共享数据的指针，那么它是可重入的。


---
- [上一级](README.md)
- 上一篇 -> [log4cplus 学习笔记](log4cplus.md)
- 下一篇 -> [mutable的使用](mutable.md)
