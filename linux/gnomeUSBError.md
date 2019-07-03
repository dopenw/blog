# 插入U盘、手机等设备，gnome异常退出到登陆界面

问题：当插入U盘、手机等设备时，桌面背景全黑，随后出现gnome异常退出到登陆界面


造成该问题的可能情况：

1. 重装 nvidia 驱动，造成 gnome 的配置文件有变动

解决办法：

重装部分 gnome 软件：

```sh
# remove
dnf remove gnome-desktop3
dnf remove gnome-session
# reinstall
dnf install @gnome-desktop
```

重新配置 gnome 界面

[上一级](README.md)
[上一篇](gen_cpp_bin.md)
[下一篇](hpOmenInstallFedora.md)
