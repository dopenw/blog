# 7.套接字选项


<!-- @import "[TOC]" {cmd="toc" depthFrom=1 depthTo=6 orderedList=false} -->
<!-- code_chunk_output -->

* [7.套接字选项](#7套接字选项)
	* [概述](#概述)
	* [getsockopt和setsockopt函数](#getsockopt和setsockopt函数)
	* [检查选项是否受支持并获取默认值](#检查选项是否受支持并获取默认值)
	* [套接字状态](#套接字状态)
	* [通用套接字选项](#通用套接字选项)
		* [SO_BROADCAST套接字选项](#so_broadcast套接字选项)
		* [SO_DEBUG套接字选项](#so_debug套接字选项)
		* [SO_DONTROUTE套接字选项](#so_dontroute套接字选项)
		* [SO_ERROR套接字选项](#so_error套接字选项)
		* [SO_KEEPALIVE套接字选项](#so_keepalive套接字选项)
		* [SO_LINGER套接字选项](#so_linger套接字选项)
		* [SO_OOBINLINE套接字选项](#so_oobinline套接字选项)
		* [SO_RCVBUF和SO_SNDBUF套接字选项](#so_rcvbuf和so_sndbuf套接字选项)
		* [SO_RCVLOWAT和SO_SNDLOWAT套接字选项](#so_rcvlowat和so_sndlowat套接字选项)
		* [SO_RCVTIMEO 和SO_SNDTIMEO套接字选项](#so_rcvtimeo-和so_sndtimeo套接字选项)
		* [SO_REUSEADDR和SO_REUSEPORT套接子选项](#so_reuseaddr和so_reuseport套接子选项)
			* [SO_REUSEADDR](#so_reuseaddr)
			* [SO_REUSEPORT](#so_reuseport)
		* [SO_TYPE套接字选项](#so_type套接字选项)
		* [SO_USELOOPBACK套接字选项](#so_useloopback套接字选项)
	* [TCP套接字选项](#tcp套接字选项)
		* [TCP_MAXSEG套接字选项](#tcp_maxseg套接字选项)
		* [TCP_NODELAY套接字选项](#tcp_nodelay套接字选项)
	* [fcntl函数](#fcntl函数)
	* [部分习题解答](#部分习题解答)

<!-- /code_chunk_output -->

## 概述

有很多方法来获取和设置影响套接字的选项：
* getsockopt和setsockopt函数
* fcntl函数
* ioctl函数（将在17章讨论）

## getsockopt和setsockopt函数
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

## 检查选项是否受支持并获取默认值

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
```sh
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

## 套接字状态

下面的套接字选项是由TCP已连接套接字从监听套接字继承来的：SO_DEBUG,SO_DONTROUTE,SO_KEEPALIVE,SO_LINGER,SO_OOBINLINE,SO_RCVBUF,SO_RCVLOWAT,SO_SNDBUF,SO_SNDLOWAT,TCP_MAXSEG,TCP_NODELAY。这对TCP来说是很重要的，因为accept一直要到TCP层完成三路握手后才会给服务器返回已连接套接字。如果想在三路握手完成时确保这些套接字选项中的某一个时给已连接套接字设置的，那么我们必须先给监听套接字设置该选项。

## 通用套接字选项

我们先从通用套接字选项开始讨论。

### SO_BROADCAST套接字选项
该选项开启或者禁止进程发送广播消息的能力。只有数据报套接字支持广播，并且还必须是在支持广播详细的网络上（例如以太网、令牌环网等）。我们不可能再点对点链路上进行广播，也不可能在基于连接的传输协议（例如TCP、SCTP）之上进行广播。
由于应用程序在发送之前必须设置该套接字选项，因此它能够有效地防止一个进程再其应用程序根本没有设计成可广播时就发送广播数据报。

### SO_DEBUG套接字选项
该选项仅由TCP支持。当给一个TCP套接字开启该选项时，内核将为TCP在该套接字发送和接收的所有分组保留详细跟踪信息。这些信息保存在内核的某个 [ 环形缓冲区](https://zh.wikipedia.org/wiki/%E7%92%B0%E5%BD%A2%E7%B7%A9%E8%A1%9D%E5%8D%80) 中，并可使用trpt程序进行检查。

### SO_DONTROUTE套接字选项
该选项规定外出分组将绕过底层协议的正常路由机制。
路由守护进程（routed和gated）经常使用该选项来绕过路由表（路由表不正确的情况下），以强制将分组从特定的接口送出。

### SO_ERROR套接字选项
当一个套接字发生错误时，源自Berkeley的内核中的协议模块将该套接字的名为so_error变量设置为标准的UNIX_Exxx值中的一个，我们称它为该套接字的待处理错误（pending error）。内核能够使用下面两种方式之一立即通知进程这个错误：
1. 如果进程阻塞在select调用时，那么无论是检查可读条件还是可写条件，select均返回并设置其中一个或所有两个条件。
2. 如果进程使用信号驱动I/O模型，那就给进程或者进程组产生一个SIGIO信号。

### SO_KEEPALIVE套接字选项
给一个TCP套接字设置保持存活（keep-alive）选项后，如果2小时内在该套接字的任一方向上都没有数据交换，TCP就自动给对端发送一个保持存活探测分节（keep-alive probe）。这是一个对端必须响应的TCP分节，它将会导致以下三种情况之一:
1. 对端以期望的ACK响应
2. 对端以RST响应，它告知本端TCP：对端已崩溃且已重新启动。该套接字的待处理错误被置为ECONNRESET，套接字本身则关闭
3. 对端对保持存活探测分节没有任何响应。源自源自Berkley的TCP将另外发送8个探测分节，两两相隔75秒，试图得到一个响应。TCP在发出第一个探测分节后11分15秒内若没有得到任何响应则放弃

该选项的功用是检测对端主机是否崩溃或变得不可达

该选项一般由服务器使用，不过客户也可以使用。服务器使用该选项是因为他们花大部分时间阻塞在等待穿越TCP连接的输入上，也就是说在等待客户的请求。
然而如果客户主机连接掉线、电源掉电或系统崩溃，服务其进程将永远不会知道，并将继续等待永远不会到达的输入。我们称这种情况为半开连接（half-open connection ）。保持存活选项将检测出这些半开连接。

### SO_LINGER套接字选项
该选项指定close函数对面向连接协议如何操作。默认操作是close立即返回，但是如果有数据残留在套接字发送缓冲区中，系统将试着把这些数据发送给对端。

SO_LINGER套接字选项使得我们可以改变这个默认设置。

```c
struct linger{
	int l_onoff; //0=off,nonzero=on
	int l_linger; //linger time,posix specifies units as seconds
};
```
对setsockopt的调用将根据其中两个结构成员的值形成下列三种情况之一：
1. 如果l_onoff为0，那么关闭该选项。l_linger的值被忽略，先前讨论的TCP默认是遏制生效，即close立即返回
2. 如果l_onoff非0且l_linger为0，那么当close某个连接时TCP将中止该连接。这就是说TCP将丢弃保留在套接字发送缓冲区中的任何数据，并发送一个RST给对端，而没有通常的四分组连接终止序列
3. 如果l_onoff非0且l_linger非0，那么当套接字关闭时内核将拖延一段时间，这就是说如果在套接字发送缓冲区中仍残留由数据，那么进程将被投入睡眠，直到所有数据都已经发送完且均被对方确认或延滞时间到。

这里有一个基本原则：设置SO_LINGER套接字选项后，close的成功返回只是告诉我们先前发送的数据（和FIN）已由对端TCP确认，而不能告诉我们对端是否已对区数据。如果不设置该套接字选项，那么我们连对端TCP是否确认了数据都不知道。

### SO_OOBINLINE套接字选项
当该选项开启时，带外数据将被留在正常的输入队列中（即在线留存）。这种情况下接收函数的MSG_OOB标志不能用来读带外数据。

### SO_RCVBUF和SO_SNDBUF套接字选项
每个套接字都有一个发送缓冲区和一个接收缓冲区。

对于TCP来说，套接字接收缓冲区中可用空间的大小限定了TCP通告对端窗口大小。TCP套接字接收缓冲区不可能溢出，因为不允许对端发出超过本端所通告窗口大小的数据。这就是TCP的流量控制，如果对端无视窗口大小而发出了超过该窗口大小的数据，本端TCP将丢弃它们。然而对于UDP来说，当接收到的数据报装不进套接字缓冲区时，该数据报就被丢弃。回顾一下，UDP是没有流量控制的：较快的发送端可以很容易的淹没较慢的接收端，导致接收端的UDP丢弃数据报。事实上较快的发送端甚至可以淹没本机的网络接口，导致数据报被本机丢弃。

当设置TCP套接字接收缓冲区的大小时，函数调用的顺序很重要。这是因为TCP的窗口规模选项是在建立连接时用SYN分节与对端互换得到的。

TCP套接字缓冲区的大小至少应该时相应连接的MSS的四倍

为避免潜在的缓冲区空间浪费，TCP套接字缓冲区大小还必须时相应连接的MSS值的偶数倍

### SO_RCVLOWAT和SO_SNDLOWAT套接字选项
每个套接字都有一个接收低水位标记和一个发送低水位标记。他们由select函数使用，这两个套接字选项允许我们修改这两个低水位标记。

UDP并没有发送缓冲区，而只有发送缓冲区大小这个属性

### SO_RCVTIMEO 和SO_SNDTIMEO套接字选项
这两个选项允许我们给套接字的接收和发送设置一个超时时间。注意，访问他们的getsockopt和setsockopt的参数是指向timeval结构的指针。这个以让我们用秒数和微妙数来规定超时。我们通过设置其值为0s和0us来禁止超时。默认情况下这两个超时都是禁止的。

接收超时影响5个输入函数：read、readv、recv、recvfrom和recvmsg。
发送超时影响5个输出函数：write、writev、send、sendto和sendmsg。

### SO_REUSEADDR和SO_REUSEPORT套接子选项
#### SO_REUSEADDR
1. SO_REUSEADDR允许启动一个监听服务器并捆绑其众所周知端口，即使以前建立的将该端口用作他们的本地端口的连接仍存在。
2. SO_REUSEADDR允许在同一端口上启动同一服务器的多个实例，只要每个实例捆绑一个不同的本地IP地址即可。
对于TCP，我们绝对不可能启动捆绑相同IP地址和相同端口号的多个服务器：这是完全重复的捆绑。
3. SO_REUSEADDR允许单个进程捆绑同一端口到多个套接字上，只要每次捆绑指定不同的本地IP地址即可
4. SO_REUSEADDR允许完全重复的捆绑：当一个IP地址和端口已绑定到某个套接字上时，如果传输协议支持，同样的IP地址和端口还可以捆绑到另一个套接字上。一般来说，该特性仅支持UDP套接字

#### SO_REUSEPORT
4.4BSD随多播支持的添加引入了SO_REUSEPORT这个套接字选项。它并没有在SO_REUSEADDR上重载所需多播语义，而是引入了下面的语义：
1. 该选项允许完全重复的捆绑，不过只有在想要捆绑同一IP地址和端口的每个套接字都指定了本套接字选项才行
2. 如果被捆绑的IP地址时一个多播地址，那么SO_REUSEADDR SO_REUSEPORT被认为是等效的
注意：该套接字选项并非所有的系统都支持它。在那些不支持多播的系统上，我们改用SO_REUSEADDR以允许合理的完全重复的捆绑

我们以下面的建议来总结对这些套接字选项的讨论：
1. 在所有的TCP服务器程序中，在调用bind之前设置SO_REUSEADDR
2. 当编写一个可在同一时刻在同一主机上运行多次的多播应用程序时，设置SO_REUSEADDR套接字选项，并将所参见多播组的地址作为本地IP地址捆绑

### SO_TYPE套接字选项
该套接字返回套接字的类型，返回的整数值是一个诸如SOCK_STREAM或SOCK_DGRAM之类的值

### SO_USELOOPBACK套接字选项
该选项仅用于路由域（AF_ROUTE）的套接字。对于这些套接字，他的默认为开（这是一个唯一一个默认值为打开的SO_XXX二元套接字选项）。当该选项开启时，相应套接字将接收在其上发送的任何数据报的一个副本。

禁止这些环回副本的另一个方法是调用shutdown，并设置它的第二个为SHUT_RD

## TCP套接字选项

### TCP_MAXSEG套接字选项

该选项允许我们获取或设置TCP连接的最大分节大小。返回值是我们的TCP可以发送给对端的最大数据量，它通常是由对端使用SYN分节通告的MSS，除非我们的TCP选择使用一个比对端通告MSS小些的值。

### TCP_NODELAY套接字选项
开启该选项将禁止TCP的Nagle算法。默认情况下该算法是启动的。

Nagle算法的目的在于减少广域网上小分组的数目。该算法指出：如果某个给定连接上有待确认数据，那么原来应该作为用户写操作之响应的在该连接上立即发送相应小分组的行为就不会发生，直到现有数据被确认为止。

待确认数据：也就是我们的TCP已发送但还在等待对端确认的数据。

Nagle算法常常与另一个TCP算法-ACK延滞算法联合使用。ACK延滞算法：使得TCP在接收到数据后不立即发送ACK，而是等待一小段时间（典型值为50～200ms），然后才发送ack。

## fcntl函数

fcntl函数提供了与网络编程相关的如下特性。

1. 非阻塞式I/O
2. 信号驱动式I/O套接字
3. F_SETOWN命令允许我们指定用于接收SIGIO和SIGURG信号的套接字属主（进程ID或进程组ID）。其中SIGIO是套接字被设置为信号驱动式I/O后产生的，SIGURG信号是在新的带外数据到达套接字时产生的。F_GETOWN命令返回套接字的当前属主。

```c
#include <fcntl.h>

int fcntl(int fd,int cmd,.../* int arg */);
//返回：若成功则取决于cmd，若出错则为-1
```

每种描述符都有一组有F_GETFL命令获取或由F_SET套接字FL命令设置的文件标志。其中影响套接字描述符的两个标志是：
1. O_NONBLOCK 非阻塞式IO
2. O_ASYNC 信号驱动式IO

使用fcntl开启非阻塞式IO的典型代码将是：
```c
int flags;

if ((flags=fnctl(fd,F_GETFL,0))<0)
	err_say("F_GETFL error");
flags |=O_NONBLOCK;
if (fcntl(fd,F_SETFL,flags)<0)
	err_say("F_SETFL error");
```

```c
//wrong way to set nonblocking
if (fcntl(fd,F_SETFL,O_NONBLOCK)<0)
	err_say("F_SETFL error");
//这段代码在设置非阻塞标志的同时也清除了所有其他文件状态标志
```

```c
//close nonblocking
flags &=~O_NONBLOCK;
if (fcntl(fd,F_SETFL,flags)<0)
	err_say("F_SETFL error");
```

信号SIGIO和SIGURG与其他信号不同之处在于，这两个信号仅在已使用F_SETOWN命令给相关套接字指派了属主后才会产生。

使用socket函数新创建的套接字并没有属主。然而如果一个新的套接字是从一个监听套接字创建来的，那么套接字属主将由已连接套接字从监听套接字继承而来。

## 部分习题解答

1. 在调用conect之前，调用getsockopt得到套接字接收缓冲区的大小和MSS，并输出这两个值。connect返回成功后，再次获取这两个套接字选项并输出他们的值？

```c
#include	"unp.h"
#include	<netinet/tcp.h>		/* for TCP_MAXSEG */

int
main(int argc, char **argv)
{
	int					sockfd, rcvbuf, mss;
	socklen_t			len;
	struct sockaddr_in	servaddr;

	if (argc != 2)
		err_quit("usage: rcvbuf <IPaddress>");

	sockfd = Socket(AF_INET, SOCK_STREAM, 0);

	len = sizeof(rcvbuf);

	//care getsockopt,setsockopt中*optval的大小由最后一个参数指定，若将len设为0，结果将会得到0

	Getsockopt(sockfd, SOL_SOCKET, SO_RCVBUF, &rcvbuf, &len);
	len = sizeof(mss);
	Getsockopt(sockfd, IPPROTO_TCP, TCP_MAXSEG, &mss, &len);
	printf("defaults: SO_RCVBUF = %d, MSS = %d\n", rcvbuf, mss);

	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons(13);		/* daytime server * /
	Inet_pton(AF_INET, argv[1], &servaddr.sin_addr);

	Connect(sockfd, (SA *) &servaddr, sizeof(servaddr));

	len = sizeof(rcvbuf);
	Getsockopt(sockfd, SOL_SOCKET, SO_RCVBUF, &rcvbuf, &len);
	len = sizeof(mss);
	Getsockopt(sockfd, IPPROTO_TCP, TCP_MAXSEG, &mss, &len);
	printf("after connect: SO_RCVBUF = %d, MSS = %d\n", rcvbuf, mss);

	exit(0);
}
```

2. 编译sock出现问题
```
error: invalid storage class for function ‘sigio_func’
```
[参考](http://www.cnblogs.com/gamerh2o/archive/2012/11/25/2787693.html) 解决办法：
```
1. 把声明改到函数外面。
2. 把要声明的函数放到要定义的函数的上面，这样就不用额外声明了，这样调整顺序后把声明语句删掉就行。
```


3. 假设客户和服务器都设置了SO_KEEPALIVE套接字选项。连接两端维护联通性，但是连接上没有应用数据在交换。当保持存活定时器每2小时到期时，在连接上有多少个TCP分集被交换？
答：
只交换2个而不是4个TCP分节。两个系统的定时器精确同步的可能性非常底；因此一端的保持存活定时器会比另一端略早一点超时。首先超时的那一段发送保持存活侦探分节，导致另一端确认这个分节。然而保持存活侦探分组的接收导致时钟略慢的主机把把持存活定时器重置为2小时

4. 几乎所有实现都在头文件<sys/socket.h>中定义了SO_ACCEPTCON常值，不过我们并没有讲述这个选项。为什么该选项存在？
答：
最初的套接字API并没有listen函数。相反，socket函数的第四个参数含有套接字选项，而SO_ACCEPTCON常值就是用来指定监听套接字的。加了listen函数后，这个选项还是保留着，不过现在只是有内核来设置

[上一级](README.md)
[上一篇](6_select_poll_function.md)
[下一篇](8_basic_udp_socket.md)
