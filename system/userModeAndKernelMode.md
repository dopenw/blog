# 用户模式和内核模式

为了使操作系统内核提供一个无懈可击的进程抽象，```处理器必须提供一种机制，限制一个应用可以执行的指令以及它可以访问的地址空间范围。```

处理器通常是用某个控制寄存器的一个模式位 (mode bit) 来提供这种功能的，该寄存器描述了进程当前享有的特权。当设置了模式位时，进程就运行在内核模式中（有时叫做超级用户模式）。```一个运行在内核模式的进程可以执行指令集中的任何指令，并且可以访问系统中的任何存储器位置。```

没有设置模式位时，进程就运行在用户模式中。 ```用户模式中的进程不允许执行特权指令，比如停止处理器、改变模式位，或者发起一个 I/O 操作。 也不允许用户模式中的进程直接引用地址空间中内核区内的代码和数据。任何这样的操作都会导致致命的保护故障。反之，用户程序必须通过系统调用接口间接地访问内核代码和数据。```

```highlight
运行应用程序代码的进程初始时是在用户模式中的。
```
 进程从用户模式变为内核模式的唯一方法是通过诸如中断、故障或者陷入系统调用这样的异常。当异常发生时，控制传递到异常处理程序，处理器将模式从用户模式变为内核模式。处理程序运行在内核模式中，当它返回到应用程序代码时，处理器就把模式从内核模式改回用户模式。

 linux 提供了一个聪明的机制，叫做 /proc 文件系统，它允许用户模式进程访问内核数据结构的内容。 /proc 文件系统，它允许用户模式访问内核数据结构的内容。 /proc 文件系统将许多内核数据结构的内容输出为一个用户程序可以读的文本文件的层次结构。比如，你可以使用 /proc 文件系统找出一般的系统属性，比如 CPU 类型 (/proc/cpuinfo),或者某个特殊的进程使用的存储器段 (/proc/<process id>/maps) 。 2.6 版本的 linux 内核引入了 /sys 文件系统，它输出关于系统总线和设备的额外的底层信息。

```sh
[breap@breap ~]$ cat /proc/meminfo
MemTotal:        8067300 kB
MemFree:         2619660 kB
MemAvailable:    4203660 kB
Buffers:          226772 kB
Cached:          1544228 kB
SwapCached:            0 kB
Active:          3857824 kB
Inactive:         976152 kB
Active(anon):    3066684 kB
Inactive(anon):    72352 kB
Active(file):     791140 kB
Inactive(file):   903800 kB
Unevictable:         740 kB
Mlocked:             740 kB
SwapTotal:       1994748 kB
SwapFree:        1994748 kB
Dirty:               312 kB
Writeback:             0 kB
AnonPages:       3063780 kB
Mapped:           756984 kB
Shmem:             76072 kB
Slab:             297788 kB
SReclaimable:     194856 kB
SUnreclaim:       102932 kB
KernelStack:       18624 kB
PageTables:        93900 kB
NFS_Unstable:          0 kB
Bounce:                0 kB
WritebackTmp:          0 kB
CommitLimit:     6028396 kB
Committed_AS:   12967632 kB
VmallocTotal:   34359738367 kB
VmallocUsed:           0 kB
VmallocChunk:          0 kB
HardwareCorrupted:     0 kB
AnonHugePages:         0 kB
ShmemHugePages:        0 kB
ShmemPmdMapped:        0 kB
CmaTotal:              0 kB
CmaFree:               0 kB
HugePages_Total:       0
HugePages_Free:        0
HugePages_Rsvd:        0
HugePages_Surp:        0
Hugepagesize:       2048 kB
DirectMap4k:      416508 kB
DirectMap2M:     7872512 kB
```

```sh
[breap@breap ~]$ ls /sys
block  class  devices   fs          kernel  power
bus    dev    firmware  hypervisor  module
```

注：参考 《深入理解计算机系统 第二版》 8.2.4 章节
[上一级](README.md)
[上一篇 -> 数据结构（树）](tree.md)
