# 总共有n步楼梯，每次只能爬1步/2步，总共有多少方案爬上楼梯


You are climbing a stair case. It takes n steps to reach to the top.

Each time you can either climb 1 or 2 steps. In how many distinct ways can you climb to the top?

Note: Given n will be a positive integer.


(Fibonacci Number)  algorithm_0:
```c++
class Solution {
public:
    int climbStairs(int n) {
        if (0==n)
            return 0;
        if (1==n)
            return 1;
        if (2==n)
            return 2;
        int a=1,b=2,result=0;

        for (int i=3;i<=n;i++)
        {
            result=a+b;
            a=b;
            b=result;
        }
        return result;

    }
};
```

6种方法:
[solution link](https://leetcode.com/problems/climbing-stairs/solution/)


[上一级](README.md)
[上一篇 -> atoi](atoi.md)
[下一篇 -> 统计字符串的相同字符个数，并输出（count-char）](count_and_say.md)
