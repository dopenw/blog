# 进程相关
1. 使用nohup运行程序，此时会忽略SIGHUP信号
2. 当控制终端接口检测到调制解调器断开（直接关闭终端，不是exit）或网络已经断开连接，则将挂断信号发送至控制进程（会话首进程）

~~可使用killall -9 `nohup_proc_name`
or kill `nohup_proc_ID`来终止nohup进程
发现进程挂了之后，重新启动程序，可马上就显示exit进程信息。刚开始以为是程序遇到致命错误无法启动，但是后来发现是磁盘空间不足，导致不能正常运行程序~~

hfjdkshfkj















































































[上一级](base.md)
[上一篇](linux.md)
