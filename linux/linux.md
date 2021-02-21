# linux 下遇到的一些问题、
[toc]

## 1 centos出现错误的依赖
### 1.1 多个源之间有冲突
发生现象：
当我安装了rpmfusion-free-release.noarch 和 nux两个源时，此时我安装VLC player，提示依赖错误。
解决办法：
后发现是nux源和rpmfusion-free-release源有冲突，卸载rpmfusion-free-release


- [上一级](README.md)
- 上一篇 -> [fedora install netease cloud music](install-netease-cloud-music.md)
- 下一篇 -> [开源库 muduo 学习笔记](muduo.md)
