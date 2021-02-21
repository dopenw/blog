# 组合和


Given a set of candidate numbers (C) (without duplicates) and a target number (T), find all unique combinations in C where the candidate numbers sums to T.

The same repeated number may be chosen from C unlimited number of times.

Note:
All numbers (including target) will be positive integers.
The solution set must not contain duplicate combinations.
For example, given candidate set [2, 3, 6, 7] and target 7,
A solution set is:

```
[
  [7],
  [2, 2, 3]
]
```


algorithm_1_0:
```c++
class Solution {
public:
    vector<vector<int>> combinationSum(vector<int>& candidates, int target) {
        vector <vector<int>> res;
        vector <int> tmp;
        sort(candidates.begin(),candidates.end());

        computer_method(res,0,target,candidates,tmp);
        return res;
    }

//care tmp
    void computer_method(vector <vector<int>> &res,int location,int target,vector <int> candidates,vector <int> tmp)
    {

        if (!target)
        {
            res.push_back(tmp);
            return ;
        }
        for (int i=location;i != candidates.size() && target >= candidates[i] ;++i)
        {
            tmp.push_back(candidates[i]);
            computer_method(res,i,target-candidates[i],candidates,tmp);
            tmp.pop_back();
        }

    }
};
```

algorithm_1_1:
```c++
class Solution {
public:
    vector<vector<int>> combinationSum(vector<int>& candidates, int target) {
        vector <vector<int>> res;
        vector <int> tmp;
        sort(candidates.begin(),candidates.end());

        computer_method(res,0,target,candidates,tmp);
        return res;
    }

//care &tmp
    void computer_method(vector <vector<int>> &res,int location,int target,vector <int> candidates,vector <int> &tmp)
    {

        if (!target)
        {
            res.push_back(tmp);
            return ;
        }
        for (int i=location;i != candidates.size() && target >= candidates[i] ;++i)
        {
            tmp.push_back(candidates[i]);
            computer_method(res,i,target-candidates[i],candidates,tmp);
            tmp.pop_back();
        }

    }
};
```

[source link](https://leetcode.com/problems/combination-sum/discuss/)


- [上一级](README.md)
- 上一篇 -> [4 sum](4sum.md)
- 下一篇 -> [组合和 2](Combination_Sum_II.md)
