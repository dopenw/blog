# 小米路由器 1 mini 搞机篇


<!-- @import "[TOC]" {cmd="toc" depthFrom=1 depthTo=6 orderedList=false} -->
<!-- code_chunk_output -->

* [小米路由器 1 mini 搞机篇](#小米路由器-1-mini-搞机篇)
	* [下载 openwrt SDK](#下载-openwrt-sdk)
	* [搭建科学上网利器](#搭建科学上网利器)
		* [编译 dns-forward](#编译-dns-forward)
		* [编译 openwrt-dist-luc](#编译-openwrt-dist-luc)
		* [编译 Shadowsocks-libev](#编译-shadowsocks-libev)
		* [安装 openwrt package](#安装-openwrt-package)
		* [dns-forwarder 设置](#dns-forwarder-设置)
		* [设置示例](#设置示例)

<!-- /code_chunk_output -->

## 下载 openwrt SDK

[SDK download link](https://wiki.openwrt.org/doc/howto/obtain.firmware.sdk) / 	Stable SDK "Chaos Calmer" 里面的 SDK



```sh
# 若提示：curses.h: No such file or directory
dnf install ncurses-devel ncurses
#  line 2: exec: ccache: not found
dnf install ccache
cd $SDK_HOME
./scripts/feeds update -a
# ./scripts/feeds install r8169-firmware
```

[Linux Error: curses.h: No such file or directory Problem Solution](https://www.cyberciti.biz/faq/linux-error-cursesh-no-such-file-directory/)

## 搭建科学上网利器

### 编译 dns-forward

 [dns-forward github](https://github.com/aa65535/openwrt-dns-forwarder)
```sh
cd $SDK_HOME
git clone https://github.com/aa65535/openwrt-dns-forwarder.git package/dns-forwarder
# 选择要编译的包 Network -> dns-forwarder
make menuconfig
# 开始编译
make package/dns-forwarder/compile V=99
```

### 编译 openwrt-dist-luc
 [openwrt-dist-luci github](https://github.com/aa65535/openwrt-dist-luci)

```sh
cd $SDK_HOME
git clone https://github.com/aa65535/openwrt-dist-luci.git package/openwrt-dist-luci
# 编译 po2lmo (如果有po2lmo可跳过)
pushd package/openwrt-dist-luci/tools/po2lmo
make && sudo make install
popd
# 选择要编译的包 LuCI -> 3. Applications
make menuconfig
# 开始编译
make package/openwrt-dist-luci/compile V=99
```



### 编译 Shadowsocks-libev
[Shadowsocks libev openwrt github](https://github.com/shadowsocks/openwrt-shadowsocks)

```sh
cd $SDK_HOME
# 添加 feeds
git clone https://github.com/shadowsocks/openwrt-feeds.git package/feeds
# 获取 shadowsocks-libev Makefile
git clone https://github.com/shadowsocks/openwrt-shadowsocks.git package/shadowsocks-libev
# 选择要编译的包 Network -> shadowsocks-libev
make menuconfig
# 开始编译
make package/shadowsocks-libev/compile V=99
```

[openwrt Shadowsocks-libev release](https://github.com/shadowsocks/openwrt-shadowsocks/releases) 比较新，可以直接下载

### 安装 openwrt package

安装出现如下错误：
```sh
 * pkg_hash_fetch_best_installation_candidate: Packages for libmbedtls found, but incompatible with the architectures configured
```


```sh
echo "arch all 1
arch noarch 1
arch ralink 10

arch ramips_24kec 200
arch ramips 300
arch mips 400
" >> /etc/opkg.conf
```
相关参考链接：
[slove "but incompatible with the architectures configured"](https://sourceforge.net/p/openwrt-dist/wiki/Home/?version=14)

[小米路由器mini折腾之配置opkg篇](https://blog.phpgao.com/xiaomi_router_opkg.html):附注，其他的源设置等已实效,网传之前的源让该路由器变砖

[安装的时候显示下面的错误？何解啊？ #5](https://github.com/zonyitoo/sysuh3c/issues/5)

```sh
opkg install shadowsocks-libev_3.1.3-1_ramips_24kec.ipk libev_4.24-1_ramips_24kec.ipk libcares_1.13.0-1_ramips_24kec.ipk libpcre_8.41-2_ramips_24kec.ipk libsodium_1.0.16-1_ramips_24kec.ipk libmbedtls_2.6.0-2_ramips_24kec.ipk
# if show "can't load library 'libmbedcrypto.so.0'"
ln -s /usr/lib/libmbedcrypto.so /usr/lib/libmbedcrypto.so.0

# 安装 luci-*后无法正常在路由器设置界面中的服务项显示
# opkg install luci-app-chinadns_1.6.1-1_all.ipk luci-app-dns-forwarder_1.6.1-1_all.ipk luci-app-redsocks2_1.6.1-1_all.ipk
```

shadowsocks-libev ,luci-shadowsocks-libev,chinadns,luci-chinadns 等在openwrt的源里面有，最好使用源里面的

### dns-forwarder 设置

###设置示例

[DNS Forwarder wiki](https://github.com/aa65535/openwrt-chinadns/wiki/Use-DNS-Forwarder)

/etc/config/dns-forwarder

```sh
config dns-forwarder
	option listen_addr '0.0.0.0'
	option listen_port '5300'
	option dns_servers '8.8.8.8'
	option enable '1'
```

/etc/config/chinadns

```sh
config chinadns
	option bidirectional '0'
	option chnroute '/etc/chinadns_chnroute.txt'
	option port '5353'
	option enable '1'
	option server '114.114.114.114,127.0.0.1#5300'
```

/etc/config/dhcp

```sh
config dnsmasq
	option domainneeded '1'
	option boguspriv '1'
	option localise_queries '1'
	option rebind_protection '1'
	option rebind_localhost '1'
	option expandhosts '1'
	option readethers '1'
	option leasefile '/tmp/dhcp.leases'
	option localservice '1'
	option noresolv '1'
	option nohosts '1'
	option local '127.0.0.1#5353'
```
[上一级](base.md)
[上一篇](v2ray.md)
