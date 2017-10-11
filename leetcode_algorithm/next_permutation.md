# 下一个排列
Next Permutation

Implement next permutation, which rearranges numbers into the lexicographically next greater permutation of numbers.

If such arrangement is not possible, it must rearrange it as the lowest possible order (ie, sorted in ascending order).

The replacement must be in-place, do not allocate extra memory.

Here are some examples. Inputs are in the left-hand column and its corresponding outputs are in the right-hand column.
```
1,2,3 → 1,3,2
3,2,1 → 1,2,3
1,1,5 → 1,5,1
```

algorithm_1:
```c++
class Solution {
public:
    void nextPermutation(vector<int>& nums) {
        if (nums.size()<2)
            return ;
        int head=nums.size()-2;
        while ( head>=0 && nums[head+1]<=nums[head])
        {
            head--;
        }
        // cout<<head<<endl;

        if (head>=0)
        {
        int tail=nums.size()-1;
        while (tail>=0 && nums[tail]<=nums[head])
        {
            tail--;
        }

        // cout<<tail<<endl;

        swap(nums[head],nums[tail]);

        }
        reverse(nums.begin()+head+1,nums.end());


    }


};
```

[Solution link](https://leetcode.com/problems/next-permutation/solution/)








[上一级](base.md)
[上一篇](merge_sorted_array.md)
[下一篇](plus_one.md)
