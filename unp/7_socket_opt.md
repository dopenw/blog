# 7.套接字选项


<!-- @import "[TOC]" {cmd="toc" depthFrom=1 depthTo=6 orderedList=false} -->
<!-- code_chunk_output -->

* [7.套接字选项](#7套接字选项)
	* [7.1 概述](#71-概述)
	* [7.2 getsockopt和setsockopt函数](#72-getsockopt和setsockopt函数)
	* [7.3 检查选项是否受支持并获取默认值](#73-检查选项是否受支持并获取默认值)
	* [7.4 套接字状态](#74-套接字状态)
	* [7.5 通用套接字选项](#75-通用套接字选项)

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
|IPPROTO_ICMPV6|ICMP6_FILTER|Y|Y|指定带传递的ICMPV6消息类型|N|icmp6_filter{}
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
    //{"SO_USELOOPBACK", SOL_SOCKET, SO_USELOOPBACK, sock_str_flag},
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
[上一级](base.md)
[上一篇](6_select_poll_function.md)
