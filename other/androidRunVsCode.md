# install VS Code in an Android Device


<!-- @import "[TOC]" {cmd="toc" depthFrom=1 depthTo=6 orderedList=false} -->

<!-- code_chunk_output -->

- [install VS Code in an Android Device](#install-vs-code-in-an-android-device)

<!-- /code_chunk_output -->

1. Install [termux](https://github.com/termux/termux-app/releases)
2. run command on termux :
    - `pkg update `
    - `pkg install proot-distro ` 
    - `proot-distro install ubuntu`
    - `proot-distro login ubuntu`
    - `apt update -y`
    - `apt upgrade`
    - `apt install sudo vim -y`
    - `adduser $USERNAME`
    - `vim /etc/sudoers`
    ```sh
    # User privilege specification 
    root ALL=(ALL:ALL) ALL 
    $USERNAME ALL=(ALL:ALL) ALL # add it 
    ```
    - `apt install wget git`
    - `su $USERNAME`
    - 
    ```sh
    wget https://github.com/coder/code-server/releases/download/v4.2.0/code-server-4.2.0-linux-arm64.tar.gz # 最新的包
    tar -xzf code-server-4.2.0-linux-arm64.tar.gz
    ```
    - `echo "export PASSWORD=\"1qaz2wsx3edc\"" >> ~/.bashrc`
    - `cd code-server-4.2.0-linux-arm64/bin`
    - `./code-server`
3. 在 termux 和 安卓设备上共享数据，可见 [Internal and external storage](https://wiki.termux.com/wiki/Internal_and_external_storage)
4. 在 termux ubuntu 中访问外置盘数据可进入路径 - `/sdcard`

---

## Link 
- [How to install VS Code in an Android Phone?](https://www.codewithharry.com/blogpost/install-vs-code-in-android)
- [code-server@github](https://github.com/coder/code-server)
- [Internal and external storage](https://wiki.termux.com/wiki/Internal_and_external_storage)

---

- [上一级](README.md)
- 上一篇 -> [利用 Fusion App 创建个人blog安卓客户端](UseFusionAppCreateBlogApp.md)
- 下一篇 -> [Atom](atom.md)
