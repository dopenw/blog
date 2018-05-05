# STL


<!-- @import "[TOC]" {cmd="toc" depthFrom=1 depthTo=6 orderedList=false} -->
<!-- code_chunk_output -->

* [STL](#stl)
	* [STL 组件](#stl-组件)
	* [容器](#容器)
		* [容器适配器](#容器适配器)
	* [迭代器](#迭代器)
		* [迭代器种类](#迭代器种类)
		* [迭代器适配器](#迭代器适配器)
	* [算法](#算法)
		* [remove 元素](#remove-元素)

<!-- /code_chunk_output -->


标准模板库是C++标准库的核心，她深刻影响了标准库的整体结构。STL 是一个泛型程序库，提供一系列软件方案，利用先进、高效的算法来管理数据。程序员无须了解 STL 的原理，便可享受数据结构和算法领域中的这一革新成果。

## STL 组件

若干精心勾画的组件共同合作，构筑起 STL 的基础。这些基础中最关键的是容器、迭代器和算法。

* 容器，用来管理某类对象的集合
* 迭代器，用来在一个对象集合内遍历元素
* 算法，用来处理集合内的元素。他们可以出于不同的目的而查找、排序、修改、使用元素

## 容器
容器用来管理一大群元素。为了适应不同需要，STL 提供了不同的容器。
总的来说，容器可分为三大类：
1. 序列式容器，这是一种有序集合，其内每个元素均有确凿的位置-取决于插入时机和地点，与元素值无关。array、vector、deque、list、forward_list
2. 关联式容器，这是一种已排序集合，元素位置取决于其 value 和给定的某个排序准则。set、multiset、map、multimap
3. 无序容器，这是一种无序集合，其内的每个元素的位置无关紧要，唯一重要的是某个特定元素是否位于此集合内。元素值或其安插顺序，都不影响元素的位置，而且元素的位置有可能在容器生命中被改变。 unordered_set、unordered_multiset、unordered_map、unordered_multimap

### 容器适配器

* stack
* queue
* priority queue

## 迭代器

### 迭代器种类
* 前向迭代器，只能够以累加操作符向前迭代。forward_list 的迭代器就属此类。其他容器如 unordered_set 、 unordered_multiset 、unordered_map 、 unordered_multimap 也都至少是此类别
* 双向迭代器，它可以双向行进：以递增运算前进或以递减运算后退。list,set,multiset,map,multimap提供的迭代器都属此类
* 随机访问迭代器，它不但具备双向迭代器的所有属性，还具备随机访问的能力。更明确的说，他们提供了迭代器算术运算的必要操作符。你可以对迭代器增加或减少一个偏移量、计算两迭代器间的距离，或使用 < 和 > 之类的 relational 操作符进行比较。vector,array,string 提供的迭代器都属此类

### 迭代器适配器
* insert iterator
* stream iterator
* reverse iterator
* move iterator (since c++11)
## 算法

### remove 元素

删除 list 容器中的所有值为 3 的元素：
```c++
#include <algorithm>
#include <deque>
#include <iostream>
#include <iterator>
#include <list>

using namespace std;

int main(int argc, char const *argv[]) {
  list<int> coll;

  for (ssize_t i = 0; i <= 6; i++) {
    coll.push_front(i);
    coll.push_back(i);
  }

  std::cout << "pre: " << ' ';
  copy(coll.cbegin(), coll.cend(), ostream_iterator<int>(cout, " "));
  std::cout << '\n';

  remove(coll.begin(), coll.end(), 3);

  std::cout << "post: " << ' ';
  copy(coll.cbegin(), coll.cend(), ostream_iterator<int>(cout, " "));
  std::cout << '\n';
  return 0;
}
```

该程序运行后的结果为：
```terminal
pre:  6 5 4 3 2 1 0 0 1 2 3 4 5 6
post:  6 5 4 2 1 0 0 1 2 4 5 6 5 6
```

要想正确的删除元素，可使用：
```c++
coll.erase(remove(coll.begin(),coll.end(),3),coll.end());
```

为何算法不自己调用 erase() 呢？这个问题刚好点出 STL 为获取弹性而付出的代价。   


注： 参考 《c++ 标准库第二版》 第六章
[上一级](base.md)
[上一篇](MFC_VS_QT.md)
[下一篇](chrono.md)
