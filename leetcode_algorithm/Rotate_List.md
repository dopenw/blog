# 旋转列表
Given a list, rotate the list to the right by k places, where k is non-negative.

For example:
Given 1->2->3->4->5->NULL and k = 2,
return 4->5->1->2->3->NULL.

```c++
  Definition for singly-linked list.
  struct ListNode {
      int val;
      ListNode *next;
      ListNode(int x) : val(x), next(NULL) {}
  };
```


## algorithm_1:
```c++
class Solution {
public:
    ListNode* rotateRight(ListNode* head, int k) {
        if(!head) return head;

        int len=1; // number of nodes
        ListNode *newH, *tail;
        newH=tail=head;

        while(tail->next)  // get the number of nodes in the list
        {
            tail = tail->next;
            len++;
        }
        tail->next = head; // circle the link

        if(k %= len)
        {
            for(auto i=0; i<len-k; i++) tail = tail->next; // the tail node is the (len-k)-th node (1st node is head)
        }
        newH = tail->next;
        tail->next = NULL;
        return newH;
    }
};
```

## algorithm_0_-1:
* error：错误理解需求

```c++
class Solution {
public:
    ListNode* rotateRight(ListNode* head, int k) {
        ListNode *tt=head;
        if (!head)
            return head;

       while (head->next && (k--)>0)
       {
           head=head->next;
       }

        ListNode *tmp=head->next;
        ListNode *newhead=tmp;

        head->next=NULL;
        // cout<<"head:"<<head->val;

        while(tmp && tmp->next)
        {
            cout<<tmp->val;
            tmp=tmp->next;
        }
        if (tmp)
        tmp->next=tt;

        return newhead;
    }
};
```

[source link](https://leetcode.com/problems/rotate-list/discuss/)
- [上一级](README.md)
- 上一篇 -> [排列 2](Permutations_II.md)
- 下一篇 -> [搜索旋转排序数组](Search_in_Rotated_Sorted_Array.md)
