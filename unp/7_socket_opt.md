# 7.套接字选项


<!-- @import "[TOC]" {cmd="toc" depthFrom=1 depthTo=6 orderedList=false} -->
<!-- code_chunk_output -->

* [7.套接字选项](#7套接字选项)
	* [7.1 概述](#71-概述)
	* [7.2 getsockopt和setsockopt函数](#72-getsockopt和setsockopt函数)
	* [7.3 检查选项是否受支持并获取默认值](#73-检查选项是否受支持并获取默认值)
	* [7.4 套接字状态](#74-套接字状态)
	* [7.5 通用套接字选项](#75-通用套接字选项)
		* [7.5.1 SO_BROADCAST套接字选项](#751-so_broadcast套接字选项)
		* [7.5.2 SO_DEBUG套接字选项](#752-so_debug套接字选项)
		* [7.5.3 SO_DONTROUTE套接字选项](#753-so_dontroute套接字选项)
		* [7.5.4 SO_ERROR套接字选项](#754-so_error套接字选项)
		* [7.5.5 SO_KEEPALIVE套接字选项](#755-so_keepalive套接字选项)
		* [7.5.7 SO_OOBINLINE套接字选项](#757-so_oobinline套接字选项)
		* [7.5.8 SO_RCVBUF和SO_SNDBUF套接字选项](#758-so_rcvbuf和so_sndbuf套接字选项)
		* [7.5.9 SO_RCVLOWAT和SO_SNDLOWAT套接字选项](#759-so_rcvlowat和so_sndlowat套接字选项)
		* [SO_RCVTIMEO 和SO_SNDTIMEO套接字选项](#so_rcvtimeo-和so_sndtimeo套接字选项)

<!-- /code_chunk_output -->

## 7.1 概述

有很多方法来获取和设置影响套接字的选项：
* getsockopt和setsockopt函数
* fcntl函数
* ioctl函数（将在17章讨论）

## 7.2 getsockopt和setsockopt函数
这两个函数仅用于套接字
```c
#include <sys/socket.h>

int getsockopt(int sockfd,int level,int optname,void * optval,socklen_t *optlen);

int setsockopt(int sockfd,int level,int optname,const void *optval,socklen_t optlen);

//均返回：若成功则为0，若出错则为-1
```


* sockfd 必须指向一个打开的套接字描述符
* level指定系统中解释选项的代码或为通用套接字代码，或为某个特定于协议的代码（例如IPV4、IPV6、TCP或SCTP）
* optval是一个指向某个变量（* optval）的指针
* optlen指定*optval大小

下表汇总了可由getsockopt获取或由setsockopt设置的选项。其中的“数据类型”列出了指针optval必须指向的每个选项的数据类型。我们用一对花括号的计法表示结构体。

套接字选项粗分为两大基本类型：
1. 启用或禁止某个特性的二元选项（称为标志选项）
2. 取得并返回我们可以设置或检查的特定值的选项（称为值选项）

|level|optname|get|set|info|flag|data type|
|-|-|-|-|-|-|-|
|SOL_SOCKET|SO_BROADCAST|Y|Y|允许发送广播数据报|Y|int
|SOL_SOCKET|SO_DEBUG|Y|Y|开启调试跟踪|Y|int
|SOL_SOCKET|SO_DONTROUTE|Y|Y|绕过外出路由表查询|Y|int
|SOL_SOCKET|SO_ERROR|Y|N|获取待处理错误并清除|N|int
|SOL_SOCKET|SO_KEEPALIVE|Y|Y|周期性测试连接是否仍存活|Y|int
|SOL_SOCKET|SO_LINGER|Y|Y|若有数据待发送则延迟关闭|N|linger{}
|SOL_SOCKET|SO_OOBINLINE|Y|Y|让接收到的带外数据继续在线留存|Y|int
|SOL_SOCKET|SO_RCVBUF|Y|Y|接收缓冲区大小|N|int
|SOL_SOCKET|SO_SNDBUF|Y|Y|发送缓冲区大小|N|int
|SOL_SOCKET|SO_RCVLOWAT|Y|Y|接收缓冲区低水位标记|N|int
|SOL_SOCKET|SO_SNDLOWAT|Y|Y|发送缓冲区低水位标记|N|int
|SOL_SOCKET|SO_RCVTIMEO|Y|Y|接收超时|N|timeval{}
|SOL_SOCKET|SO_SNDTIMEO|Y|Y|发送超时|N|timeval{}
|SOL_SOCKET|SO_REUSEADDR|Y|Y|允许重用本地地址|Y|int
|SOL_SOCKET|SO_REUSEPORT|Y|Y|允许重用本地端口|Y|int
|SOL_SOCKET|SO_TYPE|Y|N|取得套接字类型|N|int
|SOL_SOCKET|SO_USELOOPBACK|Y|Y|路由套接字取得所发送数据的副本|Y|int
|IPPROTO_IP|IP_HDRINCL |Y|Y|随数据包含ip首部|Y|int
|IPPROTO_IP|IP_OPTIONS |Y|Y|IP首部选项|N|
|IPPROTO_IP|IP_RECVDSTADDR |Y|Y|返回目的IP地址|Y|int
|IPPROTO_IP|IP_RECVIF |Y|Y|返回接收接口索引|Y|int
|IPPROTO_IP|IP_TOS |Y|Y|服务器类型和优先权|N|int
|IPPROTO_IP|IP_TTL |Y|Y|存活时间|N|int
|IPPROTO_IP|IP_MULTICAST_IF |Y|Y|指定外出接口|N|in_addr{}
|IPPROTO_IP|IP_MULTICAST_TTL |Y|Y|指定外出TTL|N|U_char
|IPPROTO_IP|IP_MULTICAST_LOOP |Y|Y|指定是否环回|N|u_char
|IPPROTO_IP|IP_ADD_MEMBERSHIP |N|Y|加入多播组|N|ip_mreq{}
|IPPROTO_IP|IP_DROP_MEMEBERSHIP |N|Y|离开多播组|N|ip_mreq{}
|IPPROTO_IP|IP_BLOCK_SOURCE |N|Y|阻塞多播组|N|ip_mreq_source{}
|IPPROTO_IP|IP_UNBLOCK_SOURCE |N|Y|开通多播组|N|ip_mreq_source{}
|IPPROTO_IP|IP_ADD_SOURCE_MEMBERSHIP |N|Y|加入源特定多播组|N|ip_mreq_source{}
|IPPROTO_IP|IP_DROP_SOURCE_MEMEBERSHIP |N|Y|离开源特定多播组|N|ip_mreq_source{}
|IPPROTO_ICMPV6|ICMP6_FILTER|Y|Y|指定待传递的ICMPV6消息类型|N|icmp6_filter{}
|IPPROTO_IPV6|IPV6_CHECKSUM|Y|Y|用于原始套接字的检验和字段偏移|N|int
|IPPROTO_IPV6|IPV6_DONTFRAG|Y|Y|丢弃大的分组而非将其分片|Y|int
|IPPROTO_IPV6|IPV6_NEXTHOP|Y|Y|指定下一跳地址|N|sock_addrin6{}
|IPPROTO_IPV6|IPV6_PATHMTU|Y|N|获取当前路径MTU|N|ip6_mtuinfo{}
|IPPROTO_IPV6|IPV6_RECVDSTOPTS|Y|Y|接收目的地选项|Y|int
|IPPROTO_IPV6|IPV6_RECVHOPLIMIT|Y|Y|接收单播跳限|Y|int
|IPPROTO_IPV6|IPV6_RECVHOPOPTS|Y|Y|接收步跳选项|Y|int
|IPPROTO_IPV6|IPV6_RECVPATHMTU|Y|Y|接收路径MTU|Y|int
|IPPROTO_IPV6|IPV6_RECVPKTINFO|Y|Y|接收分组信息|Y|int
|IPPROTO_IPV6|IPV6_RECVRTHDR|Y|Y|接收源路径|Y|int
|IPPROTO_IPV6|IPV6_RECVTCLASS|Y|Y|接收流通类别|Y|int
|IPPROTO_IPV6|IPV6_UNICAST_HOPS|Y|Y|默认单播跳限|N|int
|IPPROTO_IPV6|IPV6_USE_MIN_MTU|Y|Y|使用最小MTU|Y|int
|IPPROTO_IPV6|IPV6_V6ONLY|Y|Y|禁止V4兼容|Y|int
|IPPROTO_IPV6|IPV6_XXX|Y|Y|粘附性辅助数据|N|
|IPPROTO_IPV6|IPV6_MULTICAST_IF|Y|Y|指定外出接口|N|u_int
|IPPROTO_IPV6|IPV6_MULTICAST_HOPS|Y|Y|指定外出跳限|N|int
|IPPROTO_IPV6|IPV6_MULTICAST_LOOP|Y|Y|指定是否环回|Y|u_int
|IPPROTO_IPV6|IPV6_JOIN_GROUP|N|Y|加入多播组|N|ipv6_mreq{}
|IPPROTO_IPV6|IPV6_LEAVE_GROUP|N|Y|离开多播组|N|ipv6_mreq{}
|IPPROTO_IP( V4 \| V6)|MCAST_JOIN_GROUP|N|Y|加入多播组|N|group_req{}
|IPPROTO_IP( V4 \| V6)|MCAST_LEAVE_GROUP|N|Y|离开多播组 |N|group_source_req{}
|IPPROTO_IP( V4 \| V6)|MCAST_BLOCK_SOURCE|N|Y|阻塞多播源 |N|group_source_req{}
|IPPROTO_IP( V4 \| V6)|MCAST_UNBLOCK_SOURCE|N|Y|开通多播源 |N|group_source_req{}
|IPPROTO_IP( V4 \| V6)|MCAST_JOIN_SOURCE_GROUP|N|Y|加入源特定多播组 |N|group_source_req{}
|IPPROTO_IP( V4 \| V6)|MCAST_LEAVE_SOURCE_GROUP|N|Y|离开源特定多播组 |N|group_source_req{}    
|IPPROTO_TCP|TCP_MAXSEG|Y|Y|TCP最大分节大小|N|int
|IPPROTO_TCP|TCP_NODELAY|Y|Y|禁止Nagle算法|Y|int

注：暂时省略IPPROTO_SCTP (level)

## 7.3 检查选项是否受支持并获取默认值

```c
/* include checkopts1 */
/* *INDENT-OFF* */
#include "unp.h"
#include <netinet/tcp.h> /* for TCP_xxx defines */
#include <sys/socket.h>

union val {
  int i_val;
  long l_val;
  struct linger linger_val;
  struct timeval timeval_val;
} val;

static char *sock_str_flag(union val *, int);
static char *sock_str_int(union val *, int);
static char *sock_str_linger(union val *, int);
static char *sock_str_timeval(union val *, int);

struct sock_opts {
  const char * opt_str;
  int opt_level;
  int opt_name;
  char * (* opt_val_str)(union val * , int);
} sock_opts[] = {
    {"SO_BROADCAST", SOL_SOCKET, SO_BROADCAST, sock_str_flag},
    {"SO_DEBUG", SOL_SOCKET, SO_DEBUG, sock_str_flag},
    {"SO_DONTROUTE", SOL_SOCKET, SO_DONTROUTE, sock_str_flag},
    {"SO_ERROR", SOL_SOCKET, SO_ERROR, sock_str_int},
    {"SO_KEEPALIVE", SOL_SOCKET, SO_KEEPALIVE, sock_str_flag},
    {"SO_LINGER", SOL_SOCKET, SO_LINGER, sock_str_linger},
    {"SO_OOBINLINE", SOL_SOCKET, SO_OOBINLINE, sock_str_flag},
    {"SO_RCVBUF", SOL_SOCKET, SO_RCVBUF, sock_str_int},
    {"SO_SNDBUF", SOL_SOCKET, SO_SNDBUF, sock_str_int},
    {"SO_RCVLOWAT", SOL_SOCKET, SO_RCVLOWAT, sock_str_int},
    {"SO_SNDLOWAT", SOL_SOCKET, SO_SNDLOWAT, sock_str_int},
    {"SO_RCVTIMEO", SOL_SOCKET, SO_RCVTIMEO, sock_str_timeval},
    {"SO_SNDTIMEO", SOL_SOCKET, SO_SNDTIMEO, sock_str_timeval},
    {"SO_REUSEADDR", SOL_SOCKET, SO_REUSEADDR, sock_str_flag},
#ifdef SO_REUSEPORT
    {"SO_REUSEPORT", SOL_SOCKET, SO_REUSEPORT, sock_str_flag},
#else
    {"SO_REUSEPORT", 0, 0, NULL},
#endif
    {"SO_TYPE", SOL_SOCKET, SO_TYPE, sock_str_int},

    //{"SO_USELOOPBACK", SOL_SOCKET, SO_USELOOPBACK,

		//fedora 26，gcc 7.2.1,show error sock_str_flag},
	  // error: ‘SO_USELOOPBACK’ undeclared here 
	  // (not in a function); did you mean ‘\__USE_XOPEN2K’?

    {"IP_TOS", IPPROTO_IP, IP_TOS, sock_str_int},
    {"IP_TTL", IPPROTO_IP, IP_TTL, sock_str_int},
#ifdef IPV6_DONTFRAG
    {"IPV6_DONTFRAG", IPPROTO_IPV6, IPV6_DONTFRAG, sock_str_flag},
#else
    {"IPV6_DONTFRAG", 0, 0, NULL},
#endif
#ifdef IPV6_UNICAST_HOPS
    {"IPV6_UNICAST_HOPS", IPPROTO_IPV6, IPV6_UNICAST_HOPS, sock_str_int},
#else
    {"IPV6_UNICAST_HOPS", 0, 0, NULL},
#endif
#ifdef IPV6_V6ONLY
    {"IPV6_V6ONLY", IPPROTO_IPV6, IPV6_V6ONLY, sock_str_flag},
#else
    {"IPV6_V6ONLY", 0, 0, NULL},
#endif
    {"TCP_MAXSEG", IPPROTO_TCP, TCP_MAXSEG, sock_str_int},
    {"TCP_NODELAY", IPPROTO_TCP, TCP_NODELAY, sock_str_flag},
#ifdef SCTP_AUTOCLOSE
    {"SCTP_AUTOCLOSE", IPPROTO_SCTP, SCTP_AUTOCLOSE, sock_str_int},
#else
    {"SCTP_AUTOCLOSE", 0, 0, NULL},
#endif
#ifdef SCTP_MAXBURST
    {"SCTP_MAXBURST", IPPROTO_SCTP, SCTP_MAXBURST, sock_str_int},
#else
    {"SCTP_MAXBURST", 0, 0, NULL},
#endif
#ifdef SCTP_MAXSEG
    {"SCTP_MAXSEG", IPPROTO_SCTP, SCTP_MAXSEG, sock_str_int},
#else
    {"SCTP_MAXSEG", 0, 0, NULL},
#endif
#ifdef SCTP_NODELAY
    {"SCTP_NODELAY", IPPROTO_SCTP, SCTP_NODELAY, sock_str_flag},
#else
    {"SCTP_NODELAY", 0, 0, NULL},
#endif
    {NULL, 0, 0, NULL}};
/* *INDENT-ON* */
/* end checkopts1 */

/* include checkopts2 */
int main(int argc, char **argv) {
  int fd;
  socklen_t len;
  struct sock_opts * ptr;

  for (ptr = sock_opts; ptr->opt_str != NULL; ptr++) {
    printf("%s: ", ptr->opt_str);
    if (ptr->opt_val_str == NULL)
      printf("(undefined)\n");
    else {
      switch (ptr->opt_level) {
      case SOL_SOCKET:
      case IPPROTO_IP:
      case IPPROTO_TCP:
        fd = Socket(AF_INET, SOCK_STREAM, 0);
        break;
#ifdef IPV6
      case IPPROTO_IPV6:
        fd = Socket(AF_INET6, SOCK_STREAM, 0);
        break;
#endif
#ifdef IPPROTO_SCTP
      case IPPROTO_SCTP:
        fd = Socket(AF_INET, SOCK_SEQPACKET, IPPROTO_SCTP);
        break;
#endif
      default:
        err_quit("Can't create fd for level %d\n", ptr->opt_level);
      }

      len = sizeof(val);
      if (getsockopt(fd, ptr->opt_level, ptr->opt_name, &val, &len) == -1) {
        err_ret("getsockopt error");
      } else {
        printf("default = %s\n", (* ptr->opt_val_str)(&val, len));
      }
      close(fd);
    }
  }
  exit(0);
}
/* end checkopts2 */

/* include checkopts3 */
static char strres[128];

static char * sock_str_flag(union val *ptr, int len) {
  /* * INDENT-OFF* * /
  if (len != sizeof(int))
    snprintf(strres, sizeof(strres), "size (%d) not sizeof(int)", len);
  else
    snprintf(strres, sizeof(strres), "%s", (ptr->i_val == 0) ? "off" : "on");
  return (strres);
  /* *INDENT-ON* * /
}
/* end checkopts3 */

static char *sock_str_int(union val *ptr, int len) {
  if (len != sizeof(int))
    snprintf(strres, sizeof(strres), "size (%d) not sizeof(int)", len);
  else
    snprintf(strres, sizeof(strres), "%d", ptr->i_val);
  return (strres);
}

static char *sock_str_linger(union val *ptr, int len) {
  struct linger * lptr = &ptr->linger_val;

  if (len != sizeof(struct linger))
    snprintf(strres, sizeof(strres), "size (%d) not sizeof(struct linger)",
             len);
  else
    snprintf(strres, sizeof(strres), "l_onoff = %d, l_linger = %d",
             lptr->l_onoff, lptr->l_linger);
  return (strres);
}

static char *sock_str_timeval(union val *ptr, int len) {
  struct timeval * tvptr = &ptr->timeval_val;

  if (len != sizeof(struct timeval))
    snprintf(strres, sizeof(strres), "size (%d) not sizeof(struct timeval)",
             len);
  else
    snprintf(strres, sizeof(strres), "%d sec, %d usec", tvptr->tv_sec,
             tvptr->tv_usec);
  return (strres);
}

```

fedora 26 ,show :
```
SO_BROADCAST: default = off
SO_DEBUG: default = off
SO_DONTROUTE: default = off
SO_ERROR: default = 0
SO_KEEPALIVE: default = off
SO_LINGER: default = l_onoff = 0, l_linger = 0
SO_OOBINLINE: default = off
SO_RCVBUF: default = 87380
SO_SNDBUF: default = 16384
SO_RCVLOWAT: default = 1
SO_SNDLOWAT: default = 1
SO_RCVTIMEO: default = 0 sec, 0 usec
SO_SNDTIMEO: default = 0 sec, 0 usec
SO_REUSEADDR: default = off
SO_REUSEPORT: default = off
SO_TYPE: default = 1
IP_TOS: default = 0
IP_TTL: default = 64
IPV6_DONTFRAG: default = off
IPV6_UNICAST_HOPS: default = 64
IPV6_V6ONLY: default = off
TCP_MAXSEG: default = 536
TCP_NODELAY: default = off
SCTP_AUTOCLOSE: (undefined)
SCTP_MAXBURST: (undefined)
SCTP_MAXSEG: (undefined)
SCTP_NODELAY: (undefined)
```
SO_TYPE选项的返回值1对应于该实现的SOCK_STREAM

## 7.4 套接字状态

下面的套接字选项是由TCP已连接套接字从监听套接字继承来的：SO_DEBUG,SO_DONTROUTE,SO_KEEPALIVE,SO_LINGER,SO_OOBINLINE,SO_RCVBUF,SO_RCVLOWAT,SO_SNDBUF,SO_SNDLOWAT,TCP_MAXSEG,TCP_NODELAY。这对TCP来说是很重要的，因为accept一直要到TCP层完成三路握手后才会给服务器返回已连接套接字。如果想在三路握手完成时确保这些套接字选项中的某一个时给已连接套接字设置的，那么我们必须先给监听套接字设置该选项。

## 7.5 通用套接字选项

我们先从通用套接字选项开始讨论。

### 7.5.1 SO_BROADCAST套接字选项
该选项开启或者禁止进程发送广播消息的能力。只有数据报套接字支持广播，并且还必须是在支持广播详细的网络上（例如以太网、令牌环网等）。我们不可能再点对点链路上进行广播，也不可能在基于连接的传输协议（例如TCP、SCTP）之上进行广播。
由于应用程序在发送之前必须设置该套接字选项，因此它能够有效地防止一个进程再其应用程序根本没有设计成可广播时就发送广播数据报。

### 7.5.2 SO_DEBUG套接字选项
该选项仅由TCP支持。当给一个TCP套接字开启该选项时，内核将为TCP在该套接字发送和接收的所有分组保留详细跟踪信息。这些信息保存在内核的某个 [ 环形缓冲区](https://zh.wikipedia.org/wiki/%E7%92%B0%E5%BD%A2%E7%B7%A9%E8%A1%9D%E5%8D%80) 中，并可使用trpt程序进行检查。

### 7.5.3 SO_DONTROUTE套接字选项
该选项规定外出分组将绕过底层协议的正常路由机制。
路由守护进程（routed和gated）经常使用该选项来绕过路由表（路由表不正确的情况下），以强制将分组从特定的接口送出。

### 7.5.4 SO_ERROR套接字选项
当一个套接字发生错误时，源自Berkeley的内核中的协议模块将该套接字的名为so_error变量设置为标准的UNIX_Exxx值中的一个，我们称它为该套接字的待处理错误（pending error）。内核能够使用下面两种方式之一立即通知进程这个错误：
1. 如果进程阻塞在select调用时，那么无论是检查可读条件还是可写条件，select均返回并设置其中一个或所有两个条件。
2. 如果进程使用信号驱动I/O模型，那就给进程或者进程组产生一个SIGIO信号。

### 7.5.5 SO_KEEPALIVE套接字选项
给一个TCP套接字设置保持存活（keep-alive）选项后，如果2小时内在该套接字的任一方向上都没有数据交换，TCP就自动给对端发送一个保持存活探测分节（keep-alive probe）。这是一个对端必须响应的TCP分节，它将会导致以下三种情况之一:
1. 对端以期望的ACK响应
2. 对端以RST响应，它告知本端TCP：对端已崩溃且已重新启动。该套接字的待处理错误被置为ECONNRESET，套接字本身则关闭
3. 对端对保持存活探测分节没有任何响应。源自源自Berkley的TCP将另外发送8个探测分节，两两相隔75秒，试图得到一个响应。TCP在发出第一个探测分节后11分15秒内若没有得到任何响应则放弃

该选项的功用是检测对端主机是否崩溃或变得不可达

该选项一般由服务器使用，不过客户也可以使用。服务器使用该选项是因为他们花大部分时间阻塞在等待穿越TCP连接的输入上，也就是说在等待客户的请求。
然而如果客户主机连接掉线、电源掉电或系统崩溃，服务其进程将永远不会知道，并将继续等待永远不会到达的输入。我们称这种情况为半开连接（half-open connection ）。保持存活选项将检测出这些半开连接。

### 7.5.7 SO_OOBINLINE套接字选项
当该选项开启时，带外数据将被留在正常的输入队列中（即在线留存）。这种情况下接收函数的MSG_OOB标志不能用来读带外数据。

### 7.5.8 SO_RCVBUF和SO_SNDBUF套接字选项
每个套接字都有一个发送缓冲区和一个接收缓冲区。

对于TCP来说，套接字接收缓冲区中可用空间的大小限定了TCP通告对端窗口大小。TCP套接字接收缓冲区不可能溢出，因为不允许对端发出超过本端所通告窗口大小的数据。这就是TCP的流量控制，如果对端无视窗口大小而发出了超过该窗口大小的数据，本端TCP将丢弃它们。然而对于UDP来说，当接收到的数据报装不进套接字缓冲区时，该数据报就被丢弃。回顾一下，UDP是没有流量控制的：较快的发送端可以很容易的淹没较慢的接收端，导致接收端的UDP丢弃数据报。事实上较快的发送端甚至可以淹没本机的网络接口，导致数据报被本机丢弃。

当设置TCP套接字接收缓冲区的大小时，函数调用的顺序很重要。这是因为TCP的窗口规模选项是在建立连接时用SYN分节与对端互换得到的。

TCP套接字缓冲区的大小至少应该时相应连接的MSS的四倍

为避免潜在的缓冲区空间浪费，TCP套接字缓冲区大小还必须时相应连接的MSS值的偶数倍

### 7.5.9 SO_RCVLOWAT和SO_SNDLOWAT套接字选项
每个套接字都有一个接收低水位标记和一个发送低水位标记。他们由select函数使用，这两个套接字选项允许我们修改这两个低水位标记。

UDP并没有发送缓冲区，而只有发送缓冲区大小这个属性

### SO_RCVTIMEO 和SO_SNDTIMEO套接字选项
这两个选项允许我们给套接字的接收和发送设置一个超时时间。注意，访问他们的getsockopt和setsockopt的参数是指向timeval结构的指针。这个以让我们用秒数和微妙数来规定超时。我们通过设置其值为0s和0us来禁止超时。默认情况下这两个超时都是禁止的。

接收超时影响5个输入函数：read、readv、recv、recvfrom和recvmsg。
发送超时影响5个输出函数：write、writev、send、sendto和sendmsg。
[上一级](base.md)
[上一篇](6_select_poll_function.md)
