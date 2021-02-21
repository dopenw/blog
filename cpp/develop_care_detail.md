# 开发注意事项
1. 在作条件判断时应充分考虑出现的情况，eg：
若未充分考虑为空的情况，将为出现下面的不确定的结果
```shell
egrep $null $filename  
# => get all filename's context
egrep $partern $filename
# => some filename's context
```

2. 对无符号数要注意减法溢出

3. 使用c++开发时调用函数库时要注意
场景：使用#include "vector"，因为该路径下有之前测试生成的文件名为vector的可执行文件，编译时就报大量的错误。

4. atom文本编辑器的for关键字快捷辅助输出为
```c++
for (size_t column = length -1; column >= 0; column--) {}
```
注意这样的代码可能会造成死循环

- [上一级](README.md)
- 上一篇 -> [设计模式](design_patterns.md)
- 下一篇 -> [do_while_false的功用](do_while_false.md)
