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


[上一级](base.md)
[上一篇](design_patterns.md)
[下一篇](do_while_false.md)
