# 置换序列

The set [1,2,3,…,n] contains a total of n! unique permutations.

By listing and labeling all of the permutations in order,
We get the following sequence (ie, for n = 3):

1. "123"
2. "132"
3. "213"
4. "231"
5. "312"
6. "321"

Given n and k, return the kth permutation sequence.

Note: Given n will be between 1 and 9 inclusive.

## algorithm_0(199/200):

* Time Limit Exceeded
```c++
class Solution {
public:
    string getPermutation(int n, int k) {
        vector<int> item(n);
        iota(item.begin(),item.end(),1);
        string res;
        int loc=1;
        if (k>rec_n(n))
            return res;
        rec(item,0,loc,res,k);
        return res;
    }

    private:

    int rec_n(int n)
    {
        int sum=1;
        if (n==1)
            return 1;
        return sum=n*rec_n(n-1);
    }

    void rec(vector<int> item,int begin,int &permutation_loc,string &res,int k)
    {
        if (permutation_loc > k)
            return;
        if (begin==item.size()-1)
        {
            if (permutation_loc==k )
            {
            for (auto i:item)
            {
                // cout<<i<<",";
                res+=to_string(i);
            }
            }
            permutation_loc+=1;
            return ;
        }

        for (int i=begin;i<item.size();i++)
        {
            swap(item[begin],item[i]);
            rec(item,begin+1,permutation_loc,res,k);
        }

    }
};
```

## algorithm_1:
eg:
```
input:
n=3,k=2;

process:
f=6;
step 1:
f=2; j=0+1/2; k=1;
step 2:
f=1;j=1+1/1; k=0;
step 3:
f=1;j=2+0/1; k=0;

result:
1,3,2;

```


```c++
string getPermutation(int n, int k) {
    int i,j,f=1;
    // left part of s is partially formed permutation, right part is the leftover chars.
    string s(n,'0');
    for(i=1;i<=n;i++){
        f*=i;
        s[i-1]+=i; // make s become 1234...n
    }
    for(i=0,k--;i<n;i++){
        f/=n-i;
        j=i+k/f; // calculate index of char to put at s[i]
        char c=s[j];
        // remove c by shifting to cover up (adjust the right part).
        for(;j>i;j--)
            s[j]=s[j-1];
        k%=f;
        s[i]=c;
    }
    return s;
}
```

[souce link](https://leetcode.com/problems/permutation-sequence/discuss/)
[上一级](README.md)
[上一篇](Multiply_Strings.md)
[下一篇](Permutations.md)
