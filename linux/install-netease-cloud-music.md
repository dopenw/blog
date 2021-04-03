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

[Fedora 中文社区软件源](https://github.com/FZUG/repo)

## Link
* [重打包网易云音乐1.1? ](https://github.com/FZUG/repo/issues/288)


---
- [上一级](README.md)
- 上一篇 -> [惠普暗影精灵4安装fedora 28](hpOmenInstallFedora.md)
- 下一篇 -> [linux 下遇到的一些问题、](linux.md)
