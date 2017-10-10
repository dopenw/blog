# 排列

Given a collection of distinct numbers, return all possible permutations.

For example,
[1,2,3] have the following permutations:

```
[
  [1,2,3],
  [1,3,2],
  [2,1,3],
  [2,3,1],
  [3,1,2],
  [3,2,1]
]
```


algorithm_1_0:
```c++
class Solution {
public:
    vector<vector<int>> permute(vector<int>& nums) {
        vector<vector<int>> res;
        permute_auto(nums,0,res);
        return res;
    }


private:
  //care & nums, & res
    void permute_auto(vector<int> &nums,int begin,vector<vector<int>> &res)
    {
        if (begin>=nums.size())
        {
            res.push_back(nums);
            return;
        }
        for (int i=begin;i<nums.size();i++)
        {
            swap(nums[begin],nums[i]);
            permute_auto(nums,begin+1,res);
            //reset
            swap(nums[begin],nums[i]);
        }
    }
};
```

algorithm_1_1:
```c++
class Solution {
public:
    vector<vector<int>> permute(vector<int>& nums) {
        vector<vector<int>> res;
        permute_auto(nums,0,res);
        return res;
    }


private:
    void permute_auto(vector<int> nums,int begin,vector<vector<int>> &res)
    {
        if (begin==nums.size()-1)
        {
            res.push_back(nums);
            return;
        }
        for (int i=begin;i<nums.size();i++)
        {
            swap(nums[begin],nums[i]);
            permute_auto(nums,begin+1,res);
        }
    }
};
```

[source link](https://leetcode.com/problems/permutations/discuss/)



[上一级](base.md)
[上一篇](Multiply_Strings.md)
[下一篇](Permutations_II.md)
