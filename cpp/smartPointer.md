# 智能指针


<!-- @import "[TOC]" {cmd="toc" depthFrom=1 depthTo=6 orderedList=false} -->
<!-- code_chunk_output -->

* [智能指针](#智能指针)
	* [智能指针错误用法](#智能指针错误用法)
	* [Class unique_ptr](#class-unique_ptr)
		* [使用 unique_ptr](#使用-unique_ptr)
		* [移转 unique_ptr 的拥有权](#移转-unique_ptr-的拥有权)
		* [对付 Array](#对付-array)
		* [使用自定义的 deleter](#使用自定义的-deleter)
	* [class shared_ptr](#class-shared_ptr)
		* [对付 array](#对付-array-1)
		* [shared_ptr 环形指向](#shared_ptr-环形指向)

<!-- /code_chunk_output -->

## 智能指针错误用法
```c++
#include <iostream>
#include <memory>

struct Base
{
    Base() { std::cout << "  Base::Base()\n"; }
    // Note: non-virtual destructor is OK here
    ~Base() { std::cout << "  Base::~Base()\n"; }
};

struct Derived: public Base
{
    Derived() { std::cout << "  Derived::Derived()\n"; }
    ~Derived() { std::cout << "  Derived::~Derived()\n"; }
};

int main()
{
    Derived * pDerived=new Derived;                            
    std::shared_ptr<Base> p(pDerived);
    std::shared_ptr<Base> p1(pDerived);
     //std::cout << "All threads completed, the last one deleted Derived\n";
}
```
Run it:
```sh
Base::Base()
Derived::Derived()
Derived::~Derived()
Base::~Base()
Derived::~Derived()
Base::~Base()
```

## Class unique_ptr
[unique_ptr](https://en.cppreference.com/w/cpp/memory/unique_ptr) 一般而言，这个 smart pointer 实现了独占式拥有概念，意味着它可确保一个对象和其相应资源同一时间只被一个 pointer 拥有。一旦拥有者被销毁或变成 empty,或开始拥有另外一个对象，先前拥有的那个对象就会被销毁，其任何相应资源亦会被释放。

### 使用 unique_ptr
unique_ptr 有着与寻常 pointer 非常相似的接口，操作符 * 用来提领 (dereference) 指向对象，操作符 -> 用来访问成员-- 如果被指向的对象来自 class 或 struct;

```cpp
// create and init (pointer to) string:

std::unique_ptr<std::string> up(new std::string("nico"));

(* up)[0] ='N'; //replace first char
up->append("lai"); //append some char
std::cout<< *up <<std::endl; // print whole string
```

unique_ptr 不允许你以赋值语法将一个寻常的 pointer 当作初值。
```cpp
std::unique_ptr<int> up=new int; //error
std::unique_ptr<int> up(new int); //ok
```

unique_ptr 不必一定拥有对象，它也可以是 empty.
```cpp
std::unique_ptr<std::string> up;
```
你也可以对它赋予 nullptr 或调用 reset();

此外，你可以调用 release(),获得 unique_ptr 拥有的对象并放弃拥有权，于是调用者现在对该对象有了责任：
```cpp
std::unique_ptr<std::string> up(new std::string("nico"));
...
std::string * sp=up.release(); //up loses ownership
```

你可以调用操作符 bool() 用以检查是否 unique_ptr 拥有 （任何） 对象；
```cpp
if (up) { // if up is not empty
  ...
}
```

or

```cpp
if (up != nullptr) // if up is not empty
if (up.get() != nullptr) // if up is not empty
```

### 移转 unique_ptr 的拥有权
```cpp
//init a unique_ptr with a new object
std::unique_ptr<ClassA> up1(new ClassA);

//copy the unique_ptr
std::unique_ptr<ClassA> up2(up1); //error,not possible

//transfer ownership of the unique_ptr
std::unique_ptr<ClassA> up3(std::move(up1)); //ok
```
第三个语句将拥有权从 up1 移到 up3。 在那之后 up3 就拥有了先前以 new 建立起来的对象，而 up1 不再拥有它。一旦 up3 被销毁，以 new ClassA 创建出来的对象也就被 deleted.

Assignment 操作符的行为和上面所说很类似：
```cpp
std::unique_ptr<ClassA> up1(new ClassA);
std::unique_ptr<ClassA> up2; //create another unique_ptr

up2=up1; //error,not possible
up2=std::move(up1); //assign the unique_ptr ,transfer ownership from up1 to up2
```
这里的 move assign 将拥有权由 up1 移交给 up2。于是 up2 拥有了先前被 up1 拥有的对象。如果上述赋值操作之前 up2 原本拥有对象，会有一个 delete 动作被调用，删除该对象；

```cpp
std::unique_ptr<ClassA> up1(new ClassA);
std::unique_ptr<ClassA> up2(new ClassA);

up2=std::move(up1); /// move assign the unique_ptr
// delete object owned by up2
// transfer ownership from up1 to up2
```
失去对象拥有权的 unique_ptr 并不会获得一个 “指向无物” (refer to no object) 的新拥有权。
如果想要指新值给 unique_ptr,新值必须也是个 unique_ptr ,不可以是寻常 pointer;
```cpp
std::unique_ptr<ClassA> ptr;

ptr=new ClassA;  //error

ptr = std::unique_ptr<ClassA>(new ClassA); //ok ,delete old object ,and own new
```
赋值 nullptr 也是可以的，和调用 reset() 效果相同：
```cpp
up = nullptr; //deletes the associated object ,if any
```

### 对付 Array
[ std::unique_ptr< T []>](https://en.cppreference.com/w/cpp/memory/unique_ptr/operator_at)

下面的语句是错误的：
```cpp
std::unique_ptr<std::string> up(new std::string[10]); //runtime error
// 对于 array 应该使用 delete[] 而不是 delete
```

现在，你可能会想，如果采用 class shared_ptr ，那么你必须定义自己的 delete 才能处理 array。其实不必。

c++ STL 为 class unique_ptr 提供了一个偏特化版本用来处理 array,这个版本会在遗失其所指对象的拥有权时，对该对象调用 delete[]。
```cpp
std::unique_ptr<std::string[]> up(new std::string[10]); //ok
```
然而请注意，这个偏特化版本提供的接口稍有不同。它不再提供操作符 * 和 -> ,改而提供 [] ,用以访问指向 array 中的某一个对象：
```cpp
std::unique_ptr<std::string[]> up(new std::string[10]); //ok

...
std::cout<< *up <<std::endl; //error: not defined for array
std::cout<< up[0] << std::endl; //ok
```
也请注意，这个 class 不接受一个 “派生类型” 的array 作为初值。这带来的影响是：在 朴素直率的 (plain) array 身上起不了多态作用。

[std::unique_ptr<T[]> with an array of derived objects, use of deleted function](https://stackoverflow.com/questions/33461724/stdunique-ptrt-with-an-array-of-derived-objects-use-of-deleted-function)

### 使用自定义的 deleter
```cpp
std::unique_ptr<D, std::function<void(D*)>> p(new D, [](D* ptr)
       {
           std::cout << "destroying from a custom deleter...\n";
           delete ptr;
       });  // p owns D
   p->bar();
```

## class shared_ptr
[class shared_ptr](https://en.cppreference.com/w/cpp/memory/shared_ptr) 实现共享式拥有的概念。多个 smart pointer 可以指向相同对象，该对象和其相关资源会在“最后一个 reference 被销毁”时被释放。为了在结构较复杂的情境中执行上述操作，STL 还提供了 [weak_ptr](https://en.cppreference.com/w/cpp/memory/weak_ptr), [bad_weak_ptr](https://en.cppreference.com/w/cpp/memory/bad_weak_ptr), [enable_shared_from_this](https://en.cppreference.com/w/cpp/memory/enable_shared_from_this)等辅助类
shared_ptr sample:
```c++
// come from https://en.cppreference.com/w/cpp/memory/shared_ptr
#include <iostream>
#include <memory>
#include <thread>
#include <chrono>
#include <mutex>

struct Base
{
    Base() { std::cout << "  Base::Base()\n"; }
    // Note: non-virtual destructor is OK here
    ~Base() { std::cout << "  Base::~Base()\n"; }
};

struct Derived: public Base
{
    Derived() { std::cout << "  Derived::Derived()\n"; }
    ~Derived() { std::cout << "  Derived::~Derived()\n"; }
};

void thr(std::shared_ptr<Base> p)
{
    std::this_thread::sleep_for(std::chrono::seconds(1));
    std::shared_ptr<Base> lp = p; // thread-safe, even though the
                                  // shared use_count is incremented
    {
        static std::mutex io_mutex;
        std::lock_guard<std::mutex> lk(io_mutex);
        std::cout << "local pointer in a thread:\n"
                  << "  lp.get() = " << lp.get()
                  << ", lp.use_count() = " << lp.use_count() << '\n';
    }
}

int main()
{
    std::shared_ptr<Base> p = std::make_shared<Derived>();

    std::cout << "Created a shared Derived (as a pointer to Base)\n"
              << "  p.get() = " << p.get()
              << ", p.use_count() = " << p.use_count() << '\n';
    std::thread t1(thr, p), t2(thr, p), t3(thr, p);
    p.reset(); // release ownership from main
    std::cout << "Shared ownership between 3 threads and released\n"
              << "ownership from main:\n"
              << "  p.get() = " << p.get()
              << ", p.use_count() = " << p.use_count() << '\n';
    t1.join(); t2.join(); t3.join();
    std::cout << "All threads completed, the last one deleted Derived\n";
}
```

run it:
```sh
Base::Base()
Derived::Derived()
Created a shared Derived (as a pointer to Base)
p.get() = 0x10a2c30, p.use_count() = 1
Shared ownership between 3 threads and released
ownership from main:
p.get() = 0, p.use_count() = 0
local pointer in a thread:
lp.get() = 0x10a2c30, lp.use_count() = 4
local pointer in a thread:
lp.get() = 0x10a2c30, lp.use_count() = 4
local pointer in a thread:
lp.get() = 0x10a2c30, lp.use_count() = 2
Derived::~Derived()
Base::~Base()
All threads completed, the last one deleted Derived
```

### 对付 array
请注意， shared_ptr 提供的 default deleter 调用的是 delete,不是 delete []。
```c++
std::shared_ptr<int> p(new int[10]); //error,but compiles
```
不过你可以这样：
```c++
std::shared_ptr<int> p(new int[10],
						[](int* p){
						delete [] p;
								});
```
也可以这样：
```c++
std::shared_ptr<int> p(new int[10],
						std::default_delete<int[]>());
```
当然也可以使用：
```c++
std::unique_ptr<int[]> p(new int[10]);
```
也请注意，shared_ptr 不提供 operator []。至于 unique_ptr ，它有一个针对 array 的偏特化版本，详见 [unique_ptr 对付 Array](#对付-array)

### shared_ptr 环形指向
如果两对象使用 shared_ptr 互相指向对方，而一旦不存在其他 reference 指向它们时，你想释放它们和其相应资源。这种情况下 shared_ptr 不会释放数据，因为每个对象的 use_count() 仍是 1。
```c++
#include <iostream>
#include <memory>
#include <vector>
#include <string>

class Person{
public:
  std::string name;
  std::shared_ptr<Person> mother;
  std::shared_ptr<Person> father;
  std::vector<std::shared_ptr<Person>> kids;

  Person(const std::string& n,
  std::shared_ptr<Person> m=nullptr,
std::shared_ptr<Person> f=nullptr):name(n),mother(m),father(f)
{}
  ~Person()
  {
    std::cout<<"delete "<<name<<std::endl;
  }
};

std::shared_ptr<Person> initFamily(const std::string& name)
{
  std::shared_ptr<Person> mom(new Person(name+"'s mom"));
  std::shared_ptr<Person> dad(new Person(name+"'s dad"));
  std::shared_ptr<Person> kid(new Person(name,mom,dad));
  mom->kids.push_back(kid);
  dad->kids.push_back(kid);
  return kid;
}

int main(int argc, char const *argv[]) {
  std::shared_ptr<Person> p=initFamily("nico");

  std::cout<<"nico 's family exists"<<std::endl;
  std::cout<<"- nico is shared "<<p.use_count()<<" times"<<std::endl;
  std::cout<<"- name of 1st kid of nico's mom :"
  <<p->mother->kids[0]->name<<std::endl;

  return 0;
}
```

Run it:
```sh
nico 's family exists
- nico is shared 3 times
- name of 1st kid of nico's mom :nico
```

注，参考 《c++ 标准库第二版》 第五章

[上一级](base.md)
[上一篇](regex.md)
[下一篇](stream_IO.md)
