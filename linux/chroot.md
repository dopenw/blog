<!-- 理解chroot -->

chroot在linux系统中发挥了根目录的切换工作，同时带来了系统的安全性等好处。

## 什么是chroot

chroot，即change root directory（更改root目录）。在linux系统中，系统默认的目录结构都是以‘/’（根（root））开始的。而使用了chroot后，系统的目录结构将以指定的位置作为‘/’位置。

## 为何使用chroot

在经过chroot之后，系统读取到的目录和文件将不在是旧系统根下，而是新的根（即被指定的新的位置）的目录结构和文件，因此它带来的好处大致有一下3个：

1. 增加了系统的安全性，限制了用户的权力。
2. 建立一个与原系统隔离的系统目录结构，方便用户的开发。
3. 切换系统的根目录位置，引导linux系统启动及急救盘。

[source link](https://www.ibm.com/developerworks/cn/linux/l-cn-chroot/index.html)


---
- [上一级](README.md)
- 上一篇 -> [Build openGapps from github](buildOpengapps.md)
- 下一篇 -> [linux实用命令集合](command.md)
