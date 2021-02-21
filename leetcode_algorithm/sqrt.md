# 求平方根

Implement int sqrt(int x).

Compute and return the square root of x.


algorithm_1:

```c++
//mid^2<=x<(mid+1)^2
//return mid
```

```c++
public int sqrt(int x) {
    if (x == 0)
        return 0;
    int left = 1, right = Integer.MAX_VALUE;
    while (true) {
        int mid = left + (right - left)/2;
        if (mid > x/mid) {
            right = mid - 1;
        } else {
            if (mid + 1 > x/(mid + 1))
                return mid;
            left = mid + 1;
        }
    }
}
```

algorithm_2:
```c++
class Solution {
public:
    int sqrt(int x) {
        if (0 == x) return 0;
        int left = 1, right = x, ans;
        while (left <= right) {
            int mid = left + (right - left) / 2;
            if (mid <= x / mid) {
                left = mid + 1;
                ans = mid;
            } else {
                right = mid - 1;
            }
        }
        return ans;
    }
};
```


algorithm_3:
```c++
long r = x;
while (r*r > x)
    r = (r + x/r) / 2;
return r;
```

[source link](https://leetcode.com/problems/sqrtx/discuss/)


- [上一级](README.md)
- 上一篇 -> [给定一个排序的数组，和一个目标值，查找该值在数组的范围](search_for_a_range.md)
- 下一篇 -> [Two Sum](two_sum.md)
