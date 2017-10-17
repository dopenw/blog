# 排列 2

Given a collection of numbers that might contain duplicates, return all possible unique permutations.

For example,
[1,1,2] have the following unique permutations:

```
[
  [1,1,2],
  [1,2,1],
  [2,1,1]
]
```

algorithm_1_0:
```c++
class Solution {
public:
    vector<vector<int>> permuteUnique(vector<int>& nums) {
        vector<vector<int>> res;

        sort(nums.begin(),nums.end());

        per_rec(nums,res,0);

        return res;
    }


    private:

    void per_rec(vector<int> nums,vector<vector<int>> &res,int begin)
    {


        if (begin>=nums.size())
        {
            res.push_back(nums);
            return;
        }


        for (int i=begin;i<nums.size();i++)
        {
         if (i!=begin && nums[i]==nums[begin])
             continue;
         swap(nums[begin],nums[i]);
         per_rec(nums,res,begin+1);
        }
    }
};
```

[source link](https://leetcode.com/problems/permutations-ii/discuss/)


[上一级](base.md)
[上一篇](Permutations.md)
[下一篇](Rotate_List.md)
