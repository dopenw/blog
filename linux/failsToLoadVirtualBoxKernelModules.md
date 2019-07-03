# virtualbox on fedora 27 fails to load virtualbox Kernel Modules

## 尝试手动安装 virtualbox Kernel Modules(failed)
```sh
rcvboxdrv setup
```

执行该命令失败，使用 ```dmesg``` 查看，显示:

```highlight
vboxpci: Unknown symbol vfs_write (err 0)
```

## 升级新版的 Virtualbox (solved)

[Download VirtualBox for Linux Hosts](https://www.virtualbox.org/wiki/Linux_Downloads)




## 参考链接

[VirtualBox on Ubuntu 16.04 fails to load VirtualBox Kernel Modules](https://forums.virtualbox.org/viewtopic.php?f=7&t=77363&start=15)


[VirtualBox Kernel Module no longer builds under -current kernel update of 17-Nov-2017](https://www.linuxquestions.org/questions/slackware-14/virtualbox-kernel-module-no-longer-builds-under-current-kernel-update-of-17-nov-2017-a-4175617829/)
[上一级](README.md)
[上一篇](custom_desktop_env.md)
[下一篇](fedoraCreateCustomDesktopShortcut.md)
