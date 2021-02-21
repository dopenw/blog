# 罗马数字转换为int

Given a roman numeral, convert it to an integer.

Input is guaranteed to be within the range from 1 to 3999.


```c++
class Solution {
public:
    int romanToInt(string s) {
         unordered_map<char, int> T = {
                                   { 'I' , 1 },
                                   { 'V' , 5 },
                                   { 'X' , 10 },
                                   { 'L' , 50 },
                                   { 'C' , 100 },
                                   { 'D' , 500 },
                                   { 'M' , 1000 } };

        int sum=T[s.back()];

        for (int i=s.length()-2;i>=0;i--)
        {
            if (T[s[i]]<T[s[i+1]])
                sum-=T[s[i]];
            else
                sum+=T[s[i]];
        }
        return sum;

    }
};
```


[source link](https://leetcode.com/problems/roman-to-integer/discuss/)


- [上一级](README.md)
- 上一篇 -> [从排序的listnode列表中删除重复node](removeDuplicatesFromSortedList.md)
- 下一篇 -> [旋转图像](rotate_image.md)
