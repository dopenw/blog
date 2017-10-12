# 求x的n次方

Implement pow(x, n).

## algorithm_0:
* (Time Limit Exceeded)
```c++
class Solution {
public:
    double myPow(double x, int n) {
        double res=1.0;
        if (n<0)
        {
           x=1/x;
           n=-n;
        }
        for (int i=0;i<n;++i)
        {
            res=res*x;
        }
        return res;
    }
};
```
## inf

* 即 宏：float INFINITY
表示正无穷大的表达。它等于数学运算产生的值，如1.0 / 0.0。 -INFINITY表示负无穷大。 您可以通过将浮点值与该宏进行比较来测试浮点值是否为无穷大。但是，这不是推荐的;你应该使用isfinite宏。请参阅浮点类。 这个宏被引入了ISO C99标准。

## algorithm_1_0:
```c++
class Solution {
    public:
        double myPow(double x, int n) {

        if(n == 0)
            return 1;

        if(n<0){
            n = -n;
            x = 1/x;
        }

        double res= (n%2 == 0) ? myPow(x*x, n/2) : x*myPow(x*x, n/2);
        if (isinf(res))
            return 0;

        return res;
    }
};
```


## algorithm_2_0:
```c++
class Solution {
    public:
 double myPow(double x, int n) {
    if(n==0) return 1;

    /* n=-2147483648
    n=-n;
    n还是等于-2147483648
    */

     if (n==INT_MIN && x>1) return 0;

    if(n<0) {
        n = -n;
        x = 1/x;
    }
    double ans = 1;
    while(n>0){
        if(n&1) ans *= x;
        x *= x;
        n >>= 1;
    }
    return ans;
}
};
```

[source link](https://leetcode.com/problems/powx-n/discuss/)
[上一级](base.md)
[上一篇](plus_one.md)
[下一篇](removeDuplicatesFromSortedList.md)
