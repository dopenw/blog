<!-- mutable的使用 -->

<!-- @import "[TOC]" {cmd="toc" depthFrom=1 depthTo=6 orderedList=false} -->
<!-- code_chunk_output -->

* [mutable的使用](#mutable的使用)
	* [使用场景一](#使用场景一)
	* [使用场景二](#使用场景二)

<!-- /code_chunk_output -->

## 使用场景一

在Lambda中为了获得passing by value 和passing by reference混合体，你可以声明lambda为mutable。
```c++
#include <iostream>

using namespace std;

int main(int argc, char const *argv[]) {
  int id = 0;
  auto f = [=]() mutable {
    std::cout << "id:" << id << '\n';
    ++id;
  };
  id = 42;
  std::cout << id << '\n';
  f();
  f();
  f();
  std::cout << id << '\n';
  return 0;
}
```
result:
```
42
id:0
id:1
id:2
42
```

## 使用场景二
在const成员函数中，用mutable修饰成员变量名后，就可以修改类的成员变量。

```c++
#include <iostream>
#include <string.h>
using std::cout;
using std::endl;

class Customer {
  char name[25];
  mutable char placedorder[50];
  int tableno;
  mutable int bill;

public:
  Customer(char * s, char * m, int a, int p) {
    strcpy(name, s);
    strcpy(placedorder, m);
    tableno = a;
    bill = p;
  }
  void changePlacedOrder(char * p) const { strcpy(placedorder, p); }
  void changeBill(int s) const { bill = s; }
  void display() const {
    cout << "Customer name is: " << name << endl;
    cout << "Food ordered by customer is: " << placedorder << endl;
    cout << "table no is: " << tableno << endl;
    cout << "Total payable amount: " << bill << endl;
  }
};

int main() {
  const Customer c1("Pravasi Meet", "Ice Cream", 3, 100);
  c1.display();
  c1.changePlacedOrder("GulabJammuns");
  c1.changeBill(150);
  c1.display();
  return 0;
}
```

result:
```
Customer name is: Pravasi Meet
Food ordered by customer is: Ice Cream
table no is: 3

Total payable amount: 100
Customer name is: Pravasi Meet
Food ordered by customer is: GulabJammuns
table no is: 3
Total payable amount: 150
```
[source link](http://www.geeksforgeeks.org/c-mutable-keyword/)

---
- [上一级](README.md)
- 上一篇 -> [c++并发](multiThread.md)
- 下一篇 -> [MyString](myString.md)
