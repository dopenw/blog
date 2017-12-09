# 设计模式


<!-- @import "[TOC]" {cmd="toc" depthFrom=1 depthTo=6 orderedList=false} -->
<!-- code_chunk_output -->

* [设计模式](#设计模式)
	* [创建型模式](#创建型模式)
		* [Factory 模式](#factory-模式)
		* [Abstract factory 模式](#abstract-factory-模式)
		* [Singleton 模式](#singleton-模式)
		* [Builder 模式](#builder-模式)
		* [object pool 模式](#object-pool-模式)
		* [Prototype 模式](#prototype-模式)

<!-- /code_chunk_output -->

参考链接：
[设计模式精解－GoF 23种设计模式解析](https://manyones.files.wordpress.com/2010/07/dp-2nd.pdf)
[设计模式 wiki](https://zh.wikipedia.org/zh-cn/%E8%AE%BE%E8%AE%A1%E6%A8%A1%E5%BC%8F)
[design patterns](https://sourcemaking.com/design_patterns)

## 创建型模式

在软件工程中，创建型模式是处理对象创建机制的设计模式，试图以适合情况的方式创建对象。对象创建的基本形式可能会导致设计问题或增加设计的复杂性。创建型模式通过控制这个对象创建来解决这个问题。

### Factory 模式
工厂方法模式（英语：Factory method pattern）是一种实现了“工厂”概念的面向对象设计模式。就像其他创建型模式一样，它也是处理在不指定对象具体类型的情况下创建对象的问题。

工厂方法模式的实质是：
* 定义一个创建对象的接口，但让实现这个接口的类来决定实例化哪个类。
* 工厂方法让类的实例化推迟到子类中进行。
在父类中并不知道具体要实例化哪一个具体的子类。这里的意思为：假设我们在类 A 中要使用到类 B，B 是一个抽象父类，在 A 中并不知道具体要实例化那一个 B 的子类，但是在类 A 的子类 D 中是可以知道的。在 A 中我们没有办法直接使用类似于 new ×××的语句，因为根本就不知道×××是什么。

创建一个对象常常需要复杂的过程，所以不适合包含在一个复合对象中。创建对象可能会导致大量的重复代码，可能会需要复合对象访问不到的信息，也可能提供不了足够级别的抽象，还可能并不是复合对象概念的一部分。工厂方法模式通过定义一个单独的创建对象的方法来解决这些问题。由子类实现这个方法来创建具体类型的对象。

对象创建中的有些过程包括决定创建哪个对象、管理对象的生命周期，以及管理特定对象的建立和销毁的概念。

[wiki link](https://zh.wikipedia.org/wiki/%E5%B7%A5%E5%8E%82%E6%96%B9%E6%B3%95)

Before:
```c++
class Stooge
{
  public:
    virtual void slap_stick() = 0;
};

class Larry: public Stooge
{
  public:
    void slap_stick()
    {
        cout << "Larry: poke eyes\n";
    }
};
class Moe: public Stooge
{
  public:
    void slap_stick()
    {
        cout << "Moe: slap head\n";
    }
};
class Curly: public Stooge
{
  public:
    void slap_stick()
    {
        cout << "Curly: suffer abuse\n";
    }
};

int main()
{
  vector<Stooge*> roles;
  int choice;

  while (true)
  {
    cout << "Larry(1) Moe(2) Curly(3) Go(0): ";
    cin >> choice;
    if (choice == 0)
      break;
    else if (choice == 1)
      roles.push_back(new Larry);
    else if (choice == 2)
      roles.push_back(new Moe);
    else
      roles.push_back(new Curly);
  }
  for (int i = 0; i < roles.size(); i++)
    roles[i]->slap_stick();
  for (int i = 0; i < roles.size(); i++)
    delete roles[i];
}
```

output:
```sh
Larry(1) Moe(2) Curly(3) Go(0): 2
Larry(1) Moe(2) Curly(3) Go(0): 1
Larry(1) Moe(2) Curly(3) Go(0): 3
Larry(1) Moe(2) Curly(3) Go(0): 0
Moe: slap head
Larry: poke eyes
Curly: suffer abuse
```

after:
```c++
class Stooge
{
public:
  //factory method
  static Stooge * make_stooge(int choice);
  virtual void slap_stick()=0;
};

int main(int argc, char const *argv[]) {
  std::vector<Stooge * > roles;
  int choice;
  while (true) {
    std::cout << "Larry(1) Moe(2) Curly(3) Go(0):" << '\n';
    cin>>choice;
    if(choice==0)
    break;
    roles.push_back(Stooge::make_stooge(choice));
  }

  for (size_t i = 0; i < roles.size(); i++) {
    roles[i]->slap_stick();
  }
  for (size_t i = 0; i < roles.size(); i++) {
    delete roles[i];
  }
  return 0;
}

class Larry: public Stooge
{
  public:
    void slap_stick()
    {
        cout << "Larry: poke eyes\n";
    }
};
class Moe: public Stooge
{
  public:
    void slap_stick()
    {
        cout << "Moe: slap head\n";
    }
};
class Curly: public Stooge
{
  public:
    void slap_stick()
    {
        cout << "Curly: suffer abuse\n";
    }
};

Stooge *Stooge::make_stooge(int choice)
{
  if (choice == 1)
    return new Larry;
  else if (choice == 2)
    return new Moe;
  else
    return new Curly;
}

//care
// deleting object of abstract class type ‘Stooge’ which has non-virtual destructor will cause undefined behavior
```

output:
```sh
Larry(1) Moe(2) Curly(3) Go(0):
1
Larry(1) Moe(2) Curly(3) Go(0):
2
Larry(1) Moe(2) Curly(3) Go(0):
3
Larry(1) Moe(2) Curly(3) Go(0):
0
Larry: poke eyes
Moe: slap head
Curly: suffer abuse
```
[示例代码链接](https://sourcemaking.com/design_patterns/factory_method/cpp/1)

### Abstract factory 模式
抽象工厂模式提供了一种方式，可以将一组具有同一主题的单独的工厂封装起来。

在正常使用中，客户端程序需要创建抽象工厂的具体实现，然后使用抽象工厂作为接口来创建这一主题的具体对象。客户端程序不需要知道（或关心）它从这些内部的工厂方法中获得对象的具体类型，因为客户端程序仅使用这些对象的通用接口。抽象工厂模式将一组对象的实现细节与他们的一般使用分离开来。

“工厂”是创建产品（对象）的地方，其目的是将产品的创建与产品的使用分离。抽象工厂模式的目的，是将若干抽象产品的接口与不同主题产品的具体实现分离开。

抽象工厂模式的实质是:
* 提供接口，创建一系列相关或独立的对象，而不指定这些对象的具体类。

我们的产品结构是这样的:
```c++
class Button; // Abstract Class

class MacButton: public Button {};

class WinButton: public Button {};

class Border; // Abstract Class

class MacBorder: public Border {};

class WinBorder: public Border {};
```

对应的工厂是这样的:
```c++
class AbstractFactory {
public:
    virtual Button* CreateButton() =0;
    virtual Border* CreateBorder() =0;
};

class MacFactory: public AbstractFactory {
public:
    MacButton* CreateButton() { return new MacButton; }
    MacBorder* CreateBorder() { return new MacBorder; }
};

class WinFactory: public AbstractFactory {
public:
    WinButton* CreateButton() { return new WinButton; }
    WinBorder* CreateBorder() { return new WinBorder; }
};
```
那么客户可以根据需要选择 Mac 风格或者 Win 风格來創建 Button 或 Border:
```c++
AbstractFactory* fac;
switch (style) {
case MAC:
    fac = new MacFactory;
    break;
case WIN:
    fac = new WinFactory;
    break;
}
Button* button = fac->CreateButton();
Border* border = fac->CreateBorder();
```
[抽象工厂模式 wiki](https://zh.wikipedia.org/wiki/%E6%8A%BD%E8%B1%A1%E5%B7%A5%E5%8E%82)

### Singleton 模式
单例模式，也叫单子模式，是一种常用的软件设计模式。
在应用这个模式时，单例对象的类必须保证只有一个实例存在。许多时候整个系统只需要拥有一个的全局对象，这样有利于我们协调系统整体的行为。比如在某个服务器程序中，该服务器的配置信息存放在一个文件中，这些配置数据由一个单例对象统一读取，然后服务进程中的其他对象再通过这个单例对象获取这些配置信息。这种方式简化了在复杂环境下的配置管理。

实现单例模式的思路是：一个类能返回对象一个引用(永远是同一个)和一个获得该实例的方法（必须是静态方法，通常使用getInstance这个名称）；当我们调用这个方法时，如果类持有的引用不为空就返回这个引用，如果类保持的引用为空就创建该类的实例并将实例的引用赋予该类保持的引用；同时我们还将该类的构造函数定义为私有方法，这样其他处的代码就无法通过调用该类的构造函数来实例化该类的对象，只有通过该类提供的静态方法来得到该类的唯一实例。

单例模式在多线程的应用场合下必须小心使用。如果当唯一实例尚未创建时，有两个线程同时调用创建方法，那么它们同时没有检测到唯一实例的存在，从而同时各自创建了一个实例，这样就有两个实例被构造出来，从而违反了单例模式中实例唯一的原则。 解决这个问题的办法是为指示类是否已经实例化的变量提供一个互斥锁(虽然这样会降低效率)。

[Singleton 模式 wiki](https://zh.wikipedia.org/wiki/%E5%8D%95%E4%BE%8B%E6%A8%A1%E5%BC%8F)

Before:
```c++
class GlobaClass{
	int m_value;
public:
	GlobaClass(int v=0)
	{
		m_value=v;
	}
	int get_value()
	{
		return m_value;
	}
	void set_value(int v)
	{
		m_value=v;
	}
};

GlobaClass *global_ptr=nullptr;

void foo(void)
{
	if(!global_ptr)
	{
		global_ptr=new GlobaClass;
	}
	global_ptr->set_value(1);
	std::cout << "foo:global_ptr is"<<global_ptr->get_value() << '\n';
}

void bar(void)
{
	if (!global_ptr)
	global_ptr=new GlobaClass;
	global_ptr->set_value(2);
	std::cout << "bar:global_ptr is "<<global_ptr->get_value() << '\n';
}

int main(int argc, char const *argv[]) {
	if(!global_ptr)
	global_ptr=new GlobaClass;
	std::cout << "main:global_ptr is"<<global_ptr->get_value() << '\n';
	foo();
	bar();
	return 0;
}
```

output:
```sh
main:global_ptr is0
foo:global_ptr is1
bar:global_ptr is 2
```

after:
```c++
class GlobalClass
{
    int m_value;
    static GlobalClass * s_instance;
    GlobalClass(int v = 0)
    {
        m_value = v;
    }
  public:
    int get_value()
    {
        return m_value;
    }
    void set_value(int v)
    {
        m_value = v;
    }
    static GlobalClass *instance()
    {
        if (!s_instance)
          s_instance = new GlobalClass;
        return s_instance;
    }
};

// Allocating and initializing GlobalClass's
// static data member.  The pointer is being
// allocated - not the object inself.
GlobalClass *GlobalClass::s_instance = 0;

void foo(void)
{
  GlobalClass::instance()->set_value(1);
  cout << "foo: global_ptr is " << GlobalClass::instance()->get_value() << '\n';
}

void bar(void)
{
  GlobalClass::instance()->set_value(2);
  cout << "bar: global_ptr is " << GlobalClass::instance()->get_value() << '\n';
}

int main()
{
  cout << "main: global_ptr is " << GlobalClass::instance()->get_value() << '\n';
  foo();
  bar();
}
```

output:
```sh
main: global_ptr is 0
foo: global_ptr is 1
bar: global_ptr is 2
```

[示例代码链接](https://sourcemaking.com/design_patterns/singleton/cpp/1)

上述代码在单线程中能够正常工作，不过在多线程下可采取加互斥锁的方法：
```c++
#include <future>
#include <iostream>
#include <mutex>
#include <thread>

using namespace std;
std::mutex mutexLock;

class singleton {
  singleton(int n = 0) { m_value = n; };
  static singleton *globalPtr;
  int m_value = 0;

public:
  ~singleton(){};
  int get_value() { return m_value; }
  void set_value(int n) { m_value = n; }
  static singleton *instance() {
    if (globalPtr == NULL) {
      mutexLock.lock();
      if (globalPtr == NULL)
        globalPtr = new singleton;
      mutexLock.unlock();
    }
    return globalPtr;
  }
};
singleton *singleton::globalPtr = NULL;
int main(int argc, char const *argv[]) {

  singleton * ptr;

  auto f1 = async(launch::async, [] { return singleton::instance(); });
  auto f2 = async(launch::async, [] { return singleton::instance(); });

  ptr = f1.get();
  std::cout << "p1:" << ptr->get_value() << '\n';
  std::cout << "localtion:" << ptr << '\n';
  ptr = f2.get();
  ptr->set_value(3);
  std::cout << "p1:" << ptr->get_value() << '\n';
  std::cout << "localtion:" << ptr << '\n';
  return 0;
}
```

output:
```sh
p1:0
localtion:0x7f9514000b10
p1:3
localtion:0x7f9514000b10
```

### Builder 模式
生成器模式，他可以将复杂的建造过程抽象出来，使这个抽象过程的不同实现方法可以构造出不同表现（属性）的对象。

适用性：
* 当创建复杂对象的算法应独立于该对象的组成部分以及他们的装配方式时。
* 当构造过程必须允许被构造的对象有不同的表示时。

[生成器模式 wiki](https://zh.wikipedia.org/wiki/%E7%94%9F%E6%88%90%E5%99%A8%E6%A8%A1%E5%BC%8F)

![](../images/design_patterns_201712081616_1.png)

示例代码：
```c++
#include <cstdio>
#include <cstring>
#include <iostream>

using namespace std;

enum PersistenceType { File, Queue, Pathway };

struct PersistenceAttribute {
  PersistenceType type;
  char value[30];
};

class DistrWorkPackage {
public:
  DistrWorkPackage(char * type) {
    sprintf(_desc, "Distributed work package for :%s", type);
  }
  void setFile(char * f, char * v) {
    sprintf(_temp, "\n  File(%s): %s", f, v);
    strcat(_desc, _temp);
  }
  void setQueue(char *q, char *v) {
    sprintf(_temp, "\n  Queue(%s): %s", q, v);
    strcat(_desc, _temp);
  }
  void setPathway(char *p, char *v) {
    sprintf(_temp, "\n  Pathway(%s): %s", p, v);
    strcat(_desc, _temp);
  }
  const char *getState() { return _desc; }

private:
  char _desc[200], _temp[80];
};

class Builder {
protected:
  DistrWorkPackage *_result;

public:
  virtual void configureFile(char *) = 0;
  virtual void configureQueue(char *) = 0;
  virtual void configurePathway(char *) = 0;
  DistrWorkPackage *getResult() { return _result; }
};

class UnixBuilder : public Builder {
public:
  UnixBuilder() { _result = new DistrWorkPackage("Unix"); }
  void configureFile(char *name) { _result->setFile("flatFile", name); }
  void configureQueue(char *queue) { _result->setQueue("FIFO", queue); }
  void configurePathway(char *type) { _result->setPathway("thread", type); }
};

class VmsBuilder : public Builder {
public:
  VmsBuilder() { _result = new DistrWorkPackage("Vms"); }
  void configureFile(char *name) { _result->setFile("ISAM", name); }
  void configureQueue(char *queue) { _result->setQueue("priority", queue); }
  void configurePathway(char *type) { _result->setPathway("LWP", type); }
};

class Reader {
public:
  void setBuilder(Builder *b) { _builder = b; }
  void construct(PersistenceAttribute[], int);

private:
  Builder *_builder;
};

void Reader::construct(PersistenceAttribute list[], int num) {
  for (int i = 0; i < num; i++)
    if (list[i].type == File)
      _builder->configureFile(list[i].value);
    else if (list[i].type == Queue)
      _builder->configureQueue(list[i].value);
    else if (list[i].type == Pathway)
      _builder->configurePathway(list[i].value);
}

const int NUM_ENTRIES = 6;
PersistenceAttribute input[NUM_ENTRIES] = {
    {File, "state.dat"},         {File, "config.sys"},
    {Queue, "compute"},          {Queue, "log"},
    {Pathway, "authentication"}, {Pathway, "error processing"}};

int main() {
  UnixBuilder unixBuilder;
  VmsBuilder vmsBuilder;
  Reader reader;

  reader.setBuilder(&unixBuilder);
  reader.construct(input, NUM_ENTRIES);
  cout << unixBuilder.getResult()->getState() << endl;

  reader.setBuilder(&vmsBuilder);
  reader.construct(input, NUM_ENTRIES);
  cout << vmsBuilder.getResult()->getState() << endl;
}
```
output:
```sh
Distributed work package for :Unix
  File(flatFile): state.dat
  File(flatFile): config.sys

  Queue(FIFO): compute
  Queue(FIFO): log
  Pathway(thread): authentication
  Pathway(thread): error processing
Distributed work package for :Vms
  File(ISAM): state.dat
  File(ISAM): config.sys
  Queue(priority): compute
  Queue(priority): log
  Pathway(LWP): authentication
  Pathway(LWP): error processing
```

[示例代码链接](https://sourcemaking.com/design_patterns/builder/cpp/1)

### object pool 模式

对象池模式可以提供显着的性能提升;在初始化类实例的成本高，类的实例化率高，并且在任何时候使用的实例化数量低的情况下，这是最有效的。

对象池（也称为资源池）用于管理对象缓存。有权访问对象池的客户端可以通过简单地向池中请求一个已经实例化的对象来避免创建新的对象。一般来说，池将是一个增长的池，即池本身将创建新的对象，如果池是空的，或者我们可以有一个池，限制了创建的对象的数量。 希望将当前不再使用的所有可重用对象保留在同一个对象池中，以便可以通过一个一致的策略来管理它们。为了达到这个目的，可重用池类被设计成一个单例类。


对象池允许其他人从其池中“检出”对象，当这些对象不再被它们的进程需要时，它们被返回到池中以便被重用。 但是，我们不希望进程需要等待某个特定的对象被释放，所以对象池也会根据需要实例化新的对象，但是还必须实现一个方法来定期清理未使用的对象。

[source link](https://sourcemaking.com/design_patterns/object_pool)
![](../images/design_patterns_201712081616_2.png)

python 代码示例：
```python
class ReusablePool:
    """
    Manage Reusable objects for use by Client objects.
    """

    def __init__(self, size):
        self._reusables = [Reusable() for _ in range(size)]

    def acquire(self):
        return self._reusables.pop()

    def release(self, reusable):
        self._reusables.append(reusable)


class Reusable:
    """
    Collaborate with other objects for a limited amount of time, then
    they are no longer needed for that collaboration.
    """

    pass


def main():
    reusable_pool = ReusablePool(10)
    reusable = reusable_pool.acquire()
    reusable_pool.release(reusable)


if __name__ == "__main__":
    main()
```
[示例代码链接](https://sourcemaking.com/design_patterns/object_pool/python/1)

### Prototype 模式
原型模式是创建型模式的一种，其特点在于通过「复制」一个已经存在的实例来返回新的实例,而不是新建实例。被复制的实例就是我们所称的「原型」，这个原型是可定制的。

原型模式多用于创建复杂的或者耗时的实例，因为这种情况下，复制一个已经存在的实例使程序运行更高效；或者创建值相等，只是命名不一样的同类数据。
[原型模式 wiki](https://zh.wikipedia.org/wiki/%E5%8E%9F%E5%9E%8B%E6%A8%A1%E5%BC%8F)

![](../images/design_patterns_201712081630_1.png)

```c++
#include <iostream>
#include <string>

using namespace std;
class Prototype {
protected:
  string type;
  int value;

public:
  virtual Prototype *clone() = 0;
  string getType() { return type; }
  int getValue() { return value; }
};

class ConcretePrototype1 : public Prototype {
public:
  ConcretePrototype1(int number) {
    type = "Type1";
    value = number;
  }

  Prototype *clone() { return new ConcretePrototype1(*this); }
};

class ConcretePrototype2 : public Prototype {
public:
  ConcretePrototype2(int number) {
    type = "Type2";
    value = number;
  }

  Prototype *clone() { return new ConcretePrototype2(*this); }
};

class ObjectFactory {
  static Prototype *type1value1;
  static Prototype *type1value2;
  static Prototype *type2value1;
  static Prototype *type2value2;

public:
  static void initialize() {
    type1value1 = new ConcretePrototype1(1);
    type1value2 = new ConcretePrototype1(2);
    type2value1 = new ConcretePrototype2(1);
    type2value2 = new ConcretePrototype2(2);
  }

  static Prototype *getType1Value1() { return type1value1->clone(); }

  static Prototype *getType1Value2() { return type1value2->clone(); }

  static Prototype *getType2Value1() { return type2value1->clone(); }

  static Prototype *getType2Value2() { return type2value2->clone(); }
};

Prototype *ObjectFactory::type1value1 = 0;
Prototype *ObjectFactory::type1value2 = 0;
Prototype *ObjectFactory::type2value1 = 0;
Prototype *ObjectFactory::type2value2 = 0;

int main() {
  ObjectFactory::initialize();
  Prototype *object;

  /* All the object were created by cloning the prototypes. */
  object = ObjectFactory::getType1Value1();
  std::cout << object->getType() << ": " << object->getValue() << std::endl;

  object = ObjectFactory::getType1Value2();
  std::cout << object->getType() << ": " << object->getValue() << std::endl;

  object = ObjectFactory::getType2Value1();
  std::cout << object->getType() << ": " << object->getValue() << std::endl;

  object = ObjectFactory::getType2Value2();
  std::cout << object->getType() << ": " << object->getValue() << std::endl;

  return 0;
}

```

output:
```sh
Type1: 1
Type1: 2
Type2: 1
Type2: 2
```

[示例代码链接](https://gist.github.com/pazdera/1122349)

[上一级](base.md)
[上一篇](conv_string_to_char_pointer.md)
[下一篇](develop_care_detail.md)
