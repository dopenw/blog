<!-- 字符串乘字符串 -->

Given two non-negative integers num1 and num2 represented as strings, return the product of num1 and num2.

Note:

1. The length of both num1 and num2 is < 110\.
2. Both num1 and num2 contains only digits 0-9\.
3. Both num1 and num2 does not contain any leading zero.
4. You must not use any built-in BigInteger library or convert the inputs to integer directly.


algorithm_1_0:

```c++
class Solution {
public:
    string multiply(string num1, string num2) {
        string sum(num1.size()+num2.size(),'0');

        for (int num1_lo=num1.size()-1;num1_lo>=0;num1_lo--)

        {
            int carry=0;
            for (int num2_lo=num2.size()-1;num2_lo>=0;num2_lo--)
            {
                int tmp=(sum[num1_lo+num2_lo+1]-'0')
                + (num1[num1_lo]-'0')
                * (num2[num2_lo]-'0')
                + carry;

                sum[num1_lo+num2_lo+1]=tmp % 10+'0';
                carry=tmp/10;
            }
            sum[num1_lo]+=carry;
        }

//string::find_first_not_of()

/*Searches the string for the first character that does not match any of the characters specified in its arguments.
When pos is specified, the search only includes characters at or after position pos, ignoring any possible occurrences before that character.*/

//care static const size_t npos = -1;

        size_t start_pos=sum.find_first_not_of('0');


        if (start_pos!=string::npos)
        {
            return sum.substr(start_pos);
        }
        return "0";
    }
};
```

[source link](https://leetcode.com/problems/multiply-strings/discuss/)


---
- [上一级](README.md)
- 上一篇 -> [合并间隔](Merge_Intervals.md)
- 下一篇 -> [置换序列](Permutation_Sequence.md)
