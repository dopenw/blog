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










































[上一级](base.md)
[上一篇](conv_string_to_char_pointer.md)
[下一篇](do_while_false.md)
