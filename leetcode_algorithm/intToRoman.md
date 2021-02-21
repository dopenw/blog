# int转换为罗马数字

Given an integer, convert it to a roman numeral.

Input is guaranteed to be within the range from 1 to 3999.



```c++
class Solution {
public:
    string intToRoman(int num) {
        string M[4]={"","M","MM","MMM"};
        string C[10]={"","C","CC","CCC","CD","D","DC","DCC","DCCC","CM"};
        string X[10]={"","X","XX","XXX","XL","L","LX","LXX","LXXX","XC"};
        string I[10]={"","I","II","III","IV","V","VI","VII","VIII","IX"};
        return M[num/1000]+C[num%1000/100]+X[num%100/10]+I[num%10];
    }
};
```


[source link](https://leetcode.com/problems/integer-to-roman/discuss/)


[上一级](README.md)
[上一篇 -> 生成括号](generate_parentheses.md)
[下一篇 -> Maximum Subarray(最大子串和)](maximum_subarray.md)
