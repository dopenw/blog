# 排序


<!-- @import "[TOC]" {cmd="toc" depthFrom=1 depthTo=6 orderedList=false} -->
<!-- code_chunk_output -->

* [排序](#排序)
	* [选择排序](#选择排序)
	* [冒泡排序](#冒泡排序)
	* [插入排序](#插入排序)
	* [归并排序](#归并排序)
	* [快速排序](#快速排序)
		* [快速排序的性能](#快速排序的性能)
			* [最坏情况划分](#最坏情况划分)
			* [最好情况划分](#最好情况划分)
		* [快速排序的随机化版本](#快速排序的随机化版本)
	* [堆排序](#堆排序)
	* [计数排序](#计数排序)
	* [桶排序](#桶排序)

<!-- /code_chunk_output -->


## 选择排序

[选择排序](https://zh.wikipedia.org/wiki/%E9%80%89%E6%8B%A9%E6%8E%92%E5%BA%8F)（Selection sort）是一种简单直观的排序算法。它的工作原理如下。首先在未排序序列中找到最小（大）元素，存放到排序序列的起始位置，然后，再从剩余未排序元素中继续寻找最小（大）元素，然后放到已排序序列的末尾。以此类推，直到所有元素均排序完毕。

```c++
template <typename T> void selection_sort(std::vector<T> &v) {
  int count=v.size();
  for (int i = 0; i < count - 1; i++) {
    int min = i;
    for (int j = i + 1; j < count; j++) {
      if (v[min] > v[j])
        min = j;
    }
    swap(v[i], v[min]);
  }
}
```

| 是不是稳定的排序 | 时间复杂度           | 空间复杂度 |
|------------------|----------------------|------------|
| 不是             | 最差，平均都是O(n^2) | 1          |

## 冒泡排序

[冒泡排序](https://zh.wikipedia.org/wiki/%E5%86%92%E6%B3%A1%E6%8E%92%E5%BA%8F#C++)（英语：Bubble Sort，台湾另外一种译名为：泡沫排序）是一种简单的排序算法。它重复地走访过要排序的数列，一次比较两个元素，如果他们的顺序错误就把他们交换过来。走访数列的工作是重复地进行直到没有再需要交换，也就是说该数列已经排序完成。这个算法的名字由来是因为越小的元素会经由交换慢慢“浮”到数列的顶端。

```c++
template <typename T> void bubble_sort(std::vector<T> &v) {
  int count = v.size();
  for (int i = 0; i < count - 1; i++) {
    for (int j = 0; j < count - i - 1; j++) {
      if (v[j] > v[j + 1])
        swap(v[j], v[j + 1]);
    }
  }
}
```

| 是不是稳定的排序 | 时间复杂度           | 空间复杂度 |
|------------------|----------------------|------------|
| 是             | 最差，平均都是 O(n^2);最好是 O(n) | 1          |

## 插入排序

[插入排序](https://zh.wikipedia.org/wiki/%E6%8F%92%E5%85%A5%E6%8E%92%E5%BA%8F)（英语：Insertion Sort）是一种简单直观的排序算法。它的工作原理是通过构建有序序列，对于未排序数据，在已排序序列中从后向前扫描，找到相应位置并插入。插入排序在实现上，通常采用in-place排序（即只需用到 O(1) 的额外空间的排序），因而在从后向前扫描过程中，需要反复把已排序元素逐步向后挪位，为最新元素提供插入空间。

```c++
template <typename T> void insertion_sort(std::vector<T> &v) {
  T key;
  int i;
  int count = v.size();
  for (int j = 1; j < count; j++) {
    key = v[j];
    i = j - 1;
    while (v[i] > key && i >= 0) {
      v[i + 1] = v[i];
      i--;
    }
    v[i + 1] = key;
  }
}
```

| 是不是稳定的排序 | 时间复杂度           | 空间复杂度 |
|------------------|----------------------|------------|
| 是             | 最差，平均都是 O(n^2);最好是 O(n) | 1          |

## 归并排序

[归并排序](https://zh.wikipedia.org/wiki/%E5%BD%92%E5%B9%B6%E6%8E%92%E5%BA%8F)（英语：Merge sort，或mergesort），是创建在归并操作上的一种有效的排序算法。1945年由约翰·冯·诺伊曼首次提出。该算法是采用 [分治法](https://zh.wikipedia.org/wiki/%E5%88%86%E6%B2%BB%E6%B3%95)（Divide and Conquer）的一个非常典型的应用，且各层分治递归可以同时进行。

```c++
void merge(int arr[], int l, int m, int r) {
  int i, j, k;
  int n1 = m - l + 1;
  int n2 = r - m;

  // create temp arrays
  int L[n1], R[n2];

  // Copy data to temp arrays L[] and R[]
  for (i = 0; i < n1; i++)
    L[i] = arr[l + i];
  for (j = 0; j < n2; j++)
    R[j] = arr[m + 1 + j];

  // Merge the temp arrays back into arr[l..r]
  i = 0; // Initial index of first subarray
  j = 0; // Initial index of second subarray
  k = l; // Initial index of merged subarray
  while (i < n1 && j < n2) {
    if (L[i] <= R[j]) {
      arr[k] = L[i];
      i++;
    } else {
      arr[k] = R[j];
      j++;
    }
    k++;
  }

  // Copy the remaining elements of L[], if there are any
  while (i < n1) {
    arr[k] = L[i];
    i++;
    k++;
  }

  // Copy the remaining elements of R[], if there are any
  while (j < n2) {
    arr[k] = R[j];
    j++;
    k++;
  }
}

// l is for left index and r is right index of the sub-array of arr to be sorted
void mergeSort(int arr[], int l, int r) {
  if (l < r) {
    // Same as (l+r)/2, but avoids overflow for
    // large l and h
    int m = l + (r - l) / 2;

    // Sort first and second halves
    mergeSort(arr, l, m);
    mergeSort(arr, m + 1, r);

    merge(arr, l, m, r);
  }
}
```

[geeksforgeeks.org/merge sort](https://www.geeksforgeeks.org/merge-sort/)

| 是不是稳定的排序 | 时间复杂度           | 空间复杂度 |
|------------------|----------------------|------------|
| 是   | O(n log n) |      O(n)    |

## 快速排序

[快速排序](https://zh.wikipedia.org/wiki/%E5%BF%AB%E9%80%9F%E6%8E%92%E5%BA%8F#C++)（英语：Quicksort），又称划分交换排序（partition-exchange sort），简称快排，一种排序算法，最早由东尼·霍尔提出。

```c++
template <typename T>
int quick_sort_partition(std::vector<T> &v, int low, int high) {
  int pivot = v[high];
  int i = (low - 1);

  for (int j = low; j <= high - 1; j++) {
    if (v[j] <= pivot) {
      i++;
      swap(v[i], v[j]);
    }
  }
  swap(v[i + 1], v[high]);
  return i + 1;
}

template <typename T> void quick_sort(std::vector<T> &v, int low, int high) {
  if (low < high) {
    int pi = quick_sort_partition(v, low, high);

    quick_sort(v, low, pi - 1);
    quick_sort(v, pi + 1, high);
  }
}
```

[geeksforgeeks.org/quick-sort](https://www.geeksforgeeks.org/quick-sort/)

### 快速排序的性能

快速排序的运行时间依赖于划分是否平衡，而平衡与否又依赖于用于划分的元素。如果划分是平衡的，那么其算法性能与归并排序一样。如果划分是不平衡的，那么快速排序的性能接近于插入排序了。

#### 最坏情况划分
当划分产生的两个子问题分别包含了 n-1 个元素和 0 个元素，快速排序的最坏情况发生了。

#### 最好情况划分
在可能的最平衡的划分中，quick_sort_partition 得到的两个子问题的规模都不大于 n/2 。在这种情况下，快速排序的性能非常好。

### 快速排序的随机化版本

下面是该算法的伪代码：

```code
RANDOMIZED-PARTITION(A,p,r)
	i=RANDOM(p,r)
	exchange A[r] with A[i]
	return PARTITION(A,p,r)
```

```code
RANDOMIZED-QUICKSORT(A,p,r)
	if p<r
		q=RANDOMIZED-PARTITION(A,p,r)
		RANDOMIZED-QUICKSORT(A,p,q-1)
		RANDOMIZED-QUICKSORT(A,q+1,r)
```


| 是不是稳定的排序 | 时间复杂度           | 空间复杂度 |
|------------------|----------------------|------------|
| 不是   | 平均O(n log n),最坏情况下 O(n^2) |      O(log n)    |

## 堆排序

[堆排序](https://zh.wikipedia.org/wiki/%E5%A0%86%E6%8E%92%E5%BA%8F)（Heapsort）是指利用堆这种数据结构所设计的一种排序算法。堆积是一个近似完全二叉树的结构，并同时满足堆积的性质：即子结点的键值或索引总是小于（或者大于）它的父节点。

```c++
void heapify(std::vector<int> &v, int n, int i) {
  int largest = i;
  int l = 2 * i + 1;
  int r = 2 * i + 2;

  if (l < n && v[l] > v[largest])
    largest = l;
  if (r < n && v[r] > v[largest])
    largest = r;

  if (largest != i) {
    swap(v[i], v[largest]);
    heapify(v, n, largest);
  }
}

void heap_sort(std::vector<int> &v) {
  int n = v.size();
  for (int i = n / 2 - 1; i >= 0; i--)
    heapify(v, n, i);

  for (int i = n - 1; i >= 0; i--) {
    swap(v[0], v[i]);
    heapify(v, i, 0);
  }
}
```

[geeksforgeeks.org/heap-sort](https://www.geeksforgeeks.org/heap-sort/)

| 是不是稳定的排序 | 时间复杂度           | 空间复杂度 |
|------------------|----------------------|------------|
| 不是             | O(n log n) 	|    			 O(1)      |

## 计数排序

[计数排序](https://zh.wikipedia.org/wiki/%E8%AE%A1%E6%95%B0%E6%8E%92%E5%BA%8F)（Counting sort）是一种稳定的线性时间排序算法。计数排序使用一个额外的数组 C ，其中第i个元素是待排序数组  A 中值等于 i 的元素的个数。然后根据数组 C 来将  A 中的元素排到正确的位置。

```c++
// A 为代排序的vector，B 为 排序后的 vector,k 为 A 中最大的值 + 1
void counting_sort(std::vector<int> &A, std::vector<int> &B, int k) {
  std::vector<int> C(k, 0);
  int countA = A.size();
  for (int j = 0; j < countA; j++)
    C[A[j]] = C[A[j]] + 1;

  for (int i = 1; i < k; i++)
    C[i] = C[i - 1] + C[i];

  for (int j = countA; j > 0; j--) {
    B[--C[A[j - 1]]] = A[j - 1];
  }
}
```

| 是不是稳定的排序 | 时间复杂度           | 空间复杂度 |
|------------------|----------------------|------------|
| 是             | O(n+k) 	|    			 O(n+k)      |

## 桶排序

[桶排序](https://zh.wikipedia.org/wiki/%E6%A1%B6%E6%8E%92%E5%BA%8F#C++%E5%AE%9E%E7%8E%B0%E7%AE%97%E6%B3%95)（Bucket sort）或所谓的箱排序，是一个排序算法，工作的原理是将数组分到有限数量的桶里。每个桶再个别排序（有可能再使用别的排序算法或是以递归方式继续使用桶排序进行排序）。桶排序是鸽巢排序的一种归纳结果。当要被排序的数组内的数值是均匀分配的时候，桶排序使用线性时间 O(n) 。但桶排序并不是比较排序，他不受到 O(n log n)下限的影响。

桶排序以下列程序进行：

设置一个定量的数组当作空桶子。
寻访序列，并且把项目一个一个放到对应的桶子去。
对每个不是空的桶子进行排序。
从不是空的桶子里把项目再放回原来的序列中。

下面是处理诸如{0.23,0.25,0.29,0.1,0.56,0.79,0.99}这样的数组的伪代码：
```code
BUCKET_SORT(A)
	n=A.length
	let B[0..n-1] be a new array
	for i=0 to n-1
		make B[i] an empty list
	for i=1 to n
		insert A[i] into list B[ nA[i] ] //向下取整
	for i=0 to n-1
		sort list B[i] with insertion sort
	concatenate the list B[0],B[1],...,B[n-1] together in order
```

| 是不是稳定的排序 | 时间复杂度           | 空间复杂度 |
|------------------|----------------------|------------|
| 是             | O(n) 	|    			 O(k)      |
[上一级](base.md)
[上一篇](mulThread.md)
[下一篇](tree.md)
