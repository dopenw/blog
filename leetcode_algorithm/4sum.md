# 4 sum

Given an array S of n integers, are there elements a, b, c, and d in S such that a + b + c + d = target? Find all unique quadruplets in the array which gives the sum of target.

Note: The solution set must not contain duplicate quadruplets.

```
For example, given array S = [1, 0, -1, 0, -2, 2], and target = 0.

A solution set is:
[
  [-1,  0, 0, 1],
  [-2, -1, 1, 2],
  [-2,  0, 0, 2]
]
```

algorithm_0(Time Limit Exceeded):
```c++
class Solution {
public:
    vector<vector<int>> fourSum(vector<int>& nums, int target) {
        vector<vector<int>> result;
        if (nums.empty())
            return result;
        sort(nums.begin(),nums.end());

        for (int i=0;i<nums.size();i++)
        {
            int sum1=target-nums[i];
            for (int j=i+1;j<nums.size();j++)
            {
                sum1=sum1-nums[j];
                int l=j+1,r=nums.size()-1;
                while (l<r)
                {
                    int sum2=nums[l]+nums[r];

                    if (sum2<sum1)
                        l++;
                    else if (sum2>sum1)
                        r--;
                    else
                    {
//care
                        vector<int> tmp={nums[i],nums[j],nums[l],nums[r]};
                        result.push_back(tmp);
                        while (l<r && nums[l]==nums[l+1]) l++;
                        while (l<r && nums[r]==nums[r-1]) l--;
//care
                    }
                }
               while (j+1<nums.size() && nums[j]==nums[j+1]) ++j;

            }
            while (i+1<nums.size() && nums[i]==nums[i+1]) ++i;
            }
        return result;
    }
};
```

algorithm_0_1:
```c++
class Solution {
public:
    vector<vector<int>> fourSum(vector<int>& nums, int target) {
        vector<vector<int>> result;
        if (nums.empty())
            return result;
        sort(nums.begin(),nums.end());

        for (int i=0;i<nums.size();i++)
        {
            for (int j=i+1;j<nums.size();j++)
            {
               int sum1=target-nums[i]-nums[j];
                int l=j+1,r=nums.size()-1;
                while (l<r)
                {
                    int sum2=nums[l]+nums[r];

                    if (sum2<sum1)
                        l++;
                    else if (sum2>sum1)
                        r--;
                    else
                    {
//care
                        vector<int> quadruplet(4, 0);
                        quadruplet[0] = nums[i];
                        quadruplet[1] = nums[j];
                        quadruplet[2] = nums[l];
                        quadruplet[3] = nums[r];
                        result.push_back(quadruplet);
                        while (l<r && nums[l]==quadruplet[2]) l++;
                        while (l<r && nums[r]==quadruplet[3]) r--;
//care
                    }
                }
               while (j+1<nums.size() && nums[j]==nums[j+1]) ++j;

            }
            while (i+1<nums.size() && nums[i]==nums[i+1]) ++i;
            }
        return result;
    }
};
```








[上一级](base.md)
[上一篇](3sum_closest.md)
[下一篇](Combination_Sum.md)
