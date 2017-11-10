# I/O多路复用：select和poll函数


<!-- @import "[TOC]" {cmd="toc" depthFrom=1 depthTo=6 orderedList=false} -->
<!-- code_chunk_output -->

* [I/O多路复用：select和poll函数](#io多路复用select和poll函数)
	* [概述](#概述)
	* [I/O模型](#io模型)
		* [阻塞式I/O](#阻塞式io)
		* [非阻塞式I/O](#非阻塞式io)
		* [I/O复用](#io复用)
		* [信号驱动I/O模型](#信号驱动io模型)
		* [异步I/O模型](#异步io模型)
		* [同步I/O和异步I/O对比](#同步io和异步io对比)
	* [select 函数](#select-函数)
		* [timeout参数：](#timeout参数)
		* [readset、writeset、exceptset参数](#readset-writeset-exceptset参数)
		* [maxfdp1参数](#maxfdp1参数)
		* [描述符就绪条件](#描述符就绪条件)
			* [套接字准备好读](#套接字准备好读)
			* [一个套接字准备好写](#一个套接字准备好写)
			* [错误就绪](#错误就绪)
	* [str_cli函数（修订版）](#str_cli函数修订版)
	* [批量输入](#批量输入)
	* [shutdown函数](#shutdown函数)
	* [str_cli函数（再修订版）](#str_cli函数再修订版)
	* [TCP回射服务器程序](#tcp回射服务器程序)
			* [拒绝服务型攻击](#拒绝服务型攻击)

<!-- /code_chunk_output -->

## 概述

在前一章的服务器进程终止一节中，当FIN到达套接字时，客户正阻塞在fgets调用上。客户实际上在应对两个描述符-套接字和用户输入。

这样的进程需要一种预先告知内核的能力，使得内核一旦发现进程指定的一个或多个I/O条件就绪（也就是说输入已准备好被读取，或者描述符已能承接更多的输出），就通知进程。这个能力称为I/O复用，是由select和poll等函数支持的。

I/O多路复用典型使用在下列网络应用场合：
* 当客户处理多个描述符（通常是交互式输入和网络套接字）时，必须使用I/O复用。这是我们早先讨论过的场合
* 一个客户同时处理多个套接字时可能的，不过比较少见。
* 如果一个TCP服务器既要处理监听套接字，又要处理已连接套接字，一般就要使用I/O 复用
* 如果一个服务器既要处理TCP，又要处理UDP，一般使用I/O复用
* 如果一个服务器要处理多个服务或者多个协议，一般就要使用I/O复用

注：I/O复用 并非只限于网络编程，许多重要的应用程序也需要使用这项技术

## I/O模型

### 阻塞式I/O
最流行的I/O模型是阻塞式I/O模型，该书到目前为止的所有例子都使用该模型。默认情况下，所有套接字都是阻塞的。

![](../images/6_select_poll_function_201711091308_1.png)

进程调用recvfrom，其系统调用直到数据报到达且被复制到应用进程的缓冲区中或者发生错误才返回。

### 非阻塞式I/O

进程把一个套接字设置成非阻塞是在通知内核；当所请求的I/O操作非得把本进程投入睡眠才能完成时，不要把本进程投入睡眠，而是返回一个错误。

![](../images/6_select_poll_function_201711091308_2.png)

### I/O复用

![](../images/6_select_poll_function_201711091308_3.png)

### 信号驱动I/O模型
我们也可以使用信号，让内核在描述符就绪时发送SIGIO信号通知我们

![](../images/6_select_poll_function_201711091308_4.png)

我们首先开启套接字的信号驱动式I/O功能（25.2节会讲解），并通过并通过sigaction系统调用安装一个信号处理函数。

### 异步I/O模型

![](../images/6_select_poll_function_201711091308_5.png)

这个模型与前一节介绍的信号驱动模型的主要区别在于：信号驱动式I/O是由内核通知我们何时可以启动一个I/O操作，而异步I/O操作是由内核通知我们I/O操作何时完成。

我们调用aio_read函数（posix异步I/O函数以aio_ 或 lio_ 开头），给内核传递描述符、缓冲区指针、缓冲区大小和文件偏移，并告诉内核当整个操作完成时如何通知我们。

### 同步I/O和异步I/O对比

POSIX把这两个术语定义如下：
* 同步I/O操作导致请求进程阻塞，直到I/O操作完成；
* 异步I/O操作不导致请求进程阻塞

![](../images/6_select_poll_function_201711091721_1.png)

根据上述定义，我们的前4种模型-阻塞式、非阻塞式、I/O复用和信号驱动式I/O模型都是同步I/O模型，因为其真正的I/O操作（recvfrom）将阻塞进程。只有异步I/O模型与POSIX定义的异步I/O相匹配。


## select 函数

该函数允许进程指示内核等待多个事件中的任何一个发生，并只在有一个或多个事件发生或经历一段指定的时间后才唤醒它。


```c
#include <sys/select.h>
#include <sys/time.h>

int select(int maxfdp1,fd_set *readset,fd_set *writeset,fd_set *exceptset,const struct timeval *timeout);
//返回：若有就绪描述符则为其数目，若超时则为0，若出错则为-1
```


参数介绍：
### timeout参数：
它告知内核等待所指定描述符中的任何一个就绪可花多少时间。其中timeval结构用于指定这段时间的秒数和微妙数。
```c
struct timeval{
  long tv_sec;
  long tv_usec;
};
```
这个参数有三种可能。
1. 永远等待下去：仅在有一个描述符准备号I/O才返回。
为此，我们把该参数设置为空指针。
2. 等待一段固定的时间：在有一个描述符准备好I/O时返回，但是不超过由该参数所指向的timeval结构中指定的时间。
3. 根本不等待：检查描述符后立即返回，这称为轮询。为此timeval中的相关值需设为0。

注意：timeval结构能够表达select不支持的值。

### readset、writeset、exceptset参数
* 中间的三个参数readset、writeset、exceptset指定我们让内核测试读、写和异常条件的描述符。目前支持的异常条件只有两个：
1. 某个套接字的带外数据的到达，将在第24章讲述
2. 某个已设置为分组模式的伪终端存在可从其主端读取的控制状态信息。该书不讨论伪终端。

fd_set为描述符集的数据类型。

```c
void FD_ZERO(fd_set *fdset); //clear all bits in fdset;
void FD_SET(int fd,fd_set * fdset); //turn on the bit for fd in fdset
void FD_CLR(int fd,fd_set * fdset); //turn off the bit for fd in fdset
int FD_ISSET(int fd,fd_set *fdset); //is the bit for fd on in fdset ?
```

描述符集的初始化非常重要，因为作为自动变量分配的一个描述符集如果没有个初始化，那么可能发生不可预期的后果。

这三个参数，如果我们对某一个的条件不感兴趣，就可以把它设为空指针。事实上，如果这三个指针均为空，我们就有了一个比unix的sleep函数更为精确的定时器。

select函数修改由指针readset、writeset和exceptset指向的描述符集，因而这三个参数都是值-结果参数。调用该函数时，我们指定所关心的描述符的值，该函数返回时，结果将指示那些描述符已就绪。该函数返回后，我们可以使用FD—ISSET来测试fd_set数据类型中的描述符。描述符集中的任何与未就绪描述符对应的位返回时均清成0。

<span style="color:red">为此，每次重新调用select函数时，我们都得再次把所有描述符集内所关心的位均置为1。</span>.

### maxfdp1参数

指定待测试的描述符个数，他的值时待测的最大描述符加1，描述符0，1，2...，一直到maxfdp1-1均被测试

<span style="color:red">使用select函数最常见的两个编程错误是：
1.忘了对最大描述符加1  
2.忘了描述符集是值-结果-参数。第二个错误导致调用select时，描述符集内我们认为是1的位却被置为0
</span>.

### 描述符就绪条件

#### 套接字准备好读
1. 该套接字接收缓冲区中的数据字节数大于等于套接字接收缓冲区低水位标记好的的当前大小。对于这样的套接字执行读操作不会阻塞并将返回一个大于0的值（也就是返回准备好读入的数据）。我们可以使用SO_RCVLOWAT套接字选项设置该套接字的低水位标记。对于TCP和UDP，其默认值为1。

2. 该连接的读半部关闭（也就是接收了FIN的TCP连接）。对这样的套接字的读操作不会阻塞并返回0（也就是EOF）。

3. 该套接字是一个监听套接字且已完成的连接数不为0.对这样的套接字的accept通常不会阻塞。

4. 其上有一个套接字错误待处理。对这样的套接字的读操作将不阻塞，并返回-1，同时把errno设置为确切的条件。这些待处理错误也可以通过指定SO_ERROR套接字选项调用getsockopt获取并清除。

#### 一个套接字准备好写

1. 该套接字发送缓冲区中的数据字节数大于等于套接字发送缓冲区低水位标记好的的当前大小。

2. 该连接的写半部关闭。对这样的套接字的写操作将产生SIGPIPE信号

3. 其上有一个套接字错误待处理。详情同上

#### 错误就绪
如果一个套接字存在带外数据或者仍处于带外标记，那么它有异常条件待处理

注意：
当某个套接字上发生错误时，他将由select标记为即可读，又可写。

汇总上述导致select返回某个套接字就绪的条件

| condition | readable?    | writeable?| exception? |
| :------------- | :------------- | :- |:- |
| 有数据可读       |  Y      | N|N |
|关闭连接的读一半|Y |N|N
|给监听套接口准备好新连接|Y|N|N
|有可用于写的空间|N|Y|N
|关闭连接的写一半|N|Y|N
|待处理错误|Y|Y|N
|TCP带外数据|N|N|Y


## str_cli函数（修订版）

我们可以使用select来处理之前讨论的服务器终止问题。

str_cli函数中select处理的各种条件：

![](../images/6_select_poll_function_201711101451_1.png)

客户套接字上的三个条件处理如下：
1. 如果对端TCP发送数据，那么该套接字变为可读，并且read返回一个大于0的值（即读入数据的字节数）。
2. 如果对端TCP发送一个FIN（对端进程终止），那么该套接字变为可读，并且read返回0（EOF）。
3. 如果对端TCP发送一个RST（对端主机崩溃并重新启动），那么该套接字变为可读，并且返回-1，而errno中还有确切的错误代码。

使用select的str_cli函数的实现：
```c
#include "unp.h"

void str_cli(FILE *fp,int sockfd)
{
	int maxfdp1;
	fd_set rset;
	char sendline[MAXLINE],recvline[MAXLINE];

	FD_ZERO(&rset);

	while (1) {
		FD_SET(fileno(fp),&rset);
		//int fileno(FILE *stream);
		//The function fileno() examines the  argument  stream  and  returns  
		// its integer file descriptor.

		FD_SET(sockfd,&rset);
		maxfdp1=max(fileno(fp),sockfd)+1;
		Select(maxfdp1,&rset,NULL,NULL,NULL);

		if (FD_ISSET(sockfd,&rset))
		{
			if (Readline(sockfd,recvline,MAXLINE)==0)
				err_quit("str_cli:server terminated prematurely");
			Fputs(recvline,stdout);
		}

		if (FD_ISSET(fileno(fp),&rset))
		{
			if (Fgets(sendline,MAXLINE,fp)==NULL)
				return;
			Writen(sockfd,sendline,strlen(sendline));
		}
	}
}
```

## 批量输入
不幸的是，我们str_cli函数仍然是不正确的。

一般来说，为提升性能而引入缓冲机制增加了网络应用程序的复杂性，上面章节的代码就遭受这种复杂性之害。究其原因在于select不知道stdio使用了缓冲区-它只是从read系统调用的角度指出是否有数据可读，而不是从fgets之类调用的角度考虑。基于上述原因，<span style="color:red">混合使用stdio和select被认为是非常容易犯错误的，在这样做时必须极其小心</span>              



## shutdown函数
终止网络连接的通常方法时调用close函数。不过close有两个限制，却可以使用shutdown来避免。
1. close把描述符的引用计数减一，仅在该计数变为0时才关闭套接字。使用shutdown可以不管引用计数就激发TCP的正常连接终止序列。
2. close终止读和写两个方向的数据传送。

```c
#include <sys/socket.h>

int shutdown(int sockfd,int howto);
```

该函数的行为依赖于howto参数的值。

* SHUT_RD 关闭连接的读这——半-套接字中不再有数据可接收，并且套接字接收缓冲区中的现有数据都被丢弃。进程不能在对这样的套接字调用任何读函数。对一个TCP套接字这样调用shutdown函数后，由该套接字接收的来自对端的任何数据都被确认，然后悄然丢弃。

* SHUT_WR 关闭连接的写这一半——对于TCP套接字，这称为半关闭。当前留在套接字发送缓冲区中的数据将被发送掉，后跟TCP的正常连接终止序列

* SHUT_RDWR 连接的读半部和写半部都关闭

这三个SHUT_*名字是由POSIX规范定义，howto参数的典型值将会是0（关闭读半部）、1（关闭写半部）、2（读半部和写半部都关闭）

## str_cli函数（再修订版）

str_cli函数的改进（且正确）版本。这个版本还废弃了以文本为中心的代码，改而针对缓冲区操作，从而消除了前面提到的复杂性问题。
```c
#include "unp.h"

void str_cli(FILE * fp,int sockfd)
{
	int maxfdp1,stdineof;
	fd_set rset;
	char buf[MAXLINE];
	int n;

	stdineof=0;
	FD_ZERO(&rset);
	while (1) {
		if (stdineof==0)
			FD_SET(fileno(fp),&rset);
			FD_SET(sockfd,&rset);
			maxfdp1=max(fileno(fp),sockfd)+1;
			Select(maxfdp1,&rset,NULL,NULL,NULL);

			if (FD_ISSET(sockfd,&rset))
			{
				if ((n=Read(sockfd,buf,MAXLINE))==0)
				{
					if (stdineof==1)
						return ;
					else
						err_quit("str_cli:server terminated prematurely");
				}
				Writen(fileno(stdout),buf,n);
			}

			if (FD_ISSET(fileno(fp),&rset))
			{
					//if stdin input EOF
					if ((n=Read(fileno(fp),buf,MAXLINE))==0)
					{
						stdineof=1;
						Shutdown(sockfd,SHUT_WR);//send FIN
						FD_CLR(fileno(fp),&rset);
						continue;
					}
					Writen(sockfd,buf,n);
			}
	}
}
```

## TCP回射服务器程序

```c
#include "unp.h"

int main(int argc, char const *argv[]) {
	int i,maxi,maxfd,listenfd,connfd,sockfd;

	int nready,client[FD_SETSIZE];

	ssize_t n;
	fd_set rset,allset;
	char buf[MAXLINE];
	socklen_t clilen;
	struct sockaddr_in cliaddr,servadd;
	listenfd=Socket(AF_INET,SOCK_STREAM,0);

	bzero(&serveraddr,sizeof(servadd));
	servaddr.sin_family=AF_INET;
	servadd.sin_port.s_addr=htonl(INADDR_ANY);
	servadd.sin_port=htons(SERV_PORT);

	Bind(listenfd,(SA *)&servadd,sizeof(servadd));

	Listen(listenfd,LISTENQ);

	maxfd=listenfd; //initialize
	maxi=-1; //index into  client[] array

	for (i=0;i<FD_SETSIZE;i++)
		client[i]=-1; // -1 indicates available entry

	FD_ZERO(&allset);
	FD_SET(listenfd,&allset);

	while (1) {
		rset=allset; //structure assignment
		nready=Select(maxfd+1,&rset,NULL,NULL,NULL);

		if(FD_ISSET(listenfd,&rset)) //new client connection
		{
			clilen=sizeof(cliaddr);
			connfd=Accept(listenfd,(SA *)&cliaddr,&clilen);

			for (i=0;i<FD_SETSIZE;i++)
			{
				if (client[i]<0)
				{
					client[i]=connfd; //save descriptor
					break;
				}
			}

			if (i==FD_SETSIZE)
				err_quit("too many clients");

			FD_SET(connfd,&allset); //add new descriptor to set

			if (connfd > maxfd)
				maxfd=connfd; //for select

			if (i>maxi)
				maxi=i; //max index in client[] array

			if (--nready<=0)
			continue; //no more readable descriptor
		}

		for(i=0;i<=maxi;i++) //check all client for data
		{
			if ((sockfd=client[i])<0)
				continue;
			if (FD_ISSET(sockfd,&rset))
			{
				if ((n=Read(sockfd,buf,MAXLINE))==0)
				{
					Close(sockfd);
					FD_CLR(sockfd,&allset);
					client[i]=-1;
				}
				else
					Writen(sockfd,buf,n);
				if (--nready<=0)
					break; //no more readable descriptor
			}
		}
	}
	return 0;
}
```

### 拒绝服务型攻击

[拒绝服务型攻击](https://en.wikipedia.org/wiki/Denial-of-service_attack)

[僵尸网络](https://en.wikipedia.org/wiki/Botnet)

不幸的是，我们刚刚给出的服务器程序存在一个问题。

当一个服务器再处理多个客户时，它绝对不能阻塞于只与单个客户相关的某个函数调用。否则可能导致服务器被挂起，拒绝为所有的其他客户提供服务。这就是所谓的拒绝服务型攻击。它就是针对服务器做些动作，导致服务器不再能位其他合法客户提供服务。

可能的解决办法包括：
1. 使用非阻塞式I/O
2. 让每个客户由单独的控制线程提供服务
3. I/O操作设置一个超时




[上一级](base.md)
[上一篇](5_tcp_cli_srv_example.md)
