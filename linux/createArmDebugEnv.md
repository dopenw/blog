#在 Linux 上搭建ARM应用程序开发环境


<!-- @import "[TOC]" {cmd="toc" depthFrom=1 depthTo=6 orderedList=false} -->

<!-- code_chunk_output -->

- [安装串口工具 minicom](#安装串口工具-minicom)
- [安装 eclipse](#安装-eclipse)
- [搭建交叉编译环境](#搭建交叉编译环境)
- [安装 git 及 git gui 工具](#安装-git-及-git-gui-工具)
- [配置虚拟机的网络适配器](#配置虚拟机的网络适配器)
- [配置 eclipse ,实现远程调试 arm 程序](#配置-eclipse-实现远程调试-arm-程序)
- [Link](#link)

<!-- /code_chunk_output -->


注：使用的 linux 发布版为 ubuntu 16.04LTS 

## 安装串口工具 minicom
```sh
sudo apt install minicom
```
可参考 [ubuntu下串口工具的安装与使用](https://blog.csdn.net/lzhitwh/article/details/80304579)

## 安装 eclipse
[下载](https://www.eclipse.org/cdt/)，然后解压到指定文件夹，直接运行 eclipse 文件即可；

## 搭建交叉编译环境
此处暂时省略


## 安装 git 及 git gui 工具
```sh
sudo apt install git git-cola
```

## 配置虚拟机的网络适配器
[怎样解决VMware虚拟机无法连接外网问题](https://blog.csdn.net/yang5726685/article/details/78566213)

## 配置 eclipse ,实现远程调试 arm 程序
可参考 [Windows下ARM Linux应用程序开发环境搭建说明](https://zhuanlan.zhihu.com/p/25463241)

注意：需找到对应的 gdbserver 放到板卡系统中的 `/usr/bin` 下；eg：
```sh 
/mnt/workspace/toolchains/SDK/2015.4/gnu/arm/lin/arm-xilinx-linux-gnueabi/libc/usr/bin/gdbserver
```


## Link 
* [Debugging shared libraries with gdbserver](https://stackoverflow.com/questions/8611194/debugging-shared-libraries-with-gdbserver)
* [Fixing the Eclipse C++ remote execution “Error during file upload” error](https://www.kevinhooke.com/2015/08/16/fixing-the-eclipse-c-remote-execution-error-during-file-upload-error/)
* [How to add library path in Eclipse CDT](https://codeyarns.com/tech/2013-09-22-how-to-add-library-path-in-eclipse-cdt.html)
* [How to Add Existing Files to Eclipse Projects](https://dzone.com/articles/how-add-existing-files-eclipse)
* [Eclipse remote development and debugging](https://www.96boards.org/blog/eclipse-remote-development-debugging/)
* [Windows下ARM Linux应用程序开发环境搭建说明](https://zhuanlan.zhihu.com/p/25463241)
* [GDB 调试遇到??的问题](https://www.cnblogs.com/jlmgary/p/6198307.html)
* [Debugging shared libraries with gdbserver](https://stackoverflow.com/questions/8611194/debugging-shared-libraries-with-gdbserver)
 
[上一级](README.md)
[上一篇](command.md)
[下一篇](createGitServer.md)
