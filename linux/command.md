<!-- linux实用命令集合 -->
1. du -sh 获取当前目录的空间使用量
2. du -d1 -h 获取当前目录的下一级的空间使用量
3. ps -ef | grep '..process..' | grep -v grep |awk '{print $2}'| xargs '..command..' 对符合条件的所有进程进行批量操作


---
- [上一级](README.md)
- 上一篇 -> [理解chroot](chroot.md)
- 下一篇 -> [在 Linux 上搭建ARM应用程序开发环境](createArmDebugEnv.md)
