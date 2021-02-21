# 螺旋矩阵II

Given an integer n, generate a square matrix filled with elements from 1 to n2 in spiral order.

For example,
Given n = 3,

You should return the following matrix:
```
[
 [ 1, 2, 3 ],
 [ 8, 9, 4 ],
 [ 7, 6, 5 ]
]
```

## algorithm_0(3ms):
```c++
class Solution {
public:
    vector<vector<int>> generateMatrix(int n) {
        //init matrix

        vector<vector<int>> res;
        vector<int> tmp(n,0);
        for (int i=0;i<n;++i)
        {
            res.push_back(tmp);
        }

    int a=0,b=n-1;
    int number=1;
        while(a<=b)
        {
            for (int t=a;t<=b;++t)
                res[a][t]=number++;
            for (int r=a+1;r<=b;++r)
                res[r][b]=number++;
            for (int bu=b-1;bu>=a;--bu)
                res[b][bu]=number++;
            for (int l=b-1;l>a;--l)
                res[l][a]=number++;
            ++a;
            --b;
        }
        return res;
    }
};
```

## algorithm_0_1:

```c++
class Solution {
public:
    vector<vector<int>> generateMatrix(int n) {

        //init matrix
        vector<vector<int>> res(n,vector<int> (n,0));

    int a=0,b=n-1;
    int number=1;
        while(a<=b)
        {
            for (int t=a;t<=b;++t)
                res[a][t]=number++;
            for (int r=a+1;r<=b;++r)
                res[r][b]=number++;
            for (int bu=b-1;bu>=a;--bu)
                res[b][bu]=number++;
            for (int l=b-1;l>a;--l)
                res[l][a]=number++;
            ++a;
            --b;
        }
        return res;
    }
};
```


[source link](https://leetcode.com/problems/spiral-matrix-ii/discuss/)
[上一级](README.md)
[上一篇 -> 螺旋矩阵](Spiral_Matrix.md)
[下一篇 -> 交换一对链表节点](Swap_Nodes_in_Pairs.md)
