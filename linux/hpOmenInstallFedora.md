# 惠普暗影精灵4安装fedora 28

<!-- @import "[TOC]" {cmd="toc" depthFrom=1 depthTo=6 orderedList=false} -->
<!-- code_chunk_output -->

* [惠普暗影精灵4安装fedora 28](#惠普暗影精灵4安装fedora-28)
	* [为什么写这个文章?](#为什么写这个文章)
	* [安装 fedora](#安装-fedora)
	* [链接](#链接)

<!-- /code_chunk_output -->

## 为什么写这个文章?
我按照以前安装 fedora 那样做：
1. 下载 fedora iso 文件
2. 利用 fedora media writer 工具制作 U盘启动盘
3. 开机设置 u 盘启动
4. 进入安装界面（Start Fedora Live）

当我使用 Start Fedora Live 方法时，我前5次都没能进入安装界面（一只在开启图形界面处卡住）；第6次（后多次尝试，有极小的概率能够正常进入）进入了安装界面，进入界面后我看见 fedora 报了多次 nouveau 错误，当时并没有细想这个错误，然后我就安装了系统。当我以为成功安装了系统的时候，开机后依旧卡在开启图形界面处。我想起之前看到的错误，并在网上搜了下后，我猜测可能是 hp 暗影精灵4  与 nouveau 驱动不兼容（确实是该问题）。

按以前那样在 hp 暗影精灵4 上正常安装 fedora  是不可行的。

## 安装 fedora


我在 [fedora Troubleshooting documents](https://docs.fedoraproject.org/f28/install-guide/install/Troubleshooting.html) 里面找到了相关说明。

引导到图形化安装时有问题：
带有某些视频卡的系统无法启动到图形安装程序。如果安装程序未使用其默认设置运行，则会尝试以较低分辨率模式运行。如果仍然失败，安装程序将尝试以文本模式运行。

有几种可能的解决方案来显示问题，其中大部分涉及指定自定义引导选项。
1. 使用基本的图形模式
您可以尝试使用基本图形驱动程序执行安装。为此，请编辑安装程序的启动选项并在命令行末尾追加inst.xdriver = vesa。

2. 手动指定显示分辨率
如果安装程序无法检测到您的屏幕分辨率，则可以覆盖自动检测并手动指定它。为此，请在启动菜单追加inst.resolution = x选项，其中x是显示器的分辨率（例如1024x768）。

3. 使用备用视频驱动程序
您还可以尝试指定自定义视频驱动程序，覆盖安装程序的自动检测。要指定驱动程序，请使用inst.xdriver = x选项，其中x是要使用的设备驱动程序（例如nouveau）。

解决方法：
启动 fedora live USB ：
1. 编辑 ```Start Fedora-Workstation-Live 28 ``` 选项
2. 在这行 ```linuxefi /images/pxeboot/vmlinuz root=Live:CDLABEL=Fedora-WS-Live-28-1-1 rd.live.image quiet ``` 后面添加 ```modprobe.blacklist=nouveau```
3. Press Ctrl-x to start

目前安装后还存在的问题：
1. 安装 nvidia 显卡驱动后，shutdown fedora28 左上角有雪花点的异常，登陆用户时会黑屏几秒钟时间
2.
```sh
ACPI Error: Method parse/execution failed \_SB.PCI0.SPI1.FPNT._CRS, AE_AML_INVALID_RESOURCE_TYPE (20180313/psparse-516)
kernel: ACPI Error: Method execution failed \_SB.PCI0.SPI1.FPNT._CRS, AE_AML_INVALID_RESOURCE_TYPE (20180313/uteval-69)
kernel: ACPI Error: Method parse/execution failed \_SB.PCI0.SPI2.FPNT._CRS, AE_AML_INVALID_RESOURCE_TYPE (20180313/psparse-516)
kernel: ACPI Error: Method execution failed \_SB.PCI0.SPI2.FPNT._CRS, AE_AML_INVALID_RESOURCE_TYPE (20180313/uteval-69)
```

## 链接
* [浅说基于 Linux 内核的操作系统 (5) - 安装 Fedora](https://blog.yoitsu.moe/linux/linux_install_fedora.html)
* [win10_ubuntu1604_caffe安装](http://www.voidcn.com/article/p-uznbwqnf-nx.html)
* [fedora Troubleshooting documents](https://docs.fedoraproject.org/f28/install-guide/install/Troubleshooting.html)
* [Configuring the Installation System at the Boot Menu](https://docs.fedoraproject.org/f28/install-guide/advanced/Boot_Options.html#sect-boot-options-installer)
* [Installing Kali Linux on Hp Omen 15](https://unix.stackexchange.com/questions/440884/installing-kali-linux-on-hp-omen-15)

- [上一级](README.md)
- 上一篇 -> [插入U盘、手机等设备，gnome异常退出到登陆界面](gnomeUSBError.md)
- 下一篇 -> [fedora install netease cloud music](install-netease-cloud-music.md)
