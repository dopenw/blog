# Maximum Subarray(最大子串和)

Find the contiguous subarray within an array (containing at least one number) which has the largest sum.

For example, given the array

```
[-2,1,-3,4,-1,2,1,-5,4]
```

the contiguous subarray
```
[4,-1,2,1]
```
has the largest
```
sum = 6
```


algorithm 1（Time Limit Exceeded） 平方算法:

```c++
class Solution {
public:
    int maxSubArray(vector<int>& nums) {
        int result=nums[0];
     for (int i=0;i<nums.size();i++)
     {
         int sum=0;
        for (int j=i;j<nums.size();j++)
         {
             sum+=nums[j];
             result=max(result,sum);

         }
     }
         return result;
    }
};
```

algorithm 2 ( 10ms )  扫描算法:
```c
class Solution {
public:
    int maxSubArray(vector<int>& nums) {
        int sum=0;
        int result=nums[0];
        for (int i=0;i<nums.size();i++)
        {
            sum+=nums[i];
            result=max(sum,result);
            sum=max(sum,0);
        }
        return result;
    }
};
```

[source link](https://leetcode.com/problems/maximum-subarray/description/)









[上一级](base.md)
[上一篇](intToRoman.md)
[下一篇](merge_sorted_array.md)
