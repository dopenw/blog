# 从排序的listnode列表中删除重复node


Given a sorted linked list, delete all duplicates such that each element appear only once.

For example,
Given 1->1->2, return 1->2.
Given 1->1->2->3->3, return 1->2->3.

```c++
  struct ListNode {
      int val;
      ListNode *next;
      ListNode(int x) : val(x), next(NULL) {}
  };
```

algorithm 0:
```c++

class Solution {
public:
    ListNode* deleteDuplicates(ListNode* head) {
        if (head==NULL)
            return NULL;
        ListNode *item=head;
        ListNode *tmp=head;
        int tmp_value=head->val;
        while (item)
        {
            if (item->val !=tmp_value)
            {
                tmp->next=item;
                tmp=item;
                tmp_value=tmp->val;
            }
            item=item->next;
        }
        tmp->next=NULL;
        return head;
    }
};
```

algorithm 1:
```c++
class Solution {
public:
    ListNode* deleteDuplicates(ListNode* head) {
        if (head==NULL)
            return NULL;
        ListNode *item=head;
        while (item && item->next)
        {
            if (item->val ==item->next->val)
            {
                item->next=item->next->next;
            }
            else
                item=item->next;
        }
        return head;
    }
};
```

[solution link](https://leetcode.com/problems/remove-duplicates-from-sorted-list/solution/)












[上一级](base.md)
[上一篇](plus_one.md)
[下一篇](romanToInt.md)
