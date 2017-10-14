# 初识java

<!-- @import "[TOC]" {cmd="toc" depthFrom=1 depthTo=6 orderedList=false} -->
<!-- code_chunk_output -->

* [初识java](#初识java)
	* [继承和多态](#继承和多态)
		* [继承的概念与实现](#继承的概念与实现)
		* [super 关键字](#super-关键字)
		* [多态和动态绑定](#多态和动态绑定)
			* [动态绑定](#动态绑定)
		* [instanceof 运算符](#instanceof-运算符)
		* [多态对象的类型转换](#多态对象的类型转换)
		* [static关键字以及静态变量和静态方法](#static关键字以及静态变量和静态方法)
			* [静态初始器（静态块）](#静态初始器静态块)
			* [静态导入](#静态导入)
		* [final关键字：阻止继承和多态](#final关键字阻止继承和多态)
	* [Object类](#object类)
			* [equals()](#equals)
			* [hashCode()](#hashcode)
			* [toString()](#tostring)

<!-- /code_chunk_output -->

## 继承和多态

### 继承的概念与实现

继承是类与类之间的关系，是一个很简单很直观的概念，与现实世界中的继承（例如儿子继承父亲财产）类似。

继承可以理解为一个类从另一个类获取方法和属性的过程。如果类B继承于类A，那么B就拥有A的方法和属性。

继承使用 extends 关键字。

### super 关键字

super 关键字与 this 类似，this 用来表示当前类的实例，super 用来表示父类。

super 可以用在子类中，通过点号(.)来获取父类的成员变量和方法。super 也可以用在子类的子类中，Java 能自动向上层类追溯。

父类行为被调用，就好象该行为是本类的行为一样，而且调用行为不必发生在父类中，它能自动向上层类追溯。

super 关键字的功能：
1. 点取已经覆盖了的方法。
2. 作为方法名表示父类构造方法。
* 在构造方法中调用另一个构造方法，调用动作必须置于最起始的位置。
* 不能在构造方法以外的任何方法内调用构造方法。
* 在一个构造方法内只能调用一个构造方法。

### 多态和动态绑定

在Java中，父类的变量可以引用父类的实例，也可以引用子类的实例

```java
public class Demo {
    public static void main(String[] args){
        Animal obj = new Animal();
        obj.cry();
        obj = new Cat();
        obj.cry();
        obj = new Dog();
        obj.cry();
    }
}
class Animal{
    // 动物的叫声
    public void cry(){
        System.out.println("不知道怎么叫");
    }

}
class Cat extends Animal{
    // 猫的叫声
    public void cry(){
        System.out.println("喵喵~");
    }
}
class Dog extends Animal{
    // 狗的叫声
    public void cry(){
        System.out.println("汪汪~");
    }
}
```

结果：
```
不知道怎么叫
喵喵~
汪汪~
```


* 多态存在的三个必要条件：要有继承、要有重写、父类变量引用子类对象。


#### 动态绑定

为了理解多态的本质，下面讲一下Java调用方法的详细流程。

1) 编译器查看对象的声明类型和方法名。

假设调用 obj.func(param)，obj 为 Cat 类的对象。需要注意的是，有可能存在多个名字为func但参数签名不一样的方法。例如，可能存在方法 func(int) 和 func(String)。编译器将会一一列举所有 Cat 类中名为func的方法和其父类 Animal 中访问属性为 public 且名为func的方法。

这样，编译器就获得了所有可能被调用的候选方法列表。

2) 接下来，编泽器将检查调用方法时提供的参数签名。

如果在所有名为func的方法中存在一个与提供的参数签名完全匹配的方法，那么就选择这个方法。这个过程被称为重载解析(overloading resolution)。例如，如果调用 func("hello")，编译器会选择 func(String)，而不是 func(int)。由于自动类型转换的存在，例如 int 可以转换为 double，如果没有找到与调用方法参数签名相同的方法，就进行类型转换后再继续查找，如果最终没有匹配的类型或者有多个方法与之匹配，那么编译错误。

这样，编译器就获得了需要调用的方法名字和参数签名。

3) 如果方法的修饰符是private、static、final（static和final将在后续讲解），或者是构造方法，那么编译器将可以准确地知道应该调用哪个方法，我们将这种调用方式 称为静态绑定(static binding)。

与此对应的是，调用的方法依赖于对象的实际类型， 并在运行时实现动态绑。例如调用 func("hello")，编泽器将采用动态绑定的方式生成一条调用 func(String) 的指令。

4)当程序运行，并且釆用动态绑定调用方法时，JVM一定会调用与 obj 所引用对象的实际类型最合适的那个类的方法。我们已经假设 obj 的实际类型是 Cat，它是 Animal 的子类，如果 Cat 中定义了 func(String)，就调用它，否则将在 Animal 类及其父类中寻找。

每次调用方法都要进行搜索，时间开销相当大，因此，JVM预先为每个类创建了一个方法表(method lable)，其中列出了所有方法的名称、参数签名和所属的类。这样一来，在真正调用方法的时候，虚拟机仅查找这个表就行了。在上面的例子中，JVM 搜索 Cat 类的方法表，以便寻找与调用 func("hello") 相匹配的方法。这个方法既有可能是 Cat.func(String)，也有可能是 Animal.func(String)。注意，如果调用super.func("hello")，编译器将对父类的方法表迸行搜索。

### instanceof 运算符

多态性带来了一个问题，就是如何判断一个变量所实际引用的对象的类型 。 C++使用runtime-type information(RTTI)，Java 使用 instanceof 操作符。

instanceof 运算符用来判断一个变量所引用的对象的实际类型，注意是它引用的对象的类型，不是变量的类型。请看下面的代码：
```java
public final class Demo{
    public static void main(String[] args) {
        // 引用 People 类的实例
        People obj = new People();
        if(obj instanceof Object){
            System.out.println("我是一个对象");
        }
        if(obj instanceof People){
            System.out.println("我是人类");
        }
        if(obj instanceof Teacher){
            System.out.println("我是一名教师");
        }
        if(obj instanceof President){
            System.out.println("我是校长");
        }

        System.out.println("-----------");  // 分界线

        // 引用 Teacher 类的实例
        obj = new Teacher();
        if(obj instanceof Object){
            System.out.println("我是一个对象");
        }
        if(obj instanceof People){
            System.out.println("我是人类");
        }
        if(obj instanceof Teacher){
            System.out.println("我是一名教师");
        }
        if(obj instanceof President){
            System.out.println("我是校长");
        }
    }
}

class People{ }
class Teacher extends People{ }
class President extends Teacher{ }
```

运行结果：
```
我是一个对象
我是人类
-----------
我是一个对象
我是人类
我是一名教师
```

可以看出，如果变量引用的是当前类或它的子类的实例，instanceof 返回 true，否则返回 false


### 多态对象的类型转换

这里所说的对象类型转换，是指存在继承关系的对象，不是任意类型的对象。当对不存在继承关系的对象进行强制类型转换时，java 运行时将抛出 java.lang.ClassCastException 异常。

在继承链中，我们将子类向父类转换称为“向上转型”，将父类向子类转换称为“向下转型”。

很多时候，我们会将变量定义为父类的类型，却引用子类的对象，这个过程就是向上转型。程序运行时通过动态绑定来实现对子类方法的调用，也就是多态性。

然而有些时候为了完成某些父类没有的功能，我们需要将向上转型后的子类对象再转成子类，调用子类的方法，这就是向下转型。

* 注意：不能直接将父类的对象强制转换为子类类型，只能将向上转型后的子类对象再次转换为子类类型。
* 子类对象必须向上转型后，才能再向下转型。

```java
public class Demo {
    public static void main(String args[]) {
        SuperClass superObj = new SuperClass();
        SonClass sonObj = new SonClass();
        // 下面的代码运行时会抛出异常，不能将父类对象直接转换为子类类型
        // SonClass sonObj2 = (SonClass)superObj;
        // 先向上转型，再向下转型
        superObj = sonObj;
        SonClass sonObj1 = (SonClass)superObj;
    }
}
class SuperClass{ }
class SonClass extends SuperClass{ }
```
将第7行的注释去掉，运行时会抛出异常，但是编译可以通过。

因为向下转型存在风险，所以在接收到父类的一个引用时，请务必使用 instanceof 运算符来判断该对象是否是你所要的子类，请看下面的代码：

```java
public class Demo {
    public static void main(String args[]) {
        SuperClass superObj = new SuperClass();
        SonClass sonObj = new SonClass();
        // superObj 不是 SonClass 类的实例
        if(superObj instanceof SonClass){
            SonClass sonObj1 = (SonClass)superObj;
        }else{
            System.out.println("①不能转换");
        }
        superObj = sonObj;
        // superObj 是 SonClass 类的实例
        if(superObj instanceof SonClass){
            SonClass sonObj2 = (SonClass)superObj;
        }else{
            System.out.println("②不能转换");
        }
    }
}
class SuperClass{ }
class SonClass extends SuperClass{ }
```

结果显示：
```
①不能转换
```

总结：对象的类型转换在程序运行时检查，向上转型会自动进行，向下转型的对象必须是当前引用类型的子类。

### static关键字以及静态变量和静态方法

静态变量和静态方法能够通过类名来访问，不需要创建一个类的对象来访问该类的静态成员，所以static修饰的成员又称作类变量和类方法。静态变量与实例变量不同，实例变量总是通过对象来访问，因为它们的值在对象和对象之间有所不同。


关于静态变量和静态方法的总结：
* 一个类的静态方法只能访问静态变量；
* 一个类的静态方法不能够直接调用非静态方法；
* 如访问控制权限允许，静态变量和静态方法也可以通过对象来访问，但是不被推荐；
* 静态方法中不存在当前对象，因而不能使用 this，当然也不能使用 super；
* 静态方法不能被非静态方法覆盖；
* 构造方法不允许声明为 static 的；
* 局部变量不能使用static修饰。

#### 静态初始器（静态块）

块是由大括号包围的一段代码。静态初始器(Static Initializer)是一个存在于类中、方法外面的静态块。静态初始器仅仅在类装载的时候（第一次使用类的时候）执行一次，往往用来初始化静态变量。

示例代码：
```java
public class Demo {
    public static int i;
    static{
        i = 10;
        System.out.println("Now in static block.");
    }
    public void test() {
        System.out.println("test method: i=" + i);
    }
    public static void main(String[] args) {
        System.out.println("Demo.i=" + Demo.i);
        new Demo().test();
    }
}
```

运行结果:
```
Now in static block.
Demo.i=10
test method: i=10
```

#### 静态导入

静态导入是 Java 5 的新增特性，用来导入类的静态变量和静态方法。


一般我们导入类都这样写：
```java
import packageName.className;  // 导入某个特定的类
//或
import packageName.*;  // 导入包中的所有类

//而静态导入可以这样写：
import static packageName.className.methonName;  // 导入某个特定的静态方法
//或
import static packageName.className.*;  // 导入类中的所有静态成员
```

导入后，可以在当前类中直接用方法名调用静态方法，不必再用 className.methodName 来访问。

对于使用频繁的静态变量和静态方法，可以将其静态导入。静态导入的好处是可以简化一些操作，例如输出语句 System.out.println(); 中的 out 就是 System 类的静态变量，可以通过 import static java.lang.System.*; 将其导入，下次直接调用 out.println() 就可以了。

代码示例：
```java
import static java.lang.System.*;
import static java.lang.Math.random;
public class Demo {
    public static void main(String[] args) {
        out.println("产生的一个随机数：" + random());
    }
}
```

运行结果：
```
0.05800891549018705
```

### final关键字：阻止继承和多态

在 Java 中，声明类、变量和方法时，可使用关键字 final 来修饰。final 所修饰的数据具有“终态”的特征，表示“最终的”意思。具体规定如下：
* final 修饰的类不能被继承。
* final 修饰的方法不能被子类重写。
* final 修饰的变量（成员变量或局部变量）即成为常量，只能赋值一次。
* final 修饰的成员变量必须在声明的同时赋值，如果在声明的时候没有赋值，那么只有 一次赋值的机会，而且只能在构造方法中显式赋值，然后才能使用。
* final 修饰的局部变量可以只声明不赋值，然后再进行一次性的赋值。

final 一般用于修饰那些通用性的功能、实现方式或取值不能随意被改变的数据，以避免被误用，例如实现数学三角方法、幂运算等功能的方法，以及数学常量π=3.141593、e=2.71828 等。

事实上，为确保终态性，提供了上述方法和常量的 java.lang.Math 类也已被定义为final 的。

需要注意的是，如果将引用类型（任何类的类型）的变量标记为 final，那么该变量不能指向任何其它对象。但可以改变对象的内容，因为只有引用本身是 final 的。

final 也可以用来修饰类（放在 class 关键字前面），阻止该类再派生出子类，例如 Java.lang.String 就是一个 final 类。这样做是出于安全原因，因为要保证一旦有字符串的引用，就必须是类 String 的字符串，而不是某个其它类的字符串（String 类可能被恶意继承并篡改）。

方法也可以被 final 修饰，被 final 修饰的方法不能被覆盖；变量也可以被 final 修饰，被 final 修饰的变量在创建对象以后就不允许改变它们的值了。一旦将一个类声明为 final，那么该类包含的方法也将被隐式地声明为 final，但是变量不是。

* 被 final 修饰的方法为静态绑定，不会产生多态（动态绑定），程序在运行时不需要再检索方法表，能够提高代码的执行效率。在Java中，被 static 或 private 修饰的方法会被隐式的声明为 final，因为动态绑定没有意义。

由于动态绑定会消耗资源并且很多时候没有必要，所以有一些程序员认为：除非有足够的理由使用多态性，否则应该将所有的方法都用 final 修饰。

这样的认识未免有些偏激，因为 JVM 中的即时编译器能够实时监控程序的运行信息，可以准确的知道类之间的继承关系。如果一个方法没有被覆盖并且很短，编译器就能够对它进行优化处理，这个过程为称为内联(inlining)。例如，内联调用 e.getName()将被替换为访问e.name变量。这是一项很有意义的改进，这是由于CPU在处理调用方法的指令时，使用的分支转移会扰乱预取指令的策略，所以，这被视为不受欢迎的。然而，如果 getName() 在另外一个类中被覆盖，那么编译器就无法知道覆盖的代码将会做什么操作，因此也就不能对它进行内联处理了。

## Object类

Object 类位于 java.lang 包中，是所有 Java 类的祖先，Java 中的每个类都由它扩展而来。

定义Java类时如果没有显示的指明父类，那么就默认继承了 Object 类。例如：

```java
public class Demo{
    // ...
}
//实际上是下面代码的简写形式：
public class Demo extends Object{
    // ...
}
```

在Java中，只有基本类型不是对象，例如数值、字符和布尔型的值都不是对象，所有的数组类型，不管是对象数组还是基本类型数组都是继承自 Object 类。

Object 类定义了一些有用的方法，由于是根类，这些方法在其他类中都存在，一般是进行了重载或覆盖，实现了各自的具体功能。


#### equals()
Object 类中的 equals() 方法用来检测一个对象是否等价于另外一个对象，语法为：
    public boolean equals(Object obj)

```java
obj1.equals(obj2);
```
在Java中，数据等价的基本含义是指两个数据的值相等。在通过 equals() 和“==”进行比较的时候，引用类型数据比较的是引用，即内存地址，基本数据类型比较的是值。

注意：
* equals()方法只能比较引用类型，“ == ”可以比较引用类型及基本类型。
* 当用 equals() 方法进行比较时，对类 File、String、Date 及包装类来说，是比较类型及内容而不考虑引用的是否是同一个实例。
* 用“==”进行比较时，符号两边的数据类型必须一致（可自动转换的数据类型除外），否则编译出错，而用 equals 方法比较的两个数据只要都是引用类型即可。

#### hashCode()

散列码(hashCode)是按照一定的算法由对象得到的一个数值，散列码没有规律。如果 x 和 y 是不同的对象，x.hashCode() 与 y.hashCode() 基本上不会相同。

hashCode() 方法主要用来在集合中实现快速查找等操作，也可以用于对象的比较。

在 Java 中，对 hashCode 的规定如下：
在同一个应用程序执行期间，对同一个对象调用 hashCode()，必须返回相同的整数结果——前提是 equals() 所比较的信息都不曾被改动过。至于同一个应用程序在不同执行期所得的调用结果，无需一致。
如果两个对象被 equals() 方法视为相等，那么对这两个对象调用 hashCode() 必须获得相同的整数结果。
如果两个对象被 equals() 方法视为不相等，那么对这两个对象调用 hashCode() 不必产生不同的整数结果。然而程序员应该意识到，对不同对象产生不同的整数结果，有可能提升hashTable（后面会学到，集合框架中的一个类）的效率。

简单地说：如果两个对象相同，那么它们的 hashCode 值一定要相同；如果两个对象的 hashCode 值相同，它们并不一定相同。在 Java 规范里面规定，一般是覆盖 equals() 方法应该连带覆盖 hashCode() 方法。

#### toString()

toString() 方法是 Object 类中定义的另一个重要方法，是对象的字符串表现形式，语法为：
* public String toString()
返回值是 String 类型，用于描述当前对象的有关信息。Object 类中实现的 toString() 方法是返回当前对象的类型和内存地址信息，但在一些子类（如 String、Date 等）中进行了 重写，也可以根据需要在用户自定义类型中重写 toString() 方法，以返回更适用的信息。


[参考链接](http://www.weixueyuan.net/java/)
[上一级](base.md)
