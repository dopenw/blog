# 自动编译并运行cpp程序shell脚本

```sh
#/bin/bash
file_name=$1
out_name=$(echo $file_name | awk -F '.' '{print $1}')
#echo $out_name
information=$(g++ -Wall -O -g -std=c++11 $file_name -o $out_name)
if [ $(echo $?) -eq 0 ];then
	./$out_name
fi
```
- [上一级](README.md)
- 上一篇 -> [fedora install cinnamon desktop](fedoraInstallCinnamonDesktop.md)
- 下一篇 -> [插入U盘、手机等设备，gnome异常退出到登陆界面](gnomeUSBError.md)
