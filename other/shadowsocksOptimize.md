# shadowsocks optimize


<!-- @import "[TOC]" {cmd="toc" depthFrom=1 depthTo=6 orderedList=false} -->
<!-- code_chunk_output -->

* [shadowsocks optimize](#shadowsocks-optimize)
	* [enable bbr](#enable-bbr)
	* [optimize kernel](#optimize-kernel)
	* [open TCP Fast Open](#open-tcp-fast-open)
	* [select encrypt_method](#select-encrypt_method)
	* [link](#link)

<!-- /code_chunk_output -->


## enable bbr

[link](https://github.com/iMeiji/shadowsocks_install/wiki/%E5%BC%80%E5%90%AFTCP-BBR%E6%8B%A5%E5%A1%9E%E6%8E%A7%E5%88%B6%E7%AE%97%E6%B3%95)

## optimize kernel

in server(ubuntu):
```sh
vim /etc/sysctl.conf
```

add the lines:
```sh
# max open files
fs.file-max = 1024000
# max read buffer
net.core.rmem_max = 67108864
# max write buffer
net.core.wmem_max = 67108864
# default read buffer
net.core.rmem_default = 65536
# default write buffer
net.core.wmem_default = 65536
# max processor input queue
net.core.netdev_max_backlog = 4096
# max backlog
net.core.somaxconn = 4096

# resist SYN flood attacks
net.ipv4.tcp_syncookies = 1
# reuse timewait sockets when safe
net.ipv4.tcp_tw_reuse = 1
# turn off fast timewait sockets recycling
net.ipv4.tcp_tw_recycle = 0
# short FIN timeout
net.ipv4.tcp_fin_timeout = 30
# short keepalive time
net.ipv4.tcp_keepalive_time = 1200
# outbound port range
net.ipv4.ip_local_port_range = 10000 65000
# max SYN backlog
net.ipv4.tcp_max_syn_backlog = 4096
# max timewait sockets held by system simultaneously
net.ipv4.tcp_max_tw_buckets = 5000
# TCP receive buffer
net.ipv4.tcp_rmem = 4096 87380 67108864
# TCP write buffer
net.ipv4.tcp_wmem = 4096 65536 67108864
# turn on path MTU discovery
net.ipv4.tcp_mtu_probing = 1

# for high-latency network
net.ipv4.tcp_congestion_control = hybla
# forward ipv4
net.ipv4.ip_forward = 1
```

in terminal,run :
```sh
sysctl -p
```

## open TCP Fast Open
1. open tcp_fastopen in kernel

in ubuntu:
```sh
vim /etc/sysctl.conf
```

in fedora(such as):
```sh
vim /etc/sysctl.d/99-sysctl.conf
```

add the line:
```sh
net.ipv4.tcp_fastopen = 3
```
in terminal,run :
```sh
sysctl -p
```

2. add "fast_open" in shadowsocks.json（server and client）
```sh
"fast_open": true
```

## select encrypt_method

my computer and phone:"AES hardware support"

In this environment: aes better than chacha20

* aes-256-gcm

[link](http://blog.csdn.net/Bleachswh/article/details/78079185)

## link

[link1](https://github.com/shadowsocks/shadowsocks/wiki/Optimizing-Shadowsocks)

[link2](https://github.com/iMeiji/shadowsocks_install/wiki/shadowsocks-optimize)
[上一级](base.md)
[上一篇](openresty_test.md)
[下一篇](skip_opengapps_init.md)
