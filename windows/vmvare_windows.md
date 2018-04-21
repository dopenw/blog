# Vmvare_window使用问题及解决方法
1. 注销windows后，虚拟机未正常关机，vmvare提示：
```
此虚拟机似乎正在使用中。
如果此虚拟机已在使用中，请按“取消”按钮，以免损坏它。如果此虚拟机未使用，请按“取得所有权(&T)”按钮以获取它的所有权。
```
此时并不能够获得所有权，这个时候可以移除vmvare_path/\*.lck file 来解决该问题，附vmvare lock file desc：

          Lock Files

          A running virtual machine creates lock files to prevent consistency problems on virtual disks. If the virtual machine did not use locks, multiple virtual machines might read and write to the disk, causing data corruption.

          Lock files are always created in the same directory as the .vmdk files.

          The locking methods used by VMware Workstation on Windows and Linux hosts are different, so files shared between them are not fully protected. If you use a common file repository that provides files to users on both Windows and Linux hosts, be sure that each virtual machine is run by only one user at a time.

          When a virtual machine is powered off, it removes the lock files it created. If it cannot remove the lock, a stale lock file is left protecting the .vmdk file. For example, if the host machine crashes before the virtual machine has a chance to remove its lock file, a stale lock remains.

          If a stale lock file remains when the virtual machine is started again, the virtual machine tries to remove the stale lock. To make sure that no virtual machine could be using the lock file, the virtual machine checks the lock file to see if

          1.	The lock was created on the same host where the virtual machine is running.

          2.	The process that created the lock is not running.

          If those two conditions are true, the virtual machine can safely remove the stale lock. If either of those conditions is not true, a dialog box appears, warning you that the virtual machine cannot be powered on. If you are sure it is safe to do so, you may delete the lock files manually. On Windows hosts, the filenames of the lock files end in .lck. On Linux hosts, the filenames of the lock files end in .WRITELOCK.

          Physical disk partitions are also protected by locks. However, the host operating system is not aware of this locking convention and thus does not respect it. For this reason, VMware strongly recommends that the physical disk for a virtual machine not be installed on the same physical disk as the host operating system.


[上一级](base.md)
[上一篇](insertUSBDevicesNotResponse.md)
[下一篇](windows_linux.md)
