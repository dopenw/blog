# 两个整数相除
 Divide two integers without using multiplication, division and mod operator.

If it is overflow, return MAX_INT.

algorithm_0(Time Limit Exceeded):
```c++
class Solution {
public:
    int divide(int dividend, int divisor) {
        if (!divisor || (dividend == INT_MIN && divisor == -1))
            return INT_MAX;
        if (dividend==0)
            return 0;
        int flag=((dividend < 0) ^ (divisor < 0)) ? -1:1;
        int l=0,r=abs(dividend);
        int ans=0;
        while (l<=r)
        {
            int mid=l+(r-l)/2;
            int sum=0;
            int count=abs(divisor);
            while (count>0)
            {
                sum=sum+mid;
                count--;
            }
            // cout<<"sum"<<sum<<endl;
            // cout<<mid<<endl;
            if (sum<=abs(dividend))
            {
                l=mid+1;
                ans=mid;
                // cout<<"ans"<<ans<<endl;
            }
            else
                r=mid-1;
        }
        ans=ans*flag;
        return ans;
    }
};

```

In this problem, we are asked to divide two integers. However, we are not allowed to use division, multiplication and mod operations. So, what else can we use? Yeah, bit manipulations.

Let's do an example and see how bit manipulations work.

Suppose we want to divide 15 by 3, so 15 is dividend and 3 is divisor. Well, division simply requires us to find how many times we can subtract the divisor from the the dividend without making the dividend negative.

Let's get started. We subtract 3 from 15 and we get 12, which is positive. Let's try to subtract more. Well, we shift 3 to the left by 1 bit and we get 6. Subtracting 6 from 15 still gives a positive result. Well, we shift again and get 12. We subtract 12 from 15 and it is still positive. We shift again, obtaining 24 and we know we can at most subtract 12. Well, since 12 is obtained by shifting 3 to left twice, we know it is 4 times of 3. How do we obtain this 4? Well, we start from 1 and shift it to left twice at the same time. We add 4 to an answer (initialized to be 0). In fact, the above process is like 15 = 3 * 4 + 3. We now get part of the quotient (4), with a remainder 3.

Then we repeat the above process again. We subtract divisor = 3 from the remaining dividend = 3 and obtain 0. We know we are done. No shift happens, so we simply add 1 << 0 to the answer.

Now we have the full algorithm to perform division.

According to the problem statement, we need to handle some exceptions, such as overflow.

Well, two cases may cause overflow:

divisor = 0;
dividend = INT_MIN and divisor = -1 (because abs(INT_MIN) = INT_MAX + 1).
Of course, we also need to take the sign into considerations, which is relatively easy.

Putting all these together, we have the following code.

algorithm_1:
```c++
class Solution {
public:
    int divide(int dividend, int divisor) {
        if (!divisor || (dividend == INT_MIN && divisor == -1))
            return INT_MAX;
//使用异或简化代码
        int sign = ((dividend < 0) ^ (divisor < 0)) ? -1 : 1;
//注意这里需要左移，此处的类型应该定义为long long(8 byte)
        long long dvd = labs(dividend);
        long long dvs = labs(divisor);
        int res = 0;
//eg: (45/3=15)
//15==8+4+2+1
        while (dvd >= dvs) {
//care long long
            long long temp = dvs, multiple = 1;
            while (dvd >= (temp << 1)) {
                temp <<= 1;
                multiple <<= 1;
            }
            dvd -= temp;
            res += multiple;
        }
        return sign == 1 ? res : -res;
    }
};
```






[上一级](base.md)
[上一篇](count_and_say.md)
[下一篇](generate_parentheses.md)
