<!-- Install ss-libev and config v2ray-plugin on ubuntu -->

```sh
#!/bin/bash

# install vim and git and gcc

apt install -y git vim gcc

# install shadowsocks from repository

# apt copr enable librehat/shadowsocks -y
apt update -y
# apt install -y shadowsocks-libev
apt install wget

wget https://github.com/shadowsocks/shadowsocks-rust/releases/download/v1.16.2/shadowsocks-v1.16.2.x86_64-unknown-linux-gnu.tar.xz
sudo tar -C /usr/local -xf shadowsocks-v1.16.2.x86_64-unknown-linux-gnu.tar.xz

# install go 1.13.8
wget https://go.dev/dl/go1.16.5.linux-amd64.tar.gz
sudo tar -C /usr/local -xzf go1.16.5.linux-amd64.tar.gz
export PATH=$PATH:/usr/local/go/bin
source ~/.profile
go version

# build v2ray plugin
git clone https://github.com/shadowsocks/v2ray-plugin.git
cd v2ray-plugin
go build
cp v2ray-plugin /usr/bin/v2ray-plugin

# solve "ss-server This system doesn't provide enough entropy to quickly generate high-quality random numbers."
# apt-get install -y rng-tools
# rngd -r /dev/urandom


# add shadowsocks config
# server config

mkdir /etc/shadowsocks-rust

generatedPassword=$(/usr/local/ssservice genkey -m "aes-256-gcm")

echo "{
    \"server\":\"0.0.0.0\",
    \"server_port\":6653,
    \"local_port\":1080,
    \"password\":\"$generatedPassword\",
    \"timeout\":600,
    \"method\":\"aes-256-gcm\",
    \"fast_open\":true,
    \"plugin\":\"v2ray-plugin\",
    \"plugin_args\": [
        // Each line is an argument passed to "plugin"
        \"-server\"
    ]
}" > /etc/shadowsocks-rust/configPhone.json

# computer json
echo "{
   \"server\":\"0.0.0.0\",
    \"server_port\":7655,
    \"local_port\":1080,
    \"password\":\"$generatedPassword\",
    \"timeout\":600,
    \"method\":\"aes-256-gcm\",
    \"fast_open\":true,
    \"plugin\":\"v2ray-plugin\",
    \"plugin_args\": [
        // Each line is an argument passed to "plugin"
        \"-server\"
    ]
}" > /etc/shadowsocks-rust/configComputer.json

# other json
echo "{
       \"server\":\"0.0.0.0\",
    \"server_port\":8657,
    \"local_port\":1080,
    \"password\":\"$generatedPassword\",
    \"timeout\":600,
    \"method\":\"aes-256-gcm\",
    \"fast_open\":true,
    \"plugin\":\"v2ray-plugin\",
    \"plugin_args\": [
        // Each line is an argument passed to "plugin"
        \"-server\"
    ]
}" > /etc/shadowsocks-rust/configOther.json

# Client config
echo "{
    \"server\":\"YOUR_SERVER_IP\",
    \"server_port\":6653,
    \"local_address\": \"127.0.0.1\",
    \"local_port\":1080,
    \"password\":\"$generatedPassword\",
    \"timeout\":300,
    \"method\":\"aes-256-gcm\",
    \"plugin\":\"v2ray-plugin\",
    \"fast_open\":true
}" > /etc/shadowsocks-rust/config.json



# # install shadowsocks obfs
#
# apt install -y gcc autoconf libtool automake make zlib-devel openssl-devel asciidoc xmlto libev-devel
# # mkdir ssDir && cd ssDir
# git clone https://github.com/shadowsocks/simple-obfs.git
# cd simple-obfs
# git submodule update --init --recursive
# ./autogen.sh
# ./configure && make
# sudo make install

# enable bbr

modprobe tcp_bbr
echo "tcp_bbr" >> /etc/modules-load.d/modules.conf
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

echo
"# max open files
fs.file-max = 51200
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
# turn on TCP Fast Open on both client and server side
net.ipv4.tcp_fastopen = 3
# TCP receive buffer
net.ipv4.tcp_rmem = 4096 87380 67108864
# TCP write buffer
net.ipv4.tcp_wmem = 4096 65536 67108864
# turn on path MTU discovery
net.ipv4.tcp_mtu_probing = 1

# for high-latency network
net.ipv4.tcp_congestion_control = hybla

# for low-latency network, use cubic instead
# net.ipv4.tcp_congestion_control = cubic" >/etc/sysctl.d/local.conf

# open tcp fast open
echo "net.ipv4.tcp_fastopen = 3" >> /etc/sysctl.conf

sysctl --system

## create shadowsocks user
useradd -m shadowsocks

# create shadowsocks systemctl service

# config phone
echo "[Unit]
Description=shadowsocks
After=network.target

[Service]
User=shadowsocks
ExecStart=/usr/local/ssserver -c /etc/shadowsocks-rust/configPhone.json
Restart=on-failure
ExecReload=/bin/kill -HUP $MAINPID
KillMode=process

[Install]
WantedBy=multi-user.target" > /etc/systemd/system/SSPhone.service

# config computer
echo "[Unit]
Description=shadowsocks
After=network.target

[Service]
User=shadowsocks
ExecStart=/usr/local/ssserver -c /etc/shadowsocks-rust/configComputer.json
Restart=on-failure
ExecReload=/bin/kill -HUP $MAINPID
KillMode=process

[Install]
WantedBy=multi-user.target" > /etc/systemd/system/SSComputer.service

# config other
echo "[Unit]
Description=shadowsocks
After=network.target

[Service]
User=shadowsocks
ExecStart=/usr/local/ssserver -c /etc/shadowsocks-rust/configOther.json
Restart=on-failure
ExecReload=/bin/kill -HUP $MAINPID
KillMode=process

[Install]
WantedBy=multi-user.target" > /etc/systemd/system/SSOther.service


systemctl start SSPhone
systemctl start SSComputer
systemctl start SSOther

systemctl stop SSPhone
systemctl stop SSComputer
systemctl stop SSOther

echo "shadowsocks status ################################################################"
systemctl status SSPhone
systemctl status SSComputer
systemctl status SSOther

echo "enable shadowsocks on boot ################################################################"
systemctl enable SSPhone
systemctl enable SSComputer
systemctl enable SSOther

```

## Link
* [安装shadowsocks-libev插件v2ray-plugin](https://medium.com/@caorun/%E5%AE%89%E8%A3%85shadowsocks-libev%E6%8F%92%E4%BB%B6v2ray-plugin-dfd10ecc0085)
* [shadowsocks-libev配置v2ray-plugin](https://jackzhou.co/?p=108)
* [shadowsocks/v2ray-plugin](https://github.com/shadowsocks/v2ray-plugin)
* [shadowsocks/shadowsocks-rust](https://github.com/shadowsocks/shadowsocks-rust)

---
- [上一级](README.md)
- 上一篇 -> [ss+privoxy实现全局代理](ss_privoxy.md)
- 下一篇 -> [v2ray](v2ray.md)
