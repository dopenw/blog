# 常引用与常量


## 常引用
* 如果既要利用引用提高程序的效率，又要保护传递给函数的数据不在函数中被改变，就应使用常引用。常引用

```c++
#include <iostream>

using namespace std;

int main(int argc, char const *argv[]) {
  int a;
  const int &ra = a;
  // ra = 1; error
  a = 1;
  std::cout << ra << '\n';
  return 0;
}

```

output:
```
1
```


## 常量

* 常量必须初始化

```c++
#include <iostream>

using namespace std;

int main(int argc, char const *argv[]) {
  const int a = 1;
  // int &ra = a; error
  const int &ra = a;
  // a = 1; error
  std::cout << ra << '\n';
  return 0;
}
```

output:
```
1
```
[上一级](base.md)
[下一篇](conv_string_to_char_pointer.md)
