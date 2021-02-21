# Create git server


<!-- @import "[TOC]" {cmd="toc" depthFrom=1 depthTo=6 orderedList=false} -->
<!-- code_chunk_output -->

* [Create git server](#create-git-server)
	* [Install git](#install-git)
	* [Create git user](#create-git-user)
	* [Add your ssh key to the accesss list](#add-your-ssh-key-to-the-accesss-list)
	* [Initialize Git repository](#initialize-git-repository)
	* [Disable shell login](#disable-shell-login)
	* [Link](#link)

<!-- /code_chunk_output -->

## Install git
In server:
```sh
dnf install git
```

## Create git user
In server:
```sh
useradd git
# change passwd
passwd git
```
## Add your ssh key to the accesss list
In client:
```sh
ssh-keygen -t rsa
```

In server:
```sh
su - git
mkdir ~/.ssh && touch ~/.ssh/authorized_keys
chmod 700 ~/.ssh
chmod 640 ~/.ssh/authorized_keys
```

In client:
```sh
cat $HOME/.ssh/id_rsa.pub | ssh user@123.45.56.78 "cat >> ~/.ssh/authorized_keys"
```

## Initialize Git repository
In server:
```sh
git init --bare my-project.git
chown -R git:git my-project.git
```

## Disable shell login
```sh
sudo vim /etc/passwd
```
```sh
# change this
git:x:1001:1001:,,,:/home/git:/bin/bash
# to
git:x:1001:1001:,,,:/home/git:/usr/bin/git-shell
```



## Link
* [How To Set Up a Private Git Server on a VPS](https://www.digitalocean.com/community/tutorials/how-to-set-up-a-private-git-server-on-a-vps)
* [How to Run Your Own Git Server](https://www.linux.com/learn/how-run-your-own-git-server)
* [SSH Passwordless Login Using SSH Keygen in 5 Easy Steps](https://www.tecmint.com/ssh-passwordless-login-using-ssh-keygen-in-5-easy-steps/)
* [廖学峰：搭建Git服务器
](https://www.liaoxuefeng.com/wiki/0013739516305929606dd18361248578c67b8067c8c017b000/00137583770360579bc4b458f044ce7afed3df579123eca000)

[上一级](README.md)
[上一篇 -> 在 Linux 上搭建ARM应用程序开发环境](createArmDebugEnv.md)
[下一篇 -> ctl + alt + f3 login incorrect](ctrl_alt_f3_login_incorrect.md)
