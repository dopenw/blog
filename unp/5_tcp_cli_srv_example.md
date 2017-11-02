# 5.TCP客户/服务器程序示例


<!-- @import "[TOC]" {cmd="toc" depthFrom=1 depthTo=6 orderedList=false} -->
<!-- code_chunk_output -->

* [5.TCP客户/服务器程序示例](#5tcp客户服务器程序示例)
	* [概述](#概述)
	* [服务器程序](#服务器程序)
		* [退出函数](#退出函数)
	* [客户端程序](#客户端程序)
	* [链接器如何解析多重定义的全局符号](#链接器如何解析多重定义的全局符号)

<!-- /code_chunk_output -->

## 概述

编写一个简单的回射客户/服务器
1. 客户从标准输入读入一行文本，并写给服务器
2. 服务器从网络输入读入这行文本，并回射给客户
3. 客户从网络输入读入这行回射文本，并显示在标准输出上

* 如图所示

![](../images/5_tcp_cli_srv_example_201711021652_1.png)


## 服务器程序

```c
#include "unp.h"

void str_echo(int sockfd) {
  ssize_t n;
  char buf[MAXLINE];

again:
  while ((n = read(sockfd, buf, MAXLINE)) > 0)
    Write(sockfd, buf, n);

  // EINTR  The accept() function was interrupted by a  signal  that  was  caught  before  a  valid  connection
  // arrived.
  if (n < 0 && errno == EINTR)
    goto again;

  else if (n < 0)
    err_sys("str_echo:read error");
}

int main(int argc, char const *argv[]) {
  int listenfd, connfd;
  pid_t childpid;
  socklen_t clilen;
  struct sockaddr_in cliaddr, servaddr;

  listenfd = Socket(AF_INET, SOCK_STREAM, 0);

  bzero(&servaddr, sizeof(servaddr));
  servaddr.sin_family = AF_INET;
  servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
  servaddr.sin_port = htons(SERV_PORT);

  Bind(listenfd, (SA *)&servaddr, sizeof(servaddr));

  Listen(listenfd, LISTENQ);

  while (1) {
    clilen = sizeof(cliaddr);
    connfd = Accept(listenfd, (SA *)&cliaddr, &clilen);

    if ((childpid = Fork()) == 0) {
      Close(listenfd);
      str_echo(connfd);

      //care
      exit(0);
      //见下面的退出函数

    }
    Close(connfd);
  }
  return 0;
}

```

### 退出函数

```c
#include <stdlib.h>

void exit(int status);
void _Exit(int status);

#include <unistd.h>

void _exit(int status);

```

3个函数用于正常终止一个程序，\_exit 和 \_Exit立即进入内核，exit先执行一些清理处理，然后返回内核。

可以使用atexit函数来登记多至32个的终止处理函数，这些函数由exit自动调用。

```c
#include <stdlib.h>

int atexit(void (*func) (void));
```

## 客户端程序

```c
#include "unp.h"

void str_cli(FILE *fp, int sockfd) {
  char sendline[MAXLINE], recvline[MAXLINE];
  while ((Fgets(sendline, MAXLINE, fp)) != NULL) {
    Write(sockfd, sendline, strlen(sendline));

    if (Readline(sockfd, recvline, MAXLINE) == 0)
      err_quit("str_cli:server terminated prematurely");
    Fputs(recvline, stdout);
  }
}

int main(int argc, char const *argv[]) {
  int sockfd;

  struct sockaddr_in servaddr;

  if (argc != 2) {
    err_quit("usage:tcpcli <IP address>");
  }

  sockfd = Socket(AF_INET, SOCK_STREAM, 0);

  bzero(&servaddr, sizeof(servaddr));
  servaddr.sin_family = AF_INET;
  servaddr.sin_port = htons(SERV_PORT);

  Inet_pton(AF_INET, argv[1], &servaddr.sin_addr);

  Connect(sockfd, (SA *)&servaddr, sizeof(servaddr));

  str_cli(stdin, sockfd);

  return 0;
}

```

## 链接器如何解析多重定义的全局符号

* 详见 深入理解计算机系统 7.6.1

函数和已初始化的全局变量是强符号，未初始化的全局变量是弱符号。


根据强弱符号的定义，unix链接器使用下面的规则来处理多重定义的符号：
1. 不允许有多个强符号
2. 如果有一个强符号和多个弱符号，那么选择强符号
3. 如果有多个弱符号，那么从这些弱符号中任意选择一个

规则2和规则3的应用会造成一些不易察觉的运行时错误，对于不警惕的程序员来说，这是很难理解的，尤其当如果重复的符号定义还有不同的类型时。

考虑下面的例子：

```c
//foo5.c
#include <stdio.h>

void f(void);

int x=15213;
int y=15212;

int main(int argc, char const *argv[]) {
  f();
  printf("x=0x%x  y=0x%x \n",x,y );
  return 0;
}
```

```c
//bar5.c

double x;
void f()
{
  x=-0.0;
}
```

double 8 byte,int 4 byte。因此，bar5.c的x=-0.0将用负零的双精度浮点表示覆盖存储器中的x和y的位置。

```
gcc -o foobar5 foo5.c bar5.c

/usr/bin/ld: Warning: alignment 4 of symbol `x' in /tmp/ccPxoo2Q.o is smaller than 8 in /tmp/cc26wO2g.o

./foobar5

x=0x0 y=0x800000000
```

```c
//foo51.c
#include <stdio.h>

void f(void);

int x=15213;
int y=15212;

int main(int argc, char const *argv[]) {
  f();
  printf("x=0x%x  y=0x%x \n",x,y );
  return 0;
}
```

```c
//bar51.c

double x;
void f()
{
  x=0.0;
}
```

show:
```
x=0x0  y=0x0
```

[上一级](base.md)
[上一篇](4_basic_tcp_socket.md)
