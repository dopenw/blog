<!-- fedora install SS libev -->

<!-- @import "[TOC]" {cmd="toc" depthFrom=1 depthTo=6 orderedList=false} -->
<!-- code_chunk_output -->

- [Install SS libev shell script](#install-ss-libev-shell-script)
- [Create shadowsocks.service](#create-shadowsocksservice)
- [为其他区域网设备提供代理服务](#为其他区域网设备提供代理服务)
  - [linux](#linux)
  - [windows](#windows)
  - [在其他设备设置](#在其他设备设置)
- [Link](#link)

<!-- /code_chunk_output -->

## Install SS libev shell script
```sh
#!/bin/bash

# install vim and git

dnf install git vim -y

# install shadowsocks from repository

dnf copr enable librehat/shadowsocks
dnf update -y
dnf install shadowsocks-libev -y

# add shadowsocks config
# server config
echo "{
    \"server\":[\"[::0]\", \"0.0.0.0\"],
    \"server_port\":80,
    \"local_port\":1080,
    \"password\":\"password\",
    \"plugin\":\"obfs-server\",
    \"plugin_opts\":\"obfs=tls\",
    \"timeout\":600,
    \"method\":\"aes-256-gcm\",
    \"fast_open\":true
}" > /etc/shadowsocks-libev/config.json
# Client config
echo "{
    \"server\":\"server_ip_address\",
    \"server_port\":80,
    \"local_port\":1080,
    \"password\":\"password\",
    \"plugin\":\"obfs-local\",
    \"plugin_opts\":\"obfs=tls;obfs-host=github.com\",
    \"timeout\":600,
    \"method\":\"aes-256-gcm\",
    \"fast_open\":true
}" > /etc/shadowsocks-libev/config.json


# install shadowsocks obfs

dnf install gcc autoconf libtool automake make zlib-devel openssl-devel asciidoc xmlto libev-devel -y
git clone https://github.com/shadowsocks/simple-obfs.git
cd simple-obfs
git submodule update --init --recursive
./autogen.sh
./configure && make
sudo make install

# enable bbr

modprobe tcp_bbr
echo "tcp_bbr" >> /etc/modules-load.d/moudules.conf
echo "#enable bbr" >> /etc/sysctl.conf
echo "net.core.default_qdisc=fq" >> /etc/sysctl.conf
echo "net.ipv4.tcp_congestion_control=bbr" >> /etc/sysctl.conf
sysctl -p

# check bbr is open

echo "#####################################"
echo "check bbr status"
sysctl net.ipv4.tcp_available_congestion_control
sysctl net.ipv4.tcp_congestion_control
lsmod | grep bbr
echo "#####################################"

# optimize kernel

# echo "# max open files
# fs.file-max = 1024000
# # max read buffer
# net.core.rmem_max = 67108864
# # max write buffer
# net.core.wmem_max = 67108864
# # default read buffer
# net.core.rmem_default = 65536
# # default write buffer
# net.core.wmem_default = 65536
# # max processor input queue
# net.core.netdev_max_backlog = 4096
# # max backlog
# net.core.somaxconn = 4096
#
# # resist SYN flood attacks
# net.ipv4.tcp_syncookies = 1
# # reuse timewait sockets when safe
# net.ipv4.tcp_tw_reuse = 1
# # turn off fast timewait sockets recycling
# net.ipv4.tcp_tw_recycle = 0
# # short FIN timeout
# net.ipv4.tcp_fin_timeout = 30
# # short keepalive time
# net.ipv4.tcp_keepalive_time = 1200
# # outbound port range
# net.ipv4.ip_local_port_range = 10000 65000
# # max SYN backlog
# net.ipv4.tcp_max_syn_backlog = 4096
# # max timewait sockets held by system simultaneously
# net.ipv4.tcp_max_tw_buckets = 5000
# # TCP receive buffer
# net.ipv4.tcp_rmem = 4096 87380 67108864
# # TCP write buffer
# net.ipv4.tcp_wmem = 4096 65536 67108864
# # turn on path MTU discovery
# net.ipv4.tcp_mtu_probing = 1
#
# # for high-latency network
# net.ipv4.tcp_congestion_control = hybla
# # forward ipv4
# net.ipv4.ip_forward = 1" >> /etc/sysctl.conf
#
# sysctl -p

# open tcp fast open
echo "net.ipv4.tcp_fastopen = 3" >> /etc/sysctl.conf
sysctl -p

```

## Create shadowsocks.service
```sh
vim /etc/systemd/system/shadowsocks.service

[Unit]
Description=shadowsocks
After=network.target

[Service]
User=breap
ExecStart=/usr/bin/ss-local -c /etc/shadowsocks.json -u
Restart=on-failure
ExecReload=/bin/kill -HUP $MAINPID
KillMode=process

[Install]
WantedBy=multi-user.target
```

## 为其他区域网设备提供代理服务

### linux
这里我们可以使用 privoxy,安装及简单介绍可以看 [ss+privoxy实现全局代理](./ss_privoxy.md)
修改配置文件：
```sh
vim /etc/privoxy/config
```
```sh
# modify
listen-address  :8118  
enable-remote-toggle  1
enable-remote-http-toggle 1
# add ,端口号是网络代理(eg ss)的 local_port
forward-socks5 / 127.0.0.1:1080  
```

### windows
打开 shadowsocks 客户端 :[shadowsocks/shadowsocks-windows](https://github.com/shadowsocks/shadowsocks-windows)
打开全局代理，勾选“允许来自局域网的连接”

### 在其他设备设置

然后在需要提供代理的设备上，修改连接的wifi配置-添加代理服务器和端口就可以了：

## Link
* [shadowsocks/simple-obfs](https://github.com/shadowsocks/simple-obfs)
* [Linux下部署代理服务器](https://blog.csdn.net/m0_38110132/article/details/79796171)

---
- [上一级](README.md)
- 上一篇 -> [docker 使用记录（读docker实践记）](docker.md)
- 下一篇 -> [c/c++面试](find_a_job.md)
