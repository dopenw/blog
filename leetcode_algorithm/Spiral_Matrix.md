# 螺旋矩阵

Given a matrix of m x n elements (m rows, n columns), return all elements of the matrix in spiral order.

For example,
Given the following matrix:

```
[
 [ 1, 2, 3 ],
 [ 4, 5, 6 ],
 [ 7, 8, 9 ]
]
```

You should return [1,2,3,6,9,8,7,4,5].

## algorithm_0:
```c++
class Solution {
public:
    vector<int> spiralOrder(vector<vector<int>>& matrix) {
        vector<int> res;
        if (matrix.size()==0)
            return res;

        int m=matrix.size(),n=matrix[0].size();
        int m_a=0,m_b=m-1;
        int n_a=0,n_b=n-1;

//care
        while (m_a<=m_b && n_a<=n_b)

        {
            for (int t=n_a;t<=n_b;++t)
                res.push_back(matrix[m_a][t]);

            for (int r=m_a+1;r<=m_b;++r)
                res.push_back(matrix[r][n_b]);

//care
            if (m_a<m_b && n_a<n_b)

           {               
            for (int b=n_b-1;b>=n_a;--b)
                res.push_back(matrix[m_b][b]);

            for (int l=m_b-1;l>m_a;--l)
                res.push_back(matrix[l][n_a]);

            }
            ++m_a;
            --m_b;
            ++n_a;
            --n_b;
        }


        return res;
    }
};
```


[Solution link](https://leetcode.com/problems/spiral-matrix/solution/)
[上一级](base.md)
[上一篇](Search_in_Rotated_Sorted_Array.md)
[下一篇](Swap_Nodes_in_Pairs.md)
