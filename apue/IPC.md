# 进程间通信

<!-- @import "[TOC]" {cmd="toc" depthFrom=1 depthTo=6 orderedList=false} -->
<!-- code_chunk_output -->

* [进程间通信](#进程间通信)
	* [管道](#管道)
		* [函数popen和pclose](#函数popen和pclose)
	* [FIFO](#fifo)
		* [FIFO 用途](#fifo-用途)

<!-- /code_chunk_output -->




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

![](../images/IPC_201803241537_1.png)

单个进程中的管道几乎没有任何用处。通常，进程会先调用 pipe，接着调用 fork，从而创建父进程到子进程的 IPC 通道，反之依然。

![](../images/IPC_201803241537_2.png)

fork 之后做什么取决于我们想要的数据流的方向。对于从父进程到子进程的管道，父进程关闭管道的读端，子进程关闭写端。

![](../images/IPC_201803241537_3.png)

对于一个从子进程到父进程的管道，父进程关闭 fd[1],子进程关闭 fd[0].
当管道的一端被关闭后，下列两条规则起作用：
1. 当 读(read) 一个写段已被关闭的管道时，在所有数据都被读取后，read 返回0，表示文件结束
2. 如果 写(write) 一个读端已被关闭的管道，则产生信号 SIGPIPE 。如果该信号或者捕捉信号并从处理程序返回，则write 返回 -1，errno 设置为 EPIPE

示例：
创建了一个从父进程到子进程的管道，并且父进程经由该管道向子进程传送数据。

```c
#include "apue.h"

int main(int argc, char const *argv[]) {
  int n;
  int fd[2];
  pid_t pid;
  char line[MAXLINE];

  if (pipe(fd) < 0)
    err_sys("pipe error");
  if ((pid = fork()) < 0)
    err_sys("fork error");
  else if (pid > 0) {
    close(fd[0]);
    write(fd[1], "hello world\n", 12);
  } else {
    close(fd[1]);
    n = read(fd[0], line, MAXLINE);
    write(STDOUT_FILENO, line, n);
  }

  return 0;
}
```

示例 2：
编写一个程序，其功能是每次一页地显示已产生的输出，该例可以使用 more 这个分页程序。

[strrchr() function](http://www.cplusplus.com/reference/cstring/strrchr/)

```c
#include "apue.h"
#include <sys/wait.h>

#define DEF_PAGER "/bin/more"

int main(int argc, char const *argv[]) {
  int n;
  int fd[2];
  pid_t pid;
  char * pager, * argv0;
  char line[MAXLINE];
  FILE * fp;

  if (argc != 2)
    err_quit("usage: a.out <pathname>");
  if ((fp = fopen(argv[1], "r")) == NULL)
    err_sys("can't open %s", argv[1]);

  if (pipe(fd) < 0)
    err_sys("pipe error");

  if ((pid = fork()) < 0)
    err_sys("fork error");
  else if (pid > 0) { // parent
    close(fd[0]);     // close read end

    // parent copies argv[1] to pipe
    while (fgets(line, MAXLINE, fp) != NULL) {
      n = strlen(line);
      if (write(fd[1], line, n) != n)
        err_sys("write error to pipe");
    }

    if (ferror(fp))
      err_sys("fgets error");

    close(fd[1]);
    // close write end of pipe for reader

    if (waitpid(pid, NULL, 0) < 0)
      err_sys("waitpid error");
    // 等待组 ID 等于调用进程组 ID 的任一子进程

    exit(0);
  }

  else {          // child
    close(fd[1]); // close write end
    if (fd[0] != STDIN_FILENO) {
      if (dup2(fd[0], STDIN_FILENO) != STDIN_FILENO)
        err_sys("dup2 error to stdin");
      close(fd[0]);
      // don't need this after dup2
    }

    // get arguments for execl()
    if ((pager = getenv("PAGER")) == NULL)
      pager = DEF_PAGER;

    if ((argv0 = strrchr(pager, '/')) != NULL)
      argv0++; // step past rightmost slash
    else
      argv0 = pager; // no slash in pager

    if (execl(pager, argv0, (char *)0) < 0)
      err_sys("execl error for %s", pager);
  }
  return 0;
}
```

Run it:

![](../images/IPC_201803241537_4.png)


### 函数popen和pclose

常见的操作是创建一个连接到另一个进程的管道，然后读其输出或向其输入端发送数据，为此，标准I/O库提供了popen和pclose。这两个函数实现的操作是：创建一个管道，fork一个自进程，关闭未使用的管道端，执行一个shell运行命令，然后等待命令终止。
```c
#include "stdio.h"

FILE * popen(const char *cmdstring,const char *type);
// 若成功，返回文件指针；若出错，返回 NULL

int pclose(FILE *fp);
// 若成功，返回 cmdstring的终止状态；若出错，返回 -1
```

![](../images/IPC_201803241606_1.png)

eg:
```c
fp=popen("ls *.c","r");
```

示例：
用 popen 重写之前分页的例子
```c
#include "apue.h"
#include <sys/wait.h>

#define PAGER "${PAGER:-more}"
// 如果 shell 变量 PAGER 已经定义，且其值为非空，则使用其值，否则使用字符串 more

int main(int argc, char const *argv[]) {
  char line[MAXLINE];
  FILE * fpin, * fpout;

  if (argc != 2)
    err_quit("usage: a.out <pathname>");

  if ((fpin = fopen(argv[1], "r")) == NULL)
    err_sys("can't open %s", argv[1]);

  if ((fpout = popen(PAGER, "w")) == NULL)
    err_sys("popen error");

  while (fgets(line, MAXLINE, fpin) != NULL) {
    if (fputs(line, fpout) == EOF)
      err_sys("fputs error to pipe");
  }

  if (ferror(fpin))
    err_sys("fgets error");
  if (pclose(fpout) == -1)
    err_sys("pclose error");

  return 0;
}
```

使用 popen 减少了需要编写的代码量

## FIFO

FIFI 有时被称为命名管道。未命名的管道只能在两个相关的进程之间使用，而且这两个相关的进程还要有一个共同创建了他们的祖先进程。但是，通过 FIFO ，不相关的进程也能够交换数据。

FIFO 是一种文件类型。通过 [stat 结构](https://linux.die.net/man/2/stat) 的 st_mode 成员的编码可以知道文件是否是 FIFO 类型。可以用 S_ISFIFO 宏对此进行测试

创建 FIFO 类似于创建文件。确实，FIFO 的路径名存在于文件系统中。


```c
#include <sys/stat.h>

int mkfifo(const char *path,mode_t mode);
int mkfifoat(int fd,const char *path,mode_t mode);

// 两个函数的返回值：若成功，返回0；若出错，返回 -1
```

mkfifo 函数中的 mode 参数的规格说明与 open 函数中 mode 相同。

mkfifo 函数和 mkfifoat 函数相似，但是 mkfifoat 函数可以被用来在 fd 文件描述符表示的目录相关位置创建一个 FIFO 。像其它 \*at 函数一样，这里有三种情况：
1. 如果 path 参数指定的是绝对路径名，则 fd 参数会被忽略掉，并且 mkfifoat 函数的行为和 mkfifo 类似
2. 如果 path 参数制定的是相对路径名，则 fd 参数是一个打开目录的有效文件描述符，路径名和目录有关
3. 如果 path 参数指定的是相对路径名，并且 fd 参数有一个特殊值 AT_FDCWD，则路径名以当前目录开始，mkfifoat 和 mkfifo 类似

当我们用 mkfifo 或者 mkfifoat 创建 FIFO 时，要用 open 来打开它。确实，正常的文件 I/O 函数 (eg: close,read,write,unlink) 都需要 FIFO

当 open 一个 FIFO 时，非阻塞标志 (O_NONBLOCK) 会产生下列影响：
* 在一般情况下 （没有指定 O_NONBLOCK ），只读 open 要阻塞到某个其他进程为写而打开这个 FIFO 为止。类似地，只写 open 要阻塞到某个其他进程为读而打开它为止
* 如果指定了 O_NONBLOCK ，则只读 open 立即返回。但是，如果没有进程为读而打开一个 FIFO，那么只写 open 将返回 -1，并将 errno 设置为 ENXIO

类似于管道，若 write 一个尚无进程为读而打开的 FIFO ，则产生信号 SIGPIPE。若某个 FIFO 的最后一个写进程关闭了该 FIFO，则将为该 FIFO 的读进程产生一个文件结束标志。

一个给定的 FIFO 有多个写进程是常见的。这就意味着，如果不希望多个进程所写的数据交叉，则必须考虑原子写操作。和管道一样，常量 PIPE_BUF 说明了可被原子地写到 FIFO 的最大数据量。


### FIFO 用途

* shell 命令使用 FIFO 将数据从一条管道传送到另一条时，无需创建中间临时文件
* client - server 应用程序中，FIFO 用作汇聚点，在客户进程和服务器进程二者之间传递数据

示例：
```c++
#include <cstdio>
#include <cstring>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

int main(int argc, char const *argv[]) {
  int fd;

  char * myfifo = "/tmp/myfifo";

  mkfifo(myfifo, 0666);

  char arr1[80], arr2[80];

  while (true) {
    fd = open(myfifo, O_WRONLY);

    fgets(arr2, 80, stdin);

    write(fd, arr2, strlen(arr2) + 1);

    close(fd);

    fd = open(myfifo, O_RDONLY);
    read(fd, arr1, sizeof(arr1));

    printf("User2: %s\n", arr1);
    close(fd);
  }
  return 0;
}
```

[geeksforgeeks.org/named-pipe-fifo-example](https://www.geeksforgeeks.org/named-pipe-fifo-example-c-program/)

[上一级](base.md)
[下一篇](pthread.md)
