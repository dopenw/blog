# Add Two (ListNode) Numbers

You are given two non-empty linked lists representing two non-negative integers. The digits are stored in reverse order and each of their nodes contain a single digit. Add the two numbers and return it as a linked list.

You may assume the two numbers do not contain any leading zero, except the number 0 itself.

Input: (2 -> 4 -> 3) + (5 -> 6 -> 4)
Output: 7 -> 0 -> 8

```c++
/**
 * Definition for singly-linked list.
 * struct ListNode {
 *     int val;
 *     ListNode *next;
 *     ListNode(int x) : val(x), next(NULL) {}
 * };
 */
```

algorithm_0(38ms):
```c++
class Solution {
public:

    ListNode* addTwoNumbers(ListNode* l1, ListNode* l2) {
        ListNode *result=new ListNode(0);
        ListNode *head=result;
        ListNode *front=NULL;
        bool flag=false;
        while(true)
        {
            ListNode *tmp=new ListNode(0);
            if (l1==NULL && l2==NULL)
            {
              if(!flag)
              front->next=NULL;
                 break;
            }
               flag=false;
            if (l1!=NULL && l2!=NULL)
            {

                result->val=l1->val+l2->val+result->val;

                l1=l1->next;
                l2=l2->next;
                    result->next=tmp;
                front=result;
                if (result->val >9)
                {
                    result->val=result->val%10;
                    result->next->val=result->next->val+1;
                    flag=true;
                }
                result=result->next;
                continue;
                //break;
            }
            if (l1!=NULL && l2==NULL)
            {
                result->val=l1->val+result->val;
                l1=l1->next;
                result->next=tmp;
                   front=result;
                if (result->val >9)
                {
                    result->val=result->val%10;
                    result->next->val=result->next->val+1;
                    flag=true;
                }
                result=result->next;
                continue;
            }
            if (l1==NULL && l2!=NULL)
            {
                result->val=l2->val+result->val;
                l2=l2->next;
                result->next=tmp;
                   front=result;
                if (result->val >9)
                {
                    result->val=result->val%10;
                    result->next->val=result->next->val+1;
                    flag=true;
                }

                result=result->next;
                continue;
            }
        }
        return head;
    }
};
```

algorithm_0_1(55ms):
```
class Solution {
public:

    ListNode* addTwoNumbers(ListNode* l1, ListNode* l2) {
        ListNode *result=new ListNode(0);
        ListNode *head=result;
        int add_tmp=0;
        int sum_num=0;
        while(add_tmp || l1 || l2)
        {
            int l1_num=(l1) ? l1->val:0;
            int l2_num=(l2) ? l2->val:0;
            sum_num=add_tmp+l1_num+l2_num;
            if (sum_num>9)
            {
                sum_num=sum_num%10;
                add_tmp=1;
            }
            else
                add_tmp=0;
            result->next=new ListNode(sum_num);
            l1=l1 ? l1->next:l1;
            l2=l2 ? l2->next:l2;
            result=result->next;
        }
        return head->next;
    }
};
```

algorithm_0_2(40ms):
```c++
class Solution {
public:

    ListNode* addTwoNumbers(ListNode* l1, ListNode* l2) {
        ListNode *result=new ListNode(0);
        ListNode *head=result;
        int add_tmp=0;
        int sum_num=0;
        while(add_tmp || l1 || l2)
        {
            int l1_num=(l1) ? l1->val:0;
            int l2_num=(l2) ? l2->val:0;
            sum_num=add_tmp+l1_num+l2_num;
            if (sum_num>9)
            {
                sum_num=sum_num%10;
                add_tmp=1;
            }
            else
                add_tmp=0;
            result->next=new ListNode(sum_num);
            if (l1)
                l1=l1->next;
            if (l2)
                l2=l2->next;
            result=result->next;
        }
        return head->next;
    }
};
```
[Solution link](https://leetcode.com/problems/add-two-numbers/solution/)













[上一级](base.md)
[上一篇](add_binary.md)
[下一篇](climbing_stairs.md)
