
<!-- @import "[TOC]" {cmd="toc" depthFrom=1 depthTo=6 orderedList=false} -->
<!-- code_chunk_output -->

* [进程间通信](#进程间通信)
	* [管道](#管道)
		* [函数popen和pclose](#函数popen和pclose)

<!-- /code_chunk_output -->


# 进程间通信

经典的IPC：
* 管道
* FIFO
* 消息队列
* 信号量
* 共享存储

## 管道

管道是unix系统IPC的最古老的形式，所有的unix系统都提供此种通信机制。

管道有以下两种局限性：
1. 历史上，他们是半双工的（即数据只能在一个方向上流动）。现在，某些系统提供全双工管道，但是为了最佳的可移植性，我们绝不预先假设系统支持全双工管道。

2. 管道只能在具有公共祖先的两个进程之间使用。

管道可以通过pipe函数创建。
```c
#include <unistd.h>
int pipe(int fd[2]);
```
fd[0]为读打开，fd[1]为写打开。fd[1]的输出是fd[0]的输入。


### 函数popen和pclose

常见的操作是创建一个连接到另一个进程的管道，然后读其输出或向其输入端发送数据，为此，标准I/O库提供了popen和pclose。这两个函数实现的操作是：创建一个管道，fork一个自进程，关闭未使用的管道端，执行一个shell运行命令，然后等待命令终止。
```c
#include "stdio.h"

FILE * popen(const char *cmdstring,const char *type);

int pclose(FILE *fp);
```
eg:
```c
fp=popen("ls *.c","r");
```
[上一级](base.md)
[下一篇](pthread.md)
