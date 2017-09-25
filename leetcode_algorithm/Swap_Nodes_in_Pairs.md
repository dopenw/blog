# 交换一对链表节点

Given a linked list, swap every two adjacent nodes and return its head.

For example,
Given
```
 1->2->3->4
```

 you should return the list as
```
2->1->4->3
```

Your algorithm should use only constant space. You may not modify the values in the list, only nodes itself can be changed.

ListNode :
```c++
  //Definition for singly-linked list.
  struct ListNode {
      int val;
      ListNode *next;
      ListNode(int x) : val(x), next(NULL) {}
  };
```

## my_algorithm
```c++
class Solution {
public:
    ListNode* swapPairs(ListNode* head) {
        ListNode *dum=new ListNode(0);
        ListNode *tt=dum;
        while(head!=NULL)
        {
                ListNode *item1=head;
                // cout<<item1->val<<endl;
                ListNode *item2=head->next;
            if (item2==NULL)
            {
                dum->next=item1;
                return tt->next;
            }
            // cout<<item2->val<<endl;
                ListNode *tmp=head->next->next;
                item2->next=item1;
                item1->next=NULL;
                dum->next=item2;
            // cout<<"dum_next"<<dum->next->val<<endl;
                dum=dum->next->next;
            // cout<<"dum"<<dum->val<<endl;
              // cout<<"test"<<endl;
                head=tmp;

        }
        return tt->next;
    }
};
```
改进版：
```c++
class Solution {
public:
    ListNode* swapPairs(ListNode* head) {
        ListNode *dum=new ListNode(0);
        ListNode *tt=dum;
        while(head!=NULL)
        {
                ListNode *item1=head;
                // cout<<item1->val<<endl;
                ListNode *item2=head->next;
            if (item2==NULL)
            {
                dum->next=item1;
                return tt->next;
            }
            // cout<<item2->val<<endl;
                ListNode *tmp=head->next->next;
                item2->next=item1;
                item1->next=tmp;
                dum->next=item2;
            // cout<<"dum_next"<<dum->next->val<<endl;
                dum=item1;
            // cout<<"dum"<<dum->val<<endl;
              // cout<<"test"<<endl;
                head=tmp;

        }
        return tt->next;
    }
};
```


## other:

较自己的实现简洁的代码：
```c++
class Solution {
public:
    ListNode* swapPairs(ListNode* head) {
        ListNode * dummy = new ListNode (0);
        ListNode * pre = dummy , *cur=head, *next;

        while(cur) {
            if (cur->next) {
                pre->next = cur->next;
                next = cur->next->next;
                cur->next->next = cur;
                cur->next = NULL;
                pre = cur;
                cur = next;
            } else {
                pre->next = cur;
                cur = NULL;
            }


        }

        cur = dummy->next;
        delete[] dummy;
        return cur;
    }
};
```

```c++
ListNode* swapPairs(ListNode* head) {
    ListNode **pp = &head, *a, *b;
    while ((a = *pp) && (b = a->next)) {
        a->next = b->next;
        b->next = a;
        *pp = b;
        pp = &(a->next);
    }
    return head;
}
```


[source link](https://leetcode.com/problems/swap-nodes-in-pairs/discuss/)



























[上一级](base.md)
[下一篇](ZigZag_Conversion.md)
