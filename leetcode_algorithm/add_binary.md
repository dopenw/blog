# 相加2进制字符串

Given two binary strings, return their sum (also a binary string).

For example,
a = "11"
b = "1"
Return "100"


algorithm 0 :

```c++
class Solution {
public:
    string addBinary(string a, string b) {
        int a_loc=a.size();
        int b_loc=b.size();
        int a_item;
        int b_item;
        int flag=0;
        string result;
        while (a_loc>0 || b_loc>0)
        {
            a_item=((a_loc--)>0) ? a[a_loc]-'0':0;
            b_item=((b_loc--)>0) ? b[b_loc]-'0':0;
            int tmp=a_item+b_item+flag;
            if (tmp>=2)
                {
                flag=1;
                char tt=(tmp%2 + '0');
                result.insert(0,1,tt);
            }
            else
            {
                flag=0;
                char tt=(tmp + '0');
                result.insert(0,1,tt);
            }
        }
        if (flag==1)
        {
            result.insert(0,"1");
        }
        return result;

    }
};
```

algorithm 0_1:

```c++
class Solution {
public:
    string addBinary(string a, string b) {
        int a_loc=a.size();
        int b_loc=b.size();
        int a_item,b_item,flag=0;
        string result;
        while (a_loc>0 || b_loc>0 || flag==1)
        {
            a_item=((a_loc--)>0) ? a[a_loc]-'0':0;
            b_item=((b_loc--)>0) ? b[b_loc]-'0':0;
            int tmp=a_item+b_item+flag;
            flag=(tmp>=2) ? 1: 0;
            result.insert(0,1,static_cast<char>(tmp%2 + '0'));
        }
        return result;

    }
};
```

algorithm 1:
```c++
string addBinary(string a, string b)
{
    string result = "";
    int apos = a.size() - 1;
    int bpos = b.size() - 1;
    int adigit, bdigit, carry = 0;

    while (apos >= 0 || bpos >= 0 || carry == 1)
    {
        adigit = bdigit = 0;

        if (apos >= 0) adigit = a[apos--] == '1';
        if (bpos >= 0) bdigit = b[bpos--] == '1';

        // Another way: the digit is 1 if adigit + bdigit + carry == 1 or == 3, but I noticed that
        // XOR is more concise:
        result = static_cast<char>(adigit ^ bdigit ^ carry + '0') + result;
        carry = adigit + bdigit + carry >= 2;
    }

    return result;
}
```


algorithm 2:
```c++
class Solution
{
public:
    string addBinary(string a, string b)
    {
        string s = "";

        int c = 0, i = a.size() - 1, j = b.size() - 1;
        while(i >= 0 || j >= 0 || c == 1)
        {
            c += i >= 0 ? a[i --] - '0' : 0;
            c += j >= 0 ? b[j --] - '0' : 0;
            s = char(c % 2 + '0') + s;
            c /= 2;
        }

        return s;
    }
};
```

[source link](https://leetcode.com/problems/add-binary/discuss/)














[上一级](base.md)
[上一篇](ZigZag_Conversion.md)
[下一篇](add_two_numbers.md)
