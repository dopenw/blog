<!-- Put functions into vector and execute -->

[std::function](http://en.cppreference.com/w/cpp/utility/functional/function)

```c++
#include <functional>
#include <iostream>
#include <vector>

class WorkingClass {
public:
  typedef std::function<void(int)> handler_t;
  void AddHandler(handler_t h) { handlerList.push_back(h); }
  void DoStuff() {
    int handlerListLength = handlerList.size();
    for (int i = 0; i < handlerListLength; i++) {
      handlerList[i](666);
    }
  }

  std::vector<handler_t> handlerList;
};

int main() {
  WorkingClass wc;
  wc.AddHandler([&](int num) { std::cout << "A: " << num << std::endl; });
  wc.AddHandler([&](int num) { std::cout << "B: " << num << std::endl; });
  wc.DoStuff();
}
```

Run it:
```sh
A: 666
B: 666
```

Links:
* [Put functions into vector and execute](https://stackoverflow.com/questions/25628031/put-functions-into-vector-and-execute?utm_medium=organic&utm_source=google_rich_qa&utm_campaign=google_rich_qa)
* [C++11 Callbacks with function and lambda](https://gist.github.com/4poc/3155832)
---
- [上一级](README.md)
- 上一篇 -> [C++ Preprocessor](Preprocessor.md)
- 下一篇 -> [STL](STL.md)
