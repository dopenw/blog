# 在windows下编译调试c++和lua代码
## 前言
因公司开发的软件是在linux服务器上运作的，
而常用的操作系统是windows，为了解决调试问题，
就有了通过visualstudio和zerobrane来调试linux下的Cpp和lua相关代码的方法
## software
VisualStudio，Zerobrane，Visual GDB

1. VisualStudio create linux project(Need visual gdb)
2. Create new ssh connection
3. Make local_source_code -- remote_source_code
3. Change visual GDB setting (project setting,
build setting,debug setting,custom build steps)



1. Install luasocket library(By yum,source code,luarocks)
2. Cp (zerobrane)lualibs/mobdebug/mobdebug.lua to
/usr/share/lua/5.x/ (64 bits)
3. Open zerobrane application and click "set project file
 from current file" and set Project->Start Debugger server
4. Add require("mobdebug").start("192.168.XXX.1") in debug file










































[上一级](base.md)
[上一篇](windows_linux.md)
