# 给定一个排序的数组，和一个目标值，查找该值在数组的范围

Given an array of integers sorted in ascending order, find the starting and ending position of a given target value.

Your algorithm's runtime complexity must be in the order of O(log n).

If the target is not found in the array, return [-1, -1].

For example,
Given [5, 7, 7, 8, 8, 10] and target value 8,
return [3, 4].


algorithm_1_0:
```
class Solution {
public:
    vector<int> searchRange(vector<int>& nums, int target) {
        vector<int> result;
        int target_left_bound=left_bound(nums,target);
        cout<<target_left_bound<<endl;

        int target1_left_bound=left_bound(nums,target+1);
        cout<<target1_left_bound<<endl;
        if (target_left_bound!=target1_left_bound)
        {
            result.push_back(target_left_bound);
            result.push_back(target1_left_bound-1);
        }
        else
        {
            result.push_back(-1);
            result.push_back(-1);
        }
        return result;

    }

    int left_bound(vector<int>&nums,int num)
    {
        int left=0,right=nums.size()-1;
        //注意是要得到最小边界
        //if num==nums[mid]:
        //  right=mid-1
        while (left<=right)
        {
            int mid=left+(right-left)/2;
            if (num>nums[mid])
                left=mid+1;
            else
                right=mid-1;
        }
        return left;
    }
};
```

algorithm_1:
```c++
vector<int> searchRange(vector<int>& nums, int target) {
    int idx1 = lower_bound(nums, target);
    int idx2 = lower_bound(nums, target+1)-1;
    if (idx1 < nums.size() && nums[idx1] == target)
        return {idx1, idx2};
    else
        return {-1, -1};
}

int lower_bound(vector<int>& nums, int target) {
    int l = 0, r = nums.size()-1;
    while (l <= r) {
        int mid = (r-l)/2+l;
        if (nums[mid] < target)
            l = mid+1;
        else
            r = mid-1;
    }
    return l;
}
```

[source link](https://leetcode.com/problems/search-for-a-range/discuss/)




[上一级](base.md)
[上一篇](same_tree.md)
[下一篇](sqrt.md)
