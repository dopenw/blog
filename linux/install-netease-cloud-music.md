# fedora install netease cloud music

1. Download netease cloud music .deb file
[Download](https://music.163.com/#/download)
2. Convert .deb to .rpm file
```sh
sudo alien -r netease-cloud-music.deb
```
3. Install dependencies
```sh
dnf install qt5-qtgamepad jasper
```
4. Install netease cloud music
```sh
rpm -ivh --nodeps --force netease-cloud-music.rpm
```
[上一级](base.md)
[上一篇](gnomeUSBError.md)
[下一篇](linux.md)
