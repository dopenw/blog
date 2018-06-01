# Create git server


<!-- @import "[TOC]" {cmd="toc" depthFrom=1 depthTo=6 orderedList=false} -->
<!-- code_chunk_output -->

* [Create git server](#create-git-server)
	* [Install git](#install-git)
	* [Create git user](#create-git-user)
	* [Add your ssh key to the accesss list](#add-your-ssh-key-to-the-accesss-list)
	* [Create git server](#create-git-server-1)
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

## Create git server
In server:
```sh
git init --bare my-project.git
cd my-project.git
git init && git remote add origin git@123.45.56.78:my-project.git
```

## Link
* [How To Set Up a Private Git Server on a VPS](https://www.digitalocean.com/community/tutorials/how-to-set-up-a-private-git-server-on-a-vps)
* [How to Run Your Own Git Server](https://www.linux.com/learn/how-run-your-own-git-server)
* [SSH Passwordless Login Using SSH Keygen in 5 Easy Steps](https://www.tecmint.com/ssh-passwordless-login-using-ssh-keygen-in-5-easy-steps/)

[上一级](base.md)
[上一篇](command.md)
[下一篇](ctrl_alt_f3_login_incorrect.md)
