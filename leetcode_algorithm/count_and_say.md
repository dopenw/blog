# 统计字符串的相同字符个数，并输出（count-char）

The count-and-say sequence is the sequence of integers with the first five terms as following:

```
1.     1
2.     11
3.     21
4.     1211
5.     111221

```

1 is read off as "one 1" or 11.
11 is read off as "two 1s" or 21.
21 is read off as "one 2, then one 1" or 1211.
Given an integer n, generate the nth term of the count-and-say sequence.

Note: Each term of the sequence of integers will be represented as a string.

example 1:
```
Input: 1
Output: "1"
```

example 2:

```
Input: 4
Output: "1211"
```

algorithm_0:

```c++
class Solution {
public:
    string countAndSay(int n) {
        string vec_str="1";
        while (n>=2)
        {
            string tmp;
            char tt=vec_str[0];
            int count=0;
            for (int i=0;i<vec_str.size();i++)
            {
             if(vec_str[i]==tt)
                 count++;
                else
                {
                    tmp.push_back(static_cast<char>(count+'0'));
                    tmp.push_back(tt);
                    tt=vec_str[i];
                    count=1;
                }
             if (i==vec_str.size()-1)
             {
            	 tmp.push_back(static_cast<char>(count+'0'));
            	                     tmp.push_back(tt);
             }
            }
            vec_str=tmp;
            // cout<<tmp<<endl;
            n--;
        }
        return vec_str;
    }
};
```


algorithm_1:
```c++
string countAndSay(int n) {
    if (n == 0) return "";
    string res = "1";
    while (--n) {
        string cur = "";
        for (int i = 0; i < res.size(); i++) {
            int count = 1;
             while ((i + 1 < res.size()) && (res[i] == res[i + 1])){
                count++;    
                i++;
            }
            cur += to_string(count) + res[i];
        }
        res = cur;
    }
    return res;
}
```

[source link](https://leetcode.com/problems/count-and-say/discuss/)








[上一级](base.md)
[上一篇](climbing_stairs.md)
[下一篇](intToRoman.md)
