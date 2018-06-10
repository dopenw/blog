# linux 恢复 GRUB2

## 创建 fedora 启动盘

利用 [Fedora Media Writer](https://getfedora.org/en/workstation/download/) 创建启动盘

## 启动 fedora Live CD 并运行

```sh
# example
su -
mount /dev/mapper/fedora-root /mnt
mount /dev/sda3 /mnt/boot
mount -B /dev /mnt/dev
mount -B /proc  /mnt/proc
mount -B /sys /mnt/sys
chroot /mnt
grub2-install /dev/sda
grub2-mkconfig -o /boot/grub2/grub.cfg
exit
reboot
```


[Can't restore GRUB2 via Boot-Repair-Disk](https://ask.fedoraproject.org/en/question/86357/solved-cant-restore-grub2-via-boot-repair-disk/)

[How to reinstall or fix GRUB in Fedora without a recovery option on the Live CD?](https://ask.fedoraproject.org/en/question/40578/how-to-reinstall-or-fix-grub-in-fedora-without-a-recovery-option-on-the-live-cd/?answer=40593#post-id-40593)

[上一级](base.md)
[上一篇](rename_linux_user.md)
[下一篇](shutterException.md)
