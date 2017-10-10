# 3 Sum Closest

Given an array S of n integers, find three integers in S such that the sum is closest to a given number, target. Return the sum of the three integers. You may assume that each input would have exactly one solution.

```
For example, given array S = {-1 2 1 -4}, and target = 1.

    The sum that is closest to the target is 2. (-1 + 2 + 1 = 2).
```

algorithm_0:
```c++
class Solution {
public:
    int threeSumClosest(vector<int>& nums, int target) {
        int result=nums[0]+nums[1]+nums[2];
        int advance=abs(target-result);
        sort(nums.begin(),nums.end());
        for (int i=0;i<nums.size();i++)
        {
            int l=i+1,r=nums.size()-1;
            while (l<r)
            {
                int minValue=nums[i]+nums[l]+nums[r];
                int tmp_advance=target-minValue;
                if (tmp_advance>0)
                    l++;
                else if (tmp_advance <0)
                    r--;
                 else
                    return target;

                if (advance > abs(tmp_advance))
                {
                    result=minValue;
                    advance=abs(tmp_advance);
                }

            }
        }
        return result;
    }
};
```


## 错误版本(wrong)：
```c++
class Solution {
public:
    int threeSumClosest(vector<int>& nums, int target) {
        int result=nums[0]+nums[1]+nums[2];
        int advance=abs(target-result);
        sort(nums.begin(),nums.end());
        for (int i=0;i<nums.size();i++)
        {
            int l=i+1,r=nums.size()-1;
            cout<<"i:"<<i<<endl;
            while (l<r)
            {
                int minValue=nums[i]+nums[l]+nums[r];
                int tmp_advance=target-minValue;
                if (tmp_advance>0)
                    l++;
                else if (tmp_advance <0)
                    r--;
                 else
                    return target;
//care wrong
                while (l<r && nums[l]==nums[l+1]) l++;
                cout<<"l:"<<l<<endl;

                while (l<r && nums[r]==nums[r-1]) r--;
                cout<<"r:"<<r<<endl;
//care wrong
                if (advance > abs(tmp_advance))
                {
                    result=minValue;
                    advance=abs(tmp_advance);
                }

            }
        }
        return result;
    }
};
```










[上一级](base.md)
[上一篇](3sum.md)
[下一篇](4sum.md)
