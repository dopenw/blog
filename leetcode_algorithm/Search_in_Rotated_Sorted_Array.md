# 搜索旋转排序数组

Suppose an array sorted in ascending order is rotated at some pivot unknown to you beforehand.

(i.e., 0 1 2 4 5 6 7 might become 4 5 6 7 0 1 2).

You are given a target value to search. If found in the array return its index, otherwise return -1.

You may assume no duplicate exists in the array.

algorithm_1:
```c++
public class Solution {
public int search(int[] A, int target) {
    int lo = 0;
    int hi = A.length - 1;
    while (lo < hi) {
        int mid = (lo + hi) / 2;
        if (A[mid] == target) return mid;

        if (A[lo] <= A[mid]) {
            if (target >= A[lo] && target < A[mid]) {
                hi = mid - 1;
            } else {
                lo = mid + 1;
            }
        } else {
            if (target > A[mid] && target <= A[hi]) {
                lo = mid + 1;
            } else {
                hi = mid - 1;
            }
        }
    }
    return A[lo] == target ? lo : -1;
}
```

algorithm_2:
```c++
class Solution {
public:
    int search(vector<int>& nums, int target) {

        if (nums.empty())
            return -1;

        int chead=nums[0],head=0,tail=nums.size()-1;

//care head < tail

        while (head < tail)
        {
            int mid=head+(tail-head)/2;
            int cmid=nums[mid];

// compare chead,cmid,target

            if ((chead > cmid) ^ (chead > target) ^ (cmid < target))
                head=mid+1;
            else
                tail=mid;

        }

        int result=(nums[head]==target) ? head:-1;
        return result;
    }
};
```

[source link](https://leetcode.com/problems/search-in-rotated-sorted-array/discuss/)


---
- [上一级](README.md)
- 上一篇 -> [旋转列表](Rotate_List.md)
- 下一篇 -> [螺旋矩阵](Spiral_Matrix.md)
