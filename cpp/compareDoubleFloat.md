# 判断浮点数是否相等


不正确的做法：

```c++
bool CompareDoubles1(double A,double B)
{
  return A==B;
}
```

一个简单的做法：

```c++
#include <limits>

bool AreSame(double a,double b)
{
  return fabs(a-b) < std::numeric_limits<double>::epsilon();
}
```

c++ 显示 epsilon:
```c++
#include <iostream>
#include <limits>

using namespace std;

int main(int argc, char const *argv[]) {
  std::cout << "double:" << std::numeric_limits<double>::epsilon() << '\n';
  std::cout << "float:" << std::numeric_limits<float>::epsilon() << '\n';
  return 0;
}
```

Run it:
```sh
double:2.22045e-16
float:1.19209e-07
```

[What is the most effective way for float and double comparison?
](https://stackoverflow.com/questions/17333/what-is-the-most-effective-way-for-float-and-double-comparison)

[std :: numeric_limits :: epsilon()](http://en.cppreference.com/w/cpp/types/numeric_limits/epsilon)

[geeksforgeeks.org/cpp-program-to-find-machine-epsilon](https://www.geeksforgeeks.org/cpp-program-to-find-machine-epsilon/)
---
- [上一级](README.md)
- 上一篇 -> [CMake](cmake.md)
- 下一篇 -> [const限定符](const.md)
