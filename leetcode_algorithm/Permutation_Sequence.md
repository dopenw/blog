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

##algorithm_0(199/200):

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

[souce link](https://leetcode.com/problems/permutation-sequence/discuss/)
[上一级](base.md)
[上一篇](Multiply_Strings.md)
[下一篇](Permutations.md)
