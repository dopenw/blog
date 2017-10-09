# 组合和 2

Given a collection of candidate numbers (C) and a target number (T), find all unique combinations in C where the candidate numbers sums to T.

Each number in C may only be used once in the combination.

Note:
* All numbers (including target) will be positive integers.
* The solution set must not contain duplicate combinations.

For example, given candidate set [10, 1, 2, 7, 6, 1, 5] and target 8,
A solution set is:
```
[
  [1, 7],
  [1, 2, 5],
  [2, 6],
  [1, 1, 6]
]
```

algorithm_1_0:
```c++
class Solution {
public:
    vector<vector<int>> combinationSum2(vector<int>& candidates, int target) {
        vector<vector<int>> result;
        if (candidates.empty())
            return result;

        vector<int> tmp;
        sort(candidates.begin(),candidates.end());

        combination_recursive(result,tmp,candidates,target,0);
        return result;
    }

    void combination_recursive(vector<vector<int>> &result,vector<int> tmp,vector<int> candidates,int target,int begin)
    {
        if (target==0)
        {
            result.push_back(tmp);
            return;
        }
        cout<<begin<<endl;
        for (int i=begin;i<candidates.size() && target-candidates[i]>=0 ;i++)
        {
//注意内存越界
            if (i && candidates[i]==candidates[i-1] && i> begin ) continue;
            tmp.push_back(candidates[i]);
            combination_recursive(result,tmp,candidates,target-candidates[i],i+1);
            tmp.pop_back();
        }
    }
};
```

algorithm_1_1:
```c++
class Solution {
public:
    vector<vector<int>> combinationSum2(vector<int>& candidates, int target) {
        vector<vector<int>> result;
        if (candidates.empty())
            return result;

        vector<int> tmp;
        sort(candidates.begin(),candidates.end());

        combination_recursive(result,tmp,candidates,target,0);
        return result;
    }

    void combination_recursive(vector<vector<int>> &result,vector<int> tmp,vector<int> candidates,int target,int begin)
    {
        if (target==0)
        {
            result.push_back(tmp);
            return;
        }
        for (int i=begin;i<candidates.size() && target-candidates[i]>=0 ;i++)
        {
      // 去除 i>0 的判断
            if ( i> begin && candidates[i]==candidates[i-1]  ) continue;
            tmp.push_back(candidates[i]);
            combination_recursive(result,tmp,candidates,target-candidates[i],i+1);
            tmp.pop_back();
        }
    }
};
```

[source link](https://leetcode.com/problems/combination-sum-ii/discuss/)


[上一级](base.md)
[上一篇](Combination_Sum.md)
[下一篇](Multiply_Strings.md)
