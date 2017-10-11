# 组卦

Given an array of strings, group anagrams together.

For example, given: ["eat", "tea", "tan", "ate", "nat", "bat"],
Return:

```
[
  ["ate", "eat","tea"],
  ["nat","tan"],
  ["bat"]
]
```
Note: All inputs will be in lower-case.


algorithm_0:
```c++
class Solution {
public:
    vector<vector<string>> groupAnagrams(vector<string>& strs) {

        unordered_map<string,vector<string>> map_tmp;

        for (int i=0;i<strs.size();++i)
        {
            string tmp=strs[i];
            sort(tmp.begin(),tmp.end());
            auto iter=map_tmp.find(tmp);
            if (iter!=map_tmp.end())
            {
                iter->second.push_back(strs[i]);
            }
            else
                map_tmp.insert(make_pair(tmp,vector<string> {strs[i]}));
        }

        vector<vector<string>> res;
        for (auto i:map_tmp)
        {
            sort(i.second.begin(),i.second.end());
            res.push_back(i.second);
        }
        return res;
    }
};
```


[source link](https://leetcode.com/problems/group-anagrams/discuss/)


[上一级](base.md)
[上一篇](Combination_Sum_II.md)
[下一篇](Multiply_Strings.md)
