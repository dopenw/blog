# 5.TCP客户/服务器程序示例


<!-- @import "[TOC]" {cmd="toc" depthFrom=1 depthTo=6 orderedList=false} -->
<!-- code_chunk_output -->

* [5.TCP客户/服务器程序示例](#5tcp客户服务器程序示例)
	* [概述](#概述)
	* [服务器程序](#服务器程序)
		* [退出函数](#退出函数)
	* [客户端程序](#客户端程序)
	* [链接器如何解析多重定义的全局符号](#链接器如何解析多重定义的全局符号)
	* [tcpdump](#tcpdump)
		* [Wireshark](#wireshark)
	* [netstat](#netstat)
	* [posix信号处理](#posix信号处理)
		* [POSIX 信号语义](#posix-信号语义)

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

## tcpdump
[tcpdump](https://zh.wikipedia.org/wiki/Tcpdump) 是一个运行在命令行下的嗅探工具。它允许用户拦截和显示发送或收到过网络连接到该计算机的TCP/IP和其他数据包。tcpdump 是一个在BSD许可证下发布的自由软件。

tcpdump能够分析网络行为，性能和应用产生或接收网络流量。它支持针对网络层、协议、主机、网络或端口的过滤，并提供and、or、not等逻辑语句来帮助你去掉无用的信息，从而使用户能够进一步找出问题的根源。

也可以使用 tcpdump 的实现特定目的，例如在路由器和网关之间拦截并显示其他用户或计算机通信。通过 tcpdump 分析非加密的流量，如Telnet或HTTP的数据包，查看登录的用户名、密码、网址、正在浏览的网站内容，或任何其他信息。因此系统中存在网络分析工具主要不是对本机安全的威胁，而是对网络上的其他计算机的安全存在威胁。

简单使用:
```sh
tcpdump host $IPAddress -vv -X
```

[tcpdump Tutorial](https://danielmiessler.com/study/tcpdump/)

### Wireshark
Wireshark（前称Ethereal）是一个免费开源的网络数据包分析软件。网络数据包分析软件的功能是截取网络数据包，并尽可能显示出最为详细的网络数据包数据。

[fedora install Wireshark](https://fedoramagazine.org/how-to-install-wireshark-fedora/)

## netstat

In computing, [netstat](https://en.wikipedia.org/wiki/Netstat) (network statistics) is a command-line network utility tool that displays network connections for the Transmission Control Protocol (both incoming and outgoing), routing tables, and a number of network interface (network interface controller or software-defined network interface) and network protocol statistics. It is available on Unix-like operating systems including macOS, Linux, Solaris, and BSD, and is available on Windows NT-based operating systems including Windows XP, Windows Vista, Windows 7, Windows 8 and Windows 10.


## posix信号处理

信号就是告知某个进程发生了某个事情的通知，有时也称为软件中断。信号通常是异步发生的，也就是说进程预先并不知道信号的准确发生时刻。

信号可以：
* 由进程发给另一个进程（或自身）
* 由内核发给某个进程

SIGHLD信号就是内核在任何一个进程终止时发给他的父进程的一个信号

有两个信号不能被捕获，他们是SIGKILL和SIGSTOP：他们向内核和超级用户提供了使进程终止或停止的可靠方法。

调用POSIX sigaction函数的signal函数

```c
#include "unp.h"

Sigfunc * signal(int signo,Sigfunc *func)
{
	struct sigaction act,oact;

	act.sa_handler=func;
	sigemptyset(&act.sa_mask);
	act.sa_flags=0;

	//省略平台可移植的处理

	if (sigaction(signo,&act,&oact)<0)
	return (SIG_ERR);
	return (oact.sa_handler);

}
```

用typedef简化函数原型

signal函数原型
```c
void (* signal(int signo,void (* func)(int))) (int);
```
为了简化，在unp.h中定义了如下Sigfunc类型：
```
typedef void Sigfunc(int)
```

### POSIX 信号语义

符合POSIX的系统上的信号处理：
* 一旦安装了信号处理函数，他便一直安装着
* 在一个信号处理函数运行期间，正在提交的信号是阻塞的。而且，安装处理函数时在传递给sigaction的函数的sa_mask信号集中指定的任何额外信号也被阻塞
* 如果一个信号在被阻塞间产生了一次或多次，那么该信号被解阻塞之后通常只递交一次，也就是说unix信号默认是不排队的
* 利用sigprocmask函数选择性地阻塞或解阻塞一组信号是可能的。这使得我们可以做到在一段临界区代码执行期间，防止捕获某些消息，以此保护这段代码










[上一级](base.md)
[上一篇](4_basic_tcp_socket.md)
