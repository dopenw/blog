# Two Sum
Given an array of integers, return indices of the two numbers such that they add up to a specific target.

You may assume that each input would have exactly one solution, and you may not use the same element twice

example:
```
Given nums = [2, 7, 11, 15], target = 9,

Because nums[0] + nums[1] = 2 + 7 = 9,
return [0, 1].
```

algorithm_0:
```c++
class Solution {
public:
    vector<int> twoSum(vector<int>& nums, int target) {
        map<int,int> mm;
        vector<int> vec;
        for (int i=0;i<nums.size();i++)
        {
            mm.insert(pair<int,int>(nums[i],i));
           int tmp=target-nums[i];
            if(mm.find(tmp)!=mm.end())
            {
                 if (mm.find(tmp)->second!=i)
                {
                    vec.push_back(i);
                    vec.push_back(mm.find(tmp)->second);
                    break;
                }
            }

        }
        sort(vec.begin(),vec.end());
        return vec;
    }
};
```


algorithm_0_1:
```c++
class Solution {
public:
    vector<int> twoSum(vector<int>& nums, int target) {
        map<int,int> mm;
        vector<int> vec;
        for (int i=0;i<nums.size();i++)
        {           
            int tmp=target-nums[i];
            if(mm.find(tmp)!=mm.end())
            {
                    vec.push_back(mm.find(tmp)->second);
                    vec.push_back(i);
                    break;
            }
            mm.insert(pair<int,int>(nums[i],i));
        }
        return vec;
    }
};
```


[solution link](https://leetcode.com/problems/two-sum/solution/)









[上一级](base.md)
[上一篇](sqrt.md)
