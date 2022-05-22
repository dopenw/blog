<!-- shutter 截取屏幕异常 -->

截取到的屏幕像这样：

![](../images/shutterException_201806101950_1.png)

我在命令行运行 ```shutter```,根据提示的信息：
```sh
ERROR: upload-plugin exists but does not work properly - /usr/share/shutter/resources/system/upload_plugins/upload/vgyme
```

发现
```sh
 Shutter doesn\'t support Wayland, unfortunately.
```

解决办法：

```highlight
Edit /etc/gdm/custom.conf and add WaylandEnable=false to the [daemon] section.
```

## Link
* [Shutter launches with Completely Empty GUI](https://bugs.launchpad.net/ubuntu/+source/shutter/+bug/1764879)
* [How can I set XOrg as a default on Fedora 25?](https://ask.fedoraproject.org/en/question/96134/how-can-i-set-xorg-as-a-default-on-fedora-25/)

---
- [上一级](README.md)
- 上一篇 -> [linux 恢复 GRUB2](restoreGRUB2.md)
- 下一篇 -> [su 和 su - 的区别](su_diff_su-.md)
