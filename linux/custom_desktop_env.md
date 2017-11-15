# 自定义fedora桌面环境


<!-- @import "[TOC]" {cmd="toc" depthFrom=1 depthTo=6 orderedList=false} -->
<!-- code_chunk_output -->

* [自定义fedora桌面环境](#自定义fedora桌面环境)
	* [gnome shell extensions](#gnome-shell-extensions)
	* [gnome theme](#gnome-theme)
	* [gnome icons](#gnome-icons)
	* [gnome cusor](#gnome-cusor)
	* [显示效果](#显示效果)
	* [截屏](#截屏)
	* [系统清理工具](#系统清理工具)
	* [utilties](#utilties)
	* [upgrade fedora26 to fedora27](#upgrade-fedora26-to-fedora27)

<!-- /code_chunk_output -->


## gnome shell extensions

1. NetSpeed

2. EasyScreenCast

3. Laine

4. Dash to Dock

5. Gno-Menu

[youtube video link](https://www.youtube.com/watch?v=YFvFo5sUAhw)


## gnome theme

1. [paper-gtk-theme](https://github.com/snwh/paper-gtk-theme)

2. [numix-gtk-theme](https://github.com/numixproject/numix-gtk-theme)

3. [adapta-gtk-theme](https://github.com/adapta-project/adapta-gtk-theme)

4. [arc-theme](https://github.com/horst3180/arc-theme)

5. [minimalism](http://xenlism.github.io/minimalism/)

[youtube video link](https://www.youtube.com/watch?v=o8pfCuBOlXE)

## gnome icons

1. [Flat-Remix](https://github.com/daniruiz/Flat-Remix)

## gnome cusor

1. [Breeze Serie](https://www.gnome-look.org/content/show.php?content=164300)

[youtube link](https://www.youtube.com/watch?v=XKc5DXa8JSs&t=70s)

## 显示效果

采用：
1. NetSpeed
2. EasyScreenCast
3. Dash to Dock
4. paper-gtk-theme
5. Flat-Remix
6. Breeze Serie

![](../images/custom_desktop_env_201711071727_1.png)

## 截屏

实现快捷截取选择屏幕区域:
"alter + a" -> "shutter -s -e"

## 系统清理工具

1. [bleachbit](https://www.bleachbit.org/)

![](../images/custom_desktop_env_201711132254_1.png)

2. [stacer](https://github.com/oguzhaninan/Stacer)

![](../images/custom_desktop_env_201711132254_2.png)

3. [gcleaner](https://launchpad.net/gcleaner)

[youtube link](https://www.youtube.com/watch?v=wdC1n18kv_E&t=283s)

## utilties

1. [fedy](https://www.folkswithhats.org/)

![](../images/custom_desktop_env_201711141142_1.png)

2. [overgrive](https://www.thefanclub.co.za/overgrive)
开启全局http，https代理后依旧一直处于offline(暂不可用)

[youtube link](https://youtu.be/-rnnhcTqgys)

## upgrade fedora26 to fedora27

[link](https://fedoramagazine.org/upgrading-fedora-25-fedora-26/)
```sh
dnf --allowerasing system-upgrade download --releasever=27
```
![](../images/custom_desktop_env_201711151103_1.png)


[上一级](base.md)
[上一篇](ctrl_alt_f3_login_incorrect.md)
[下一篇](gen_cpp_bin.md)
