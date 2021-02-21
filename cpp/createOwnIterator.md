# 用户自定义迭代器


简单示例：
```c++
#include <algorithm>
#include <iostream>
#include <vector>

using namespace std;

class Test {
private:
  std::vector<int> m_vec{1, 2, 3, 4, 5, 6, 7, 8, 9};

public:
  typedef std::vector<int>::iterator iterator;
  typedef std::vector<int>::const_iterator const_iterator;

  iterator begin() { return m_vec.begin(); }

  const_iterator begin() const { return m_vec.begin(); }

  iterator end() { return m_vec.end(); }

  const_iterator end() const { return m_vec.end(); }
};

int main(int argc, char const *argv[]) {
  Test test;
  for (auto i : test) {
    std::cout << i << '\n';
  }
  return 0;
}
```

Run it:

```sh
1
2
3
4
5
6
7
8
9
```

继承 iterator 类，并自定义新的迭代器：
```c++
// std::iterator example
#include <iostream> // std::cout
#include <iterator> // std::iterator, std::input_iterator_tag

class MyIterator : public std::iterator<std::input_iterator_tag, int> {
  int * p;

public:
  MyIterator(int * x) : p(x) {}
  MyIterator(const MyIterator &mit) : p(mit.p) {}
  MyIterator &operator++() {
    ++p;
    return * this;
  }
  MyIterator operator++(int) {
    MyIterator tmp(* this);
    operator++();
    return tmp;
  }
  bool operator==(const MyIterator &rhs) const { return p == rhs.p; }
  bool operator!=(const MyIterator &rhs) const { return p != rhs.p; }
  int &operator *() { return * p; }
};

int main() {
  int numbers[] = {10, 20, 30, 40, 50};
  MyIterator from(numbers);
  MyIterator until(numbers + 5);
  for (MyIterator it = from; it != until; it++)
    std::cout << * it << ' ';
  std::cout << '\n';

  return 0;
}
```

Run it:
```sh
10 20 30 40 50
```

[user defined iterator](http://www.cplusplus.com/reference/iterator/iterator/)

Link:
* [Creating my own Iterators
](https://stackoverflow.com/questions/148540/creating-my-own-iterators)
* [ACCU:Iterators and Memberspaces](https://accu.org/index.php/journals/1527)
* [Writing your own STL Container](https://stackoverflow.com/questions/7758580/writing-your-own-stl-container/7759622#7759622)
* [c++11 foreach syntax and custom iterator](https://stackoverflow.com/questions/7562356/c11-foreach-syntax-and-custom-iterator)
* [Using boost::iterator](https://stackoverflow.com/questions/2822989/using-boostiterator)
* [cplusplus.com/reference/iterator/](http://www.cplusplus.com/reference/iterator/)

- [上一级](README.md)
- 上一篇 -> [将string转换为char指针](conv_string_to_char_pointer.md)
- 下一篇 -> [设计模式](design_patterns.md)
