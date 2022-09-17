<!-- 三星 Galaxy Watch 5 搞机篇 -->


<!-- @import "[TOC]" {cmd="toc" depthFrom=1 depthTo=6 orderedList=false} -->

<!-- code_chunk_output -->

- [1. 利用 ADB 安装应用](#1-利用-adb-安装应用)
- [2. 启用 ECG 和血压检测](#2-启用-ecg-和血压检测)
- [3. 安装 Google 商店和谷歌服务框架](#3-安装-google-商店和谷歌服务框架)

<!-- /code_chunk_output -->

设备地区版本：中国 —— 国行版。

# 1. 利用 ADB 安装应用

1. 下载 ADB 
2. 手表上启用 ADB 调试和 ADB WI-FI 调试
3. 下载 APK ,可从 [APK Mirror](https://www.apkmirror.com/) 上下载。比如我们下载 facer 并将下载文件命名为 `facer.apk`
4. ADB 连接手表,例如：
    ```sh
    adb connect 192.168.1.325:5555
    ```
    如果手表上有弹出授权提醒，点击确认。
5. 传输 APK 文件到手表：
    ```sh
    adb push facer.apk /sdcard
    ```
    等待传输成功的提示,就像这样：
    ```sh
    facer.apk : 1 file pushed, 0 skipped. ***
    ```
6. 安装应用 
    ```sh
    adb -e install facer.apk     
    ```
    等待应用安装。某些应用安装比较慢，请耐性等待。如果安装时间太长（比如超过 30 分钟），很可能就安装不了。
7. 如果不再需要在通过 ADB 安装应用，建议关闭手表中的 ADB 调试。

其他可能遇到的问题：
- 已连接多个设备。可使用 
    ```sh
    adb disconnect
    ```
    确保只连接一个设备（方便操作）。

详见 [How to Install APKs on Wear OS Smartwatches](https://www.guidingtech.com/how-to-install-apks-on-wear-os-smartwatches/)

# 2. 启用 ECG 和血压检测

详见 [How to enable ECG and Blood Pressure features on the Samsung Galaxy Watch 4](https://www.xda-developers.com/how-to-enable-ecg-blood-pressure-monitoring-samsung-galaxy-watch-4/)

# 3. 安装 Google 商店和谷歌服务框架

安装后三星国行手机同手表的连接立即断开。使用 google wear 应用可以找到该设备，可连接。但是后面提示 “无法更新手表”，然后就没有然后了。

使用 ADB 无法禁用相关程序，也无法删除相关程序。考虑到需要配合国行手机使用，就重置系统了。


---

- [上一级](README.md)
- 上一篇 -> [利用 Pandoc 转换文档格式](pandocConvertDoc.md)
- 下一篇 -> [shadowsocks optimize](shadowsocksOptimize.md)
