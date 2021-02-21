# 当类中含有const，reference成员变量,其构造函数都需要初始化列表

1. 类中含有const，但不对其初始化(error)
```c++
#include <iostream>

using namespace std;

class test {
public:
  // test(int a) : m_a(a){};
  test(){};
  // test(int a) { m_a = a; } err;
  ~test() { std::cout << m_a << '\n'; }

private:
  const int m_a;
};

int main(int argc, char const *argv[]) {
  test tt;
  return 0;
}

```

output:
```sh
nitalization_list.cpp: In constructor ‘test::test()’:
initalization_list.cpp:8:3: error: uninitialized const member in ‘const int’ [-fpermissive]
   test(){};
   ^~~~
initalization_list.cpp:13:13: note: ‘const int test::m_a’ should be initialized
   const int m_a;
```

2. 使用初始化列表
```c++
#include <iostream>

using namespace std;

class test {
public:
  test(int a) : m_a(a){};
  // test(int a) { m_a = a; } // err;
  ~test() { std::cout << m_a << '\n'; }

private:
  const int m_a;
};

int main(int argc, char const *argv[]) {
  test tt(3);
  return 0;
}

```
[上一级](README.md)
[上一篇 -> Google c++ test framework](google_test_framework.md)
[下一篇 -> c++ 内部类](inner_class.md)
