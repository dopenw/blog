# 基本UDP套接字编程


<!-- @import "[TOC]" {cmd="toc" depthFrom=1 depthTo=6 orderedList=false} -->
<!-- code_chunk_output -->

* [基本UDP套接字编程](#基本udp套接字编程)
	* [概述](#概述)
	* [recvfrom和sendto函数](#recvfrom和sendto函数)

<!-- /code_chunk_output -->


## 概述
UDP是无连接不可靠的数据报协议，非常不同于TCP提供的面向连接的可靠字节流。然而相比TCP，有些场合确实更适合使用UDP，使用UDP编写的一些常见的应用程序有：DNS、NFS、SNMP（简单网络管理协议）

典型的UDP客户/服务器程序的函数调用：
![](../images/8_basic_udp_socket_201711162242_1.png)

## recvfrom和sendto函数
这两个函数类似于标准的read和write函数，不过需要三个额外的参数。
```c
#include <sys/socket.h>

ssize_t recvfrom(int sockfd,void * buff,size_t nytes,int flags,struct sockaddr * from,socklen_t *addrlen);

ssize_t sendto(int sockfd,const void *buff,size_t nbytes,int flags,const struct sockaddr *to,socklen_t addrlen);
//均返回：若成功则为读或写的字节数，若出错则为-1
```
注意，sendto的最后一个参数是一个整数值，而recvfrom的最后一个参数是一个指向整数值的指针（值-结果）。

recvfrom的最后两个参数类似于accept的最后两个参数：返回时其中套接字地址结构的内容告诉我们是谁发送了数据报（UDP情况下）或是谁发起了连接（TCP情况下）。sendto的最后两个参数类似于connect的最后两个参数：调用时其中套接字地址结构被我们填入数据报将发往（UDP情况下）或与之建立连接（TCP情况下）的协议地址。

写一个长度为0的数据报是可行的。recvfrom返回0值是可接受的。

如果recvfrom的from参数是一个空指针，那么相应的长度参数也必须是一个空指针，表示我们并不关心数据发送者的协议地址。
recvfrom和sendto都可以用于TCP，尽管通常没有理由这么做。
[上一级](base.md)
[上一篇](7_socket_opt.md)
