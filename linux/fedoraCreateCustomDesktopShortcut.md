<!-- fedora create custom desktop shortcut -->

1. Go to the location
```sh
cd $HOME/.local/share/applications
```

2. create .desktop file
```sh
#such as
vim stacer.desktop
```
```sh
[Desktop Entry]
Version=8.0
Type=Application
Name=stacer
Icon=/home/breap/software/icons/stacer.png
Exec=bash -c "stacer"
Comment=stacer
Categories=Tools;
Terminal=false           
```

## Link
* [Desktop shortcut for application launcher in Fedora 23](https://technoref.wordpress.com/2016/01/18/desktop-shortcut-in-fedora-23/)

---
- [上一级](README.md)
- 上一篇 -> [virtualbox on fedora 27 fails to load virtualbox Kernel Modules](failsToLoadVirtualBoxKernelModules.md)
- 下一篇 -> [fedora install cinnamon desktop](fedoraInstallCinnamonDesktop.md)
