# 设计模式


<!-- @import "[TOC]" {cmd="toc" depthFrom=1 depthTo=6 orderedList=false} -->
<!-- code_chunk_output -->

* [设计模式](#设计模式)
	* [创建型模式](#创建型模式)
		* [Factory 模式](#factory-模式)
		* [Abstract factory 模式](#abstract-factory-模式)
		* [Singleton 模式](#singleton-模式)

<!-- /code_chunk_output -->

参考链接：
[设计模式精解－GoF 23种设计模式解析](https://manyones.files.wordpress.com/2010/07/dp-2nd.pdf)
[设计模式 wiki](https://zh.wikipedia.org/zh-cn/%E8%AE%BE%E8%AE%A1%E6%A8%A1%E5%BC%8F)

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
[示例代码参考链接](https://sourcemaking.com/design_patterns/factory_method/cpp/1)

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

after:
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

[示例代码参考链接](https://sourcemaking.com/design_patterns/singleton/cpp/1)
[上一级](base.md)
[上一篇](conv_string_to_char_pointer.md)
[下一篇](develop_care_detail.md)
