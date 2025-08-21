<!-- v2ray -->

<!-- @import "[TOC]" {cmd="toc" depthFrom=1 depthTo=6 orderedList=false} -->
<!-- code_chunk_output -->

- [下载安装](#下载安装)
- [编辑配置文件](#编辑配置文件)
- [运行 v2ray](#运行-v2ray)
- [遇到的问题](#遇到的问题)
- [Link](#link)

<!-- /code_chunk_output -->
[v2ray/v2ray-core](https://github.com/v2ray/v2ray-core)

## 下载安装

V2Ray 提供了一个在 Linux 中的自动化安装脚本。这个脚本会自动检测有没有安装过 V2Ray，如果没有，则进行完整的安装和配置；如果之前安装过
V2Ray，则只更新 V2Ray 二进制程序而不更新配置。

以下指令假设已在 su 环境下，如果不是，请先运行 sudo su。

运行下面的指令下载并安装 V2Ray。当 yum 或 apt-get 可用的情况下，此脚本会自动安装 unzip 和 daemon。这两个组件是安装 V2Ray
的必要组件。如果你使用的系统不支持 yum 或 apt-get，请自行安装 unzip 和 daemon

```sh
bash <(curl -L https://raw.githubusercontent.com/v2fly/fhs-install-v2ray/master/install-release.sh)
```

脚本运行完成后,编辑 /usr/local/etc/v2ray/config.json 配置文件：

## 编辑配置文件

客户端配置示例：

```json
{
  "inbounds": [
    {
      "port": 1080,
      // 监听端口
      "protocol": "socks",
      // 入口协议为 SOCKS 5
      "sniffing": {
        "enabled": true,
        "destOverride": [
          "http",
          "tls"
        ]
      },
      "settings": {
        "auth": "noauth"
        //socks的认证设置，noauth 代表不认证，由于 socks 通常在客户端使用，所以这里不认证
      }
    }
  ],
  "outbounds": [
    {
      "protocol": "vmess",
      // 出口协议
      "settings": {
        "vnext": [
          {
            "address": "serveraddr.com",
            // 服务器地址，请修改为你自己的服务器 IP 或域名
            "port": 16823,
            // 服务器端口
            "users": [
              {
                "id": "b831381d-6324-4d53-ad4f-8cda48b30811",
                // 用户 ID，必须与服务器端配置相同,类似于 ss 的密UUID
                "alterId": 64
                // 此处的值也应当与服务器相同
              }
            ]
          }
        ]
      }
    }
  ]
}
```

由于 id 使用的是 UUID 的格式，我们可以使用任何 UUID 生成工具生成 UUID 作为这里的
id。比如 [UUID Generator](https://www.uuidgenerator.net/) 这个网站，只要一打开或者刷新这个网页就可以得到一个
UUID，如下图。或者可以在 Linux 使用命令 cat /proc/sys/kernel/random/uuid 生成。

服务器配置示例：

```json
{
  "inbounds": [
    {
      "port": 16823,
      // 服务器监听端口
      "protocol": "vmess",
      // 主传入协议
      "settings": {
        "clients": [
          {
            "id": "b831381d-6324-4d53-ad4f-8cda48b30811",
            // 用户 ID，客户端与服务器必须相同
            "alterId": 64
          }
        ]
      }
    }
  ],
  "outbounds": [
    {
      "protocol": "freedom",
      // 主传出协议
      "settings": {}
    }
  ]
}
```

## 运行 v2ray

```sh
systemctl start v2ray # 运行 v2ray
systemctl status v2ray # 查看 v2ray 运行状态
systemctl enable v2ray # 设置为开机启动
```

## 遇到的问题

1. 注意两端的时间要一致，比如 客户端时间为 11.12 ，服务端时间为 11.14 ,则不能正常工作。

## Link

* [v2ray手册](https://www.v2ray.com/)
* [白话文教程/VMess](https://toutyrater.github.io/basic/vmess.html)
* [ windows下正常，Archlinux下不能正常代理 #1050](https://github.com/v2ray/v2ray-core/issues/1050)

---

- [上一级](README.md)
- 上一篇 -> [Install ss-libev and config v2ray-plugin on ubuntu](ubuntuInstallSSAndV2rayPlugin.md)
- 下一篇 -> [vsCode](vsCode.md)
