# su 和 su - 的区别

我们在某些场景下，需要切换root用户，那么这两个到底有什么区别呢？

1. 我们使用printenv打印当前用户的环境变量：
```sh
GNOME_DESKTOP_SESSION_ID=this-is-deprecated
LOGNAME=breap
DBUS_SESSION_BUS_ADDRESS=unix:path=/run/user/1002/bus
XDG_RUNTIME_DIR=/run/user/1002
cpp=/home/breap/Desktop/cpp
python=/home/breap/project/python
XAUTHORITY=/run/user/1002/gdm/Xauthority
PATH=/usr/local/bin:/usr/local/sbin:/usr/bin:/usr/sbin:/home/breap/.local/bin:/home/breap/bin:/home/breap/software/luacheck/bin
MODULESHOME=/usr/share/Modules
HISTSIZE=1000
SESSION_MANAGER=local/unix:@/tmp/.ICE-unix/1832,unix/unix:/tmp/.ICE-unix/1832
desktop=/home/breap/Desktop
LESSOPEN=||/usr/bin/lesspipe.sh %s
BASH_FUNC_module%%=() {  eval `/usr/bin/modulecmd bash $*`
}
BASH_FUNC_scl%%=() {  if [ "$1" = "load" -o "$1" = "unload" ]; then
 eval "module $@";
 else
 /usr/bin/scl "$@";
 fi
}
_=/usr/bin/printenv
```
2. 使用su命令切换root后使用printenv:
```sh
LOGNAME=breap
DBUS_SESSION_BUS_ADDRESS=unix:path=/run/user/1002/bus
XDG_RUNTIME_DIR=/run/user/1002
cpp=/home/breap/Desktop/cpp
python=/home/breap/project/python
XAUTHORITY=/root/.xauth3s6apU
PATH=/usr/local/bin:/usr/local/sbin:/usr/bin:/usr/sbin:/home/breap/.local/bin:/home/breap/bin:/home/breap/software/luacheck/bin
MODULESHOME=/usr/share/Modules
HISTSIZE=1000
desktop=/home/breap/Desktop
SESSION_MANAGER=local/unix:@/tmp/.ICE-unix/1832,unix/unix:/tmp/.ICE-unix/1832
download=/home/breap/Downloads
LESSOPEN=||/usr/bin/lesspipe.sh %s
BASH_FUNC_module%%=() {  eval `/usr/bin/modulecmd bash $*`
}
BASH_FUNC_scl%%=() {  if [ "$1" = "load" -o "$1" = "unload" ]; then
 eval "module $@";
 else
 /usr/bin/scl "$@";
 fi
}
_=/usr/bin/printenv
```

这时，我们发现这和普通用户的环境变量是一样的

3. 使用su - 命令切换root后使用printenv:

```sh
LOADEDMODULES=
MAIL=/var/spool/mail/root
SHELL=/bin/bash
TERM=xterm-256color
SHLVL=1
MODULEPATH=/etc/scl/modulefiles:/usr/share/Modules/modulefiles:/etc/modulefiles:/usr/share/modulefiles
LOGNAME=root
XAUTHORITY=/root/.xauthrBrHh8
PATH=/usr/local/sbin:/usr/local/bin:/usr/sbin:/usr/bin:/root/bin
MODULESHOME=/usr/share/Modules
HISTSIZE=1000
download=/home/breap/Downloads
LESSOPEN=||/usr/bin/lesspipe.sh %s
BASH_FUNC_module%%=() {  eval `/usr/bin/modulecmd bash $*`
}
BASH_FUNC_scl%%=() {  if [ "$1" = "load" -o "$1" = "unload" ]; then
 eval "module $@";
 else
 /usr/bin/scl "$@";
 fi
}
_=/usr/bin/printenv
```
现在是root本身的环境变量

小结：使用这两个命令都可以切换root账户，但是本身切换后的环境变量是不一样的，su切换后的环境变量是切换前用户的环境变量，而su - 切换后的环境变量是root的环境变量
---
- [上一级](README.md)
- 上一篇 -> [shutter 截取屏幕异常](shutterException.md)
- 下一篇 -> [解决 Wine 下字体的乱码、模糊等问题](wineFontProblem.md)
