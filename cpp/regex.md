# c++正则表达式
1. 正则表达式Regex(regular expression)是一种强大的描述字符序列的工具。在许多语言中都存在着正则表达式，C++11中也将正则表达式纳入了新标准的一部分，不仅如此，它还支持了6种不同的正则表达式的语法，分别是：ECMASCRIPT、basic、extended、awk、grep和egrep。
2. 使用regex后编译器提示
```cpp
terminate called after throwing an instance of 'std::regex_error'
  what():  regex_error
Aborted (core dumped)
```
原以为是语法错误，结果通过
[我是一只C++小小鸟](http://www.cnblogs.com/ittinybird/p/4853532.html)的博客找到了问题：

**GCC4.8.3编译器有regex头文件，语法完全支持，但是库还没跟上。编译时没有问题，但是一运行就会直接抛出异常**

3. regex 函数
    1. bool regex_match(str,regex) 检验regex之完全匹配
    2. bool regex_match(str,matchRet,regex) 检验并返回regex的完全匹配
    3.bool regex_search(str,regex) 查找regex的匹配
    4. bool regex_search(str,matchRet,regex) 查找并返回regex的匹配
    5. strRes regex_replace(str.regex,repl) 根据regex替换所有匹配







































[上一级](base.md)
[上一篇](do_while_false.md)
