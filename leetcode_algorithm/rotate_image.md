# 旋转图像

You are given an n x n 2D matrix representing an image.

Rotate the image by 90 degrees (clockwise).

Note:
You have to rotate the image in-place, which means you have to modify the input 2D matrix directly. DO NOT allocate another 2D matrix and do the rotation.

Example 1:

```
Given input matrix =
[
  [1,2,3],
  [4,5,6],
  [7,8,9]
],

rotate the input matrix in-place such that it becomes:
[
  [7,4,1],
  [8,5,2],
  [9,6,3]
]
```

Example 2:
```
Given input matrix =
[
  [ 5, 1, 9,11],
  [ 2, 4, 8,10],
  [13, 3, 6, 7],
  [15,14,12,16]
],

rotate the input matrix in-place such that it becomes:
[
  [15,13, 2, 5],
  [14, 3, 4, 1],
  [12, 6, 8, 9],
  [16, 7,10,11]
]
```


algorithm_1_0:
```c++
class Solution {
public:

  /*
   * clockwise rotate
   * first reverse up to down, then swap the symmetry
   * 1 2 3     7 8 9     7 4 1
   * 4 5 6  => 4 5 6  => 8 5 2
   * 7 8 9     1 2 3     9 6 3
  */

    void rotate(vector<vector<int>>& matrix) {

        reverse(matrix.begin(),matrix.end());
        for (int i=0;i<matrix.size();i++)
            for (int j=i+1;j<matrix[i].size();j++)
                swap(matrix[i][j],matrix[j][i]);
    }
};
```

algorithm_2_0:
```c++
class Solution {
public:
    void rotate(vector<vector<int>>& matrix) {


    int b=0,e=matrix.size()-1;

/*

* 1 *       * 3 *       * 4 *       * 2 *
2 * 3   =>  2 * 1   =>  2 * 1  = >  4 * 1
* 4 *       * 4 *       * 3 *       * 3 *

*/

        while(b<e)
        {
            for (int i=0;i<(e-b);i++)
            {
                swap(matrix[b][b+i], matrix[b+i][e]);
                swap(matrix[b][b+i], matrix[e][e-i]);
                swap(matrix[b][b+i], matrix[e-i][b]);
            }
            ++b;
            --e;
        }

    }
};
```

[source link](https://leetcode.com/problems/rotate-image/discuss/)

[上一级](base.md)
[上一篇](romanToInt.md)
[下一篇](same_tree.md)
