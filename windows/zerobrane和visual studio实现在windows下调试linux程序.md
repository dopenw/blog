# 前言
因公司开发的软件是在linux服务器上运作的，
而常用的操作系统是windows，为了解决调试问题，
就有了用这个方式来调试linux下的Cpp和lua相关代码
## 工具
VisualStudio 2010，zerobrane，visual gdb

### VisualStudio
1. VisualStudio create linux project
2. Create new ssh connection
3. Make local_source_code -- remote_source_code
3. Change visual GDB setting (project setting,
build setting,debug setting,custom build steps)

### zerobrane

#### tools:luasocket

1. install luasocket library(By yum,source code,luarocks)
2. Cp (zerobrane)lualibs/mobdebug/mobdebug.lua to
/usr/share/lua/5.x/ (64 bits)
3. open zerobrane application and click "set project file
 from current file" and set Project->Start Debugger server
4. In debug file add require("mobdebug").start("192.168.XXX.1")
