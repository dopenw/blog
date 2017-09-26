#将给定非负整数的vector<int> digits加一，返回加一后的最高位

Given a non-negative integer represented as a non-empty array of digits, plus one to the integer.

You may assume the integer do not contain any leading zero, except the number 0 itself.

The digits are stored such that the most significant digit is at the head of the list.


algorithm 1:

```c++
class Solution {
public:
    vector<int> plusOne(vector<int>& digits) {
        bool carry=true;

        for (int i=digits.size()-1;i>=0 && carry;i--)
        {
            carry=(++digits[i]%=10)==0;
        }

        if (carry)
        {
            digits.insert(digits.begin(),1);
        }
        return digits;
    }
};
```

[source link](https://leetcode.com/problems/plus-one/discuss/)













[上一级](base.md)
[上一篇](merge_sorted_array.md)
[下一篇](removeDuplicatesFromSortedList.md)
