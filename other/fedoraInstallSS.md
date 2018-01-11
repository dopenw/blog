# fedora install SS libev

```sh
#!/bin/bash

# install vim and git

dnf install git vim -y

# install shadowsocks from repository

dnf copr enable librehat/shadowsocks
dnf update -y
dnf install shadowsocks-libev -y

# add shadowsocks config

echo "{
    \"server\":[\"[::0]\", \"0.0.0.0\"],
    \"server_port\":5633,
    \"local_port\":1080,
    \"password\":\"password\",
    \"plugin\":\"obfs-server\",
    \"plugin_opts\":\"obfs=tls\",
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
[上一级](base.md)
[上一篇](docker.md)
[下一篇](find_a_job.md)
