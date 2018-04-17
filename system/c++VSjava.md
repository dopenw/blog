# C++ VS Java

1. Java 是运行在虚拟机上的，所以具备了比较好的平台移植性，C++ 也可以具有较好的移植性，前提是不使用操作系统独有的 API ，比如 linux 和 windows 下 开启线程 是不一样的。也是由于 Java 的虚拟机造成它的效率低于 C++。

2. C++ 保留了指针。指针是一个令人头痛的玩意，用好了效率提高，用的不好可能导致系统崩溃。而 Java 的虚拟机为我们把这些东西都封装好了，在使用Java 的时候虽然会报一些空指针异常，但确实增加了程序的健壮性。

3. C++ 支持多重继承。尽管多重继承功能很强，但使用复杂，而且会引起许多麻烦，编译程序实现它也很不容易。Java 不支持多重继承，但允许一个类继承多个接口(```extends + implement```),实现了 C++ 的多重继承的功能，又避免了 C++ 中的多重继承实现方式带来的诸多不便。

4. Java 是完全面向对象的语言，所有函数和变量必须是类的一部分。除了基本数据类型外，其余的都应该作为类的一部分，包括数组。对象将数据和方法结合起来，把他们封装在类中，这样每个对象都可实现自己的特点和行为。而 C++ 允许将函数和变量定义位全局的。此外，Java 中取消了 c/C++ 中的结构和联合，消除了不必要的麻烦。错误示例：
```java
int a = 3;

void tt() { System.out.println("jk"); }

public class test {
  void tk() { System.out.println("tk"); }
  public static void main(String[] args) {
    System.out.println("hello");
    test t = new test();
    t.tk();
  }
}
```
```sh
[breap@breap java]$ javac test.java
test.java:2: error: class, interface, or enum expected
int a = 3;
^

test.java:4: error: class, interface, or enum expected
void tt() { System.out.println("jk"); }
^
test.java:4: error: class, interface, or enum expected
void tt() { System.out.println("jk"); }
                                      ^
3 errors
```

5. 内存管理。我们在 C++ 中可以直接手动释放内存，但是 Java 做不到，他的垃圾是由虚拟机去回收的，不需要程序员去操作。

6. 操作符重载：Java不支持操作符重载

7. ```goto``` 语句:Java关键字列表指定了goto关键字，但它被标记为“未使用”。

8. Java 不支持 C++ 中的自动强制类型转换，若需要必须由程序员强制类型转换

参考链接：
* [Java和C++ C比较](https://blog.csdn.net/codingandroid/article/details/8595949)
* [Is there a goto statement in Java?](https://stackoverflow.com/questions/2545103/is-there-a-goto-statement-in-java?utm_medium=organic&utm_source=google_rich_qa&utm_campaign=google_rich_qa)
* [Operator overloading in Java](https://stackoverflow.com/questions/1686699/operator-overloading-in-java/1686714)
[上一级](base.md)
[上一篇](UML.md)
[下一篇](dynamicProgramming.md)
