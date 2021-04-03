# 判断两个二叉树是不是相同的(same_tree)

Given two binary trees, write a function to check if they are equal or not.

Two binary trees are considered equal if they are structurally identical and the nodes have the same value.

```
/**
 * Definition for a binary tree node.
 * struct TreeNode {
 *     int val;
 *     TreeNode *left;
 *     TreeNode *right;
 *     TreeNode(int x) : val(x), left(NULL), right(NULL) {}
 * };
 */
```


algorithm_1:
```c++
class Solution {
public:
    bool isSameTree(TreeNode* p, TreeNode* q) {
        if (p==NULL || q==NULL) return (p==q);
        return (p->val==q->val && isSameTree(p->left,q->left) && isSameTree(p->right,q->right));

    }
};
```

[source link](https://leetcode.com/problems/same-tree/discuss/)


---
- [上一级](README.md)
- 上一篇 -> [旋转图像](rotate_image.md)
- 下一篇 -> [给定一个排序的数组，和一个目标值，查找该值在数组的范围](search_for_a_range.md)
