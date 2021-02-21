# 生成括号

Given n pairs of parentheses, write a function to generate all combinations of well-formed parentheses.

For example, given n = 3, a solution set is:

```
[
  "((()))",
  "(()())",
  "(())()",
  "()(())",
  "()()()"
]
```

algorithm_1:
```c++
class Solution {
public:
    vector<string> generateParenthesis(int n) {
        vector<string> tmp;
        auto_add(tmp,"",n,0);
        return tmp;
    }

    void auto_add(vector<string> &v,string str,int n,int m)
    {
        if (m==0 && n==0)
        {
            v.push_back(str);
            return;
        }
        if (m>0)
            auto_add(v,str+")",n,m-1);
        if (n>0)
            auto_add(v,str+"(",n-1,m+1);
    }
};

```

[source link](https://leetcode.com/problems/generate-parentheses/discuss/)


- [上一级](README.md)
- 上一篇 -> [两个整数相除](divide_two_integers.md)
- 下一篇 -> [int转换为罗马数字](intToRoman.md)
