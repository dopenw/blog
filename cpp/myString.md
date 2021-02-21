# MyString

```c++
#include <algorithm>
#include <cstdlib>
#include <iostream>
#include <string.h>

class MyString {
public:
  MyString(const char * str = NULL);
  MyString(const MyString &other);
  MyString &operator=(const MyString &other);
  ~MyString() { delete[] m_data; };

private:
  char *m_data;
};

MyString::MyString(const char *str) {
  if (str == NULL) {
    m_data = new char[1];
    * m_data = '\0';
  } else {
    m_data = new char[strlen(str) + 1];
    strcpy(m_data, str);
  }
}

MyString::MyString(const MyString &data) {
  // care
  m_data = new char[strlen(data.m_data) + 1];
  strcpy(m_data, data.m_data);
}

MyString &MyString::operator=(const MyString &data) {
  //pass
}

int main(int argc, char const *argv[]) {
  MyString test("123");
  return 0;
}
```

经典的解法：

```c++
MyString &MyString::operator=(const MyString &data) {
  if (this != &data)
  {
    delete[] m_data;
    m_data = nullptr;
    m_data = new char[strlen(data.m_data) + 1];
    strcpy(m_data, data.m_data);
  }
  return * this;
}
```

考虑异常安全性的解法：

```c++
MyString &MyString::operator=(const MyString &data) {
  if (this != &data)
  {
    MyString strTemp(data);
    char * pTemp = strTemp.m_data;
    strTemp.m_data = m_data;
    m_data = pTemp;
  }
  return * this;
}
```

由于strTemp.m_data指向的内存就是实例之前的m_data的内存，当程序出了if作用域后，就会自动调用析构函数释放实例的内存。

在新的代码中，我们在MyString的构造函数中用new分配内存。如果由于内存不足抛出诸如bad_alloc等异常，但我们还没有修改原来示例的状态，因此实例的状态还是有效的，这就保证了异常的安全性。

参见：剑指offer 第二版 第二章

[上一级](README.md)
[上一篇 -> mutable的使用](mutable.md)
[下一篇 -> c++正则表达式](regex.md)
