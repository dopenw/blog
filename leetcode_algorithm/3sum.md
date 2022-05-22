<!-- 3Sum -->

Given an array S of n integers, are there elements a, b, c in S such that a + b + c = 0? Find all unique triplets in the array which gives the sum of zero.

Note: The solution set must not contain duplicate triplets.

```
For example, given array S = [-1, 0, 1, 2, -1, -4],

A solution set is:
[
  [-1, 0, 1],
  [-1, -1, 2]
]

```


algorithm_1:
```c++
class Solution {
public:
    vector<vector<int>> threeSum(vector<int>& nums) {
        vector<vector<int>> result;
        std::sort(nums.begin(),nums.end());
        for (int i=0;i<nums.size();i++)
        {
            int target=-nums[i];
            int front=i+1;
            int tail=nums.size()-1;

            while(front<tail)
            {
                int sum=nums[front]+nums[tail];

                if (sum<target)
                    front++;
                else if (sum>target)
                    tail--;
                else
                {
                    int tmp1=nums[front];
                    int tmp2=nums[tail];
                    vector<int> tmp {nums[i],nums[front],nums[tail]};
                    result.push_back(tmp);

                    while(front<tail && tmp1==nums[front]) front++;

                    while (front<tail && tmp2==nums[tail]) tail--;

                }
            }
            while (i+1 <nums.size() && nums[i]==nums[i+1])
                i++;
        }
        return result;
    }
};
```

[source link](https://leetcode.com/problems/3sum/discuss/)


---
- [上一级](README.md)
- 下一篇 -> [3 Sum Closest](3sum_closest.md)
