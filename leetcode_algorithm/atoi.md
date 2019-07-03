# atoi

实现atoi将字符串转换为整数:

[Description](https://leetcode.com/problems/string-to-integer-atoi/description/)

```c++
#include <climits>
#include <iostream>

using namespace std;

int atoi(const char *str) {
  int sign = 1, base = 0, i = 0;
  while (str[i] == ' ')
    i++;
  if (str[i] == '-' || str[i] == '+') {
    sign = 1 - 2 * (str[i++] == '-');
  }

  while (str[i] >= '0' && str[i] <= '9') {
    if (base > INT_MAX / 10 || (base == INT_MAX / 10 && str[i] - '0' > 7)) {
      if (sign == 1)
        return INT_MAX;
      else
        return INT_MIN;
    }
    base = 10 * base + (str[i++] - '0');
  }
  return base * sign;
}

int main(int argc, char const *argv[]) {
  std::cout << "please input string: " << '\n';
  string tmp;
  cin >> tmp;
  while (tmp != "break") {
    std::cout << atoi(tmp.c_str()) << '\n';
    cin >> tmp;
  }
  return 0;
}
```

[code link](https://leetcode.com/problems/string-to-integer-atoi/discuss/4654)
[上一级](README.md)
[上一篇](add_two_numbers.md)
[下一篇](climbing_stairs.md)
